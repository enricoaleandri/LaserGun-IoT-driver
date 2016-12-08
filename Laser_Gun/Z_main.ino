

int Xdirect = 4;
int Ydirect = 4;
//MotorTask task[] ={MotorTask("",0,0,0,0),MotorTask("",0,0,0,0),MotorTask("",0,0,0,0),MotorTask("",0,0,0,0)};//
  
void setup() {
  Serial.begin(115200);
  //SPIFFS.format();
  WifiManaged::WifiManagedInit();
  //mqtt = MQTT();
  //UDPServerInit();
  SerialComunication::info("Main", "setup", "Test SerialComunication class");
  
  laserServoX.attach(14);  // D5
  laserServoY.attach(12);  // D6

  //task[0] = MotorTask( "Mattutino", 8, 0, 60,1);
  //task[1] = MotorTask( "Tarta mattinata", 11, 0, 60,1);
  //task[2] = MotorTask( "Pomeridiano", 15, 0, 60,1);
  //task[3] = MotorTask( "Serale", 21, 0, 60,1);
  
}

void loop() {
  
  /*String result = UDPServerReadData();
  if(result != ""){
    Serial.print(result);
    laserController.parseUDPMessage(result);
  }
//  laserController.parseUDPMessage(result);
*/

  /*if (!client.connected()) {
    mqtt.reconnect();
  }
  client.loop();*/


}
