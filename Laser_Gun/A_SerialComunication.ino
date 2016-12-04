class SerialComunication{
  public : 
  SerialComunication() {
    
  }
  void setup(){ }


  static char readSerial(){
    if (Serial.available() > 0) 
    {
      return Serial.read();
    }  
    return ' ';
  }

  static void info(String className, String methodName,String mex){
      Serial.println("["+className+"] -> "+methodName+" : INFO : "+mex);
  }
  
  static void error(String className, String methodName, String errorMessage){
      Serial.println("["+className+"] -> "+methodName+" : ERROR : "+errorMessage);
  }
    
};

