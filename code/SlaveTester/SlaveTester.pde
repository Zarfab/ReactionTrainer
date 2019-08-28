import websockets.*;

WebsocketServer ws;

JSONObject json;


void setup() 
{
  size(200, 200);
  ws= new WebsocketServer(this, 8025, "/rt");
  json = new JSONObject();
}



void draw() {
  background(0);
}


void webSocketServerEvent(String msg){
 println(msg);
}


void keyPressed() {
  if(key == 'w') {
    json.setInt("distanceThreshold", 150);
    json.setInt("timeout", 2500);
    json.setInt("order", 0);
    
    json.setInt("colorDuration", 1500);
    JSONArray palette = new JSONArray();
    palette.append(color(255, 64, 127));
    palette.append(color(0));
    json.setJSONArray("colorPalette", palette);
    json.setInt("colorAnim", 0);
    
    json.setInt("soundDuration", 200);
    json.setInt("soundFreq", 392);
    println(json);
    ws.sendMessage(json.toString());
  }
  if(key == 'g') {
    JSONObject group = new JSONObject();
    group.setInt("brightness", 25);
    group.setInt("groupColor", color(0, 168, 255));
    println(group);
    ws.sendMessage(group.toString());
  }
}
