import websockets.*;
import processing.serial.*;
import controlP5.*;

WebsocketServer ws;

boolean useSerial = true;
Serial serial;

JSONObject json;
ControlP5 cp5;
PaletteItem palette[];
int paletteIndexMouseDown = -1;
int paletteIndexMouseUp = -1;

void setup() 
{
  size(640, 520);
  ws= new WebsocketServer(this, 8025, "/rt");
  json = new JSONObject();
  
  // List all the available serial ports:
  if(useSerial) {
    printArray(Serial.list());
    serial = new Serial(this, Serial.list()[0], 115200);
  }
  
  
  // add controls
  cp5 = new ControlP5(this);
  cp5.addSlider("brightness")
     .setPosition(200,20)
     .setSize(width - 300, 40)
     .setRange(0,255)
     .setValue(60)
     .getCaptionLabel().align(ControlP5.LEFT, ControlP5.BOTTOM_OUTSIDE)
     ;
  cp5.addColorWheel("groupColor" , 20 , 0 , 160 )
    .setRGB(color(168, 168, 24))
    ;
  cp5.addButton("SendGroupSettings")
     .setPosition(width - 160, 100)
     .setSize(120, 40)
     ;
     
  cp5.addSlider("distanceThreshold")
     .setPosition(20, 190)
     .setSize(180, 20)
     .setRange(0,250)
     .setValue(150)
     .getCaptionLabel().align(ControlP5.LEFT, ControlP5.BOTTOM_OUTSIDE)
     ;
  cp5.addSlider("timeout")
     .setPosition(230, 190)
     .setSize(180, 20)
     .setRange(400, 5000)
     .setValue(2600)
     .getCaptionLabel().align(ControlP5.LEFT, ControlP5.BOTTOM_OUTSIDE)
     ;
  String orderItems[] = {"SIMULTANEOUS", "COLOR_BEFORE", "SOUND_BEFORE"};
  cp5.addScrollableList("order")
     .setPosition(440, 190)
     .setSize(180, 80)
     .setBarHeight(20)
     .setItemHeight(20)
     .addItems(orderItems)
     .setOpen(false)
     .setValue(0)
     ;

  String colorAnimItems[] = {"NONE", "STILL", "BLINK", "TIMER", "STOPWATCH", "LOADING", "WHEEL"};
  cp5.addScrollableList("colorAnim")
     .setPosition(20, 240)
     .setSize(180, 80)
     .setBarHeight(20)
     .setItemHeight(20)
     .addItems(colorAnimItems)
     .setOpen(false)
     .setValue(0)
     ;
  cp5.addSlider("colorDuration")
     .setPosition(20, 270)
     .setSize(180, 20)
     .setRange(400, 5000)
     .setValue(1000)
     .getCaptionLabel().align(ControlP5.LEFT, ControlP5.BOTTOM_OUTSIDE)
     ;
  cp5.addColorWheel("paletteColor" , 20 , 320 , 180 )
    .setRGB(color(168, 168, 24));
  palette = new PaletteItem[16];
  int paletteItemH = (height - 240 - 20) / 16;
  for(int i = 0; i < 16; i++) {
    palette[i] = new PaletteItem(220, 240 + i * paletteItemH, 80, paletteItemH);
  }
  
  // create a toggle and change the default look to a (on/off) switch look
  cp5.addToggle("useSound")
     .setPosition(width * 0.75 - 30, 240)
     .setSize(60, 20)
     .setValue(true)
     .setMode(ControlP5.SWITCH)
     ;
  cp5.addSlider("soundDuration")
     .setPosition(width * 0.75 - 120, 280)
     .setSize(240, 20)
     .setRange(400, 5000)
     .setValue(1000)
     .getCaptionLabel().align(ControlP5.LEFT, ControlP5.BOTTOM_OUTSIDE)
     ;
  cp5.addKnob("soundFreq")
     .setPosition(width * 0.75 - 70, 320)
     .setSize(140, 140)
     .setRange(20, 880)
     .setValue(392)
     ;
     
  cp5.addButton("SendShot")
     .setPosition(width - 160, height - 30)
     .setSize(120, 20)
     ;
}



/*
public void useSound(boolean value) {
  if(value) {
    cp5.get("soundDuration").show();
    cp5.get("soundFreq").show();
  }
  else {
    cp5.get("soundDuration").hide();
    cp5.get("soundFreq").hide();
  }
}*/


public void paletteColor(int c) {
  for(int i = 0; i < 16; i++) {
    if(palette[i].selected) {
      palette[i].c = c;
    }
  }
}

public void SendGroupSettings() {
  JSONObject group = new JSONObject();
  group.setInt("brightness", int(cp5.get("brightness").getValue()));
  group.setInt("groupColor", cp5.get(ColorWheel.class,"groupColor").getRGB());
  String toSend = group.toString();
  System.err.println(toSend);
  toSend = toSend.replaceAll("\\s+", "").replaceAll("\\t+", "").replaceAll("\\n", "");
  ws.sendMessage(toSend);
  if(useSerial) serial.write(toSend);
}

public void SendShot() {
  json.setInt("distanceThreshold", int(cp5.get("distanceThreshold").getValue()));
  json.setInt("timeout", int(cp5.get("timeout").getValue()));
  json.setInt("order", int(cp5.get("order").getValue()));
  
  json.setInt("colorDuration", int(cp5.get("colorDuration").getValue()));
  JSONArray paletteJson = new JSONArray();
  for(int i = 0; i < 16; i++) paletteJson.append(palette[i].c);
  json.setJSONArray("colorPalette", paletteJson);
  json.setInt("colorAnim",  int(cp5.get("colorAnim").getValue()));
  
  json.setInt("soundDuration", int(cp5.get("soundDuration").getValue()));
  int freq = -1;
  if(cp5.get("useSound").getValue() != 0)
    freq = int(cp5.get("soundFreq").getValue());
  json.setInt("soundFreq", freq);
  String toSend = json.toString();
  System.err.println(toSend);
  ws.sendMessage(toSend);
  if(useSerial) serial.write(toSend);
}



void draw() {
  background(127);
  stroke(255);
  strokeWeight(4);
  line(0, 180, width, 180);
  strokeWeight(1);
  line(0, 230, width, 230);
  line(width/2, height - 40, width, height - 40);
  line(width/2, 230, width/2, height);
  
  for(int i = 0; i < 16; i++) {
    palette[i].draw();
  }
  if(useSerial) {
    while (serial.available() > 0) {
      print(serial.readChar());
    }
  }
}


void webSocketServerEvent(String msg){
 println(msg);
}



void mousePressed() {
  paletteIndexMouseDown = -1;
  for(int i = 0; i < 16; i++) {
    if(palette[i].mouseOver()) {
      paletteIndexMouseDown = i;
      return;
    }
  }
}

void mouseReleased() {
  boolean mouseOverPaletteColor = mouseX >= 20 && mouseY >= 320 &&
        mouseX <= 20 + 180 && mouseY <= 320 + 180;
  if(!mouseOverPaletteColor) {
    paletteIndexMouseUp = -1;
    for(int i = 0; i < 16; i++) {
      if(palette[i].mouseOver()) {
        paletteIndexMouseUp = i;
        break;
      }
    }
    if(paletteIndexMouseUp < paletteIndexMouseDown) {
      int temp = paletteIndexMouseUp;
      paletteIndexMouseUp = paletteIndexMouseDown;
      paletteIndexMouseDown = temp;
    }
    for(int i = 0; i < 16; i++) {
      if(paletteIndexMouseDown >= 0 && i >= paletteIndexMouseDown && i <= paletteIndexMouseUp)
        palette[i].selected = true;
      else
        palette[i].selected = false;
    }
  }
}
