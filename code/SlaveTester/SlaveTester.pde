import processing.serial.*;

Serial serial;



void setup() 
{
  size(200, 200);
  printArray(Serial.list());
  serial = new Serial(this, Serial.list()[1], 115200);
}



void draw() {
  if ( serial.available() > 0) {  // If data is available,
    print(serial.read());         // read it and store it in val
  }
}
