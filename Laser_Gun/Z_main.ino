
ServoLaserMotor laserController;

int Xdirect = 4;
int Ydirect = 4;

void setup() {
  Serial.begin(9600);
  WifiManaged:WifiManaged();
  //UDPServerInit();
  SerialComunication::info("Main", "setup", "Test SerialComunication class");
  
  laserServoX.attach(14);  // D5
  laserServoY.attach(12);  // D6
}

void loop() {
  /*String result = UDPServerReadData();
  if(result != ""){
    Serial.print(result);
    laserController.parseUDPMessage(result);
  }
//  laserController.parseUDPMessage(result);
*/

}
