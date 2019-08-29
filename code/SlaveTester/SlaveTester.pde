import websockets.*;
import processing.serial.*;

WebsocketServer ws;

boolean useSerial = false;
Serial serial;

JSONObject json;


void setup() 
{
  size(200, 200);
  ws= new WebsocketServer(this, 8025, "/rt");
  json = new JSONObject();
  
  // List all the available serial ports:
  if(useSerial) {
    printArray(Serial.list());
    serial = new Serial(this, Serial.list()[0], 115200);
  }
}



void draw() {
  background(0);
  if(useSerial) {
    while (serial.available() > 0) {
      println(serial.readChar());
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
    println(toSend);
    toSend = toSend.replaceAll("\\s+", "").replaceAll("\\t+", "").replaceAll("\\n", "");
    ws.sendMessage(toSend);
    if(useSerial) serial.write(toSend);
  }
}
