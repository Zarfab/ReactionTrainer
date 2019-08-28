import websockets.*;

WebsocketServer ws;

JSONObject json;


void setup() 
{
  size(200, 200);
  ws= new WebsocketServer(this, 81, "/");
}



void draw() {
}


void webSocketServerEvent(String msg){
 println(msg);
}
