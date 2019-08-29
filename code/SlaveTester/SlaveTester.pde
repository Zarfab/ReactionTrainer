import websockets.*;
import processing.serial.*;
import controlP5.*;

WebsocketServer ws;

boolean useSerial = true;
Serial serial;

JSONObject json;
ControlP5 cp5;


void setup() 
{
  size(640, 480);
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
     .setValue(60);
     ;
  cp5.addColorWheel("groupColor" , 20 , 0 , 160 )
    .setRGB(color(168, 168, 24))
    ;
  cp5.addButton("SendGroupSettings")
     .setPosition(width - 160, 100)
     .setSize(120, 40)
     ;
}

public void SendGroupSettings() {
  JSONObject group = new JSONObject();
  group.setInt("brightness", int(cp5.get("brightness").getValue()));
  group.setInt("groupColor", cp5.get(ColorWheel.class,"groupColor").getRGB());
  String toSend = group.toString();
  System.err.println(toSend);
  ws.sendMessage(toSend);
  if(useSerial) serial.write(toSend);
}



void draw() {
  background(127);
  stroke(255);
  strokeWeight(3);
  line(0, 180, width, 180);
  if(useSerial) {
    while (serial.available() > 0) {
      print(serial.readChar());
    }
  }
}


void webSocketServerEvent(String msg){
 println(msg);
}


void keyPressed() {
  String toSend = "";
  if(key == 'w') {
    json.setInt("distanceThreshold", 150);
    json.setInt("timeout", 2500);
    json.setInt("order", 0);
    
    json.setInt("colorDuration", 2400);
    JSONArray palette = new JSONArray();
    for(int i = 0; i < 16; i++) palette.append(color(i*16, 64, 128 - i*8));
    json.setJSONArray("colorPalette", palette);
    json.setInt("colorAnim", 2);
    
    json.setInt("soundDuration", 200);
    json.setInt("soundFreq", -1); // 392
    toSend = json.toString();    
  }
  if(key == 'g') {
    JSONObject group = new JSONObject();
    group.setInt("brightness", 25);
    group.setInt("groupColor", color(168, 168, 24));
    toSend = group.toString(); 
  }
  if(toSend.length() > 1) {
    System.err.println(toSend);
    toSend = toSend.replaceAll("\\s+", "").replaceAll("\\t+", "").replaceAll("\\n", "");
    ws.sendMessage(toSend);
    if(useSerial) serial.write(toSend);
  }
}
