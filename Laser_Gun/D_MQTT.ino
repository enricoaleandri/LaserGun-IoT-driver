
ServoLaserMotor laserController;

WiFiClient espClient;
PubSubClient client(espClient);


class MQTT{
  public:
  
  char* mqtt_channel_server_log = "Enrìc-laser-gun-server-log"; 
  char* mqtt_channel_client = "Enrìc-laser-gun-client"; // heroku app custom mosquito 
  char* mqtt_server; // = "m21.cloudmqtt.com"; // heroku app custom mosquito 
  char* mqtt_username; // = "bmahrqbk"; // heroku app custom mosquito
  char* mqtt_password; // = "CbyFARn4yINP"; // heroku app custom mosquito
  char* mqtt_port; // heroku app custom mosquito*//*
  char msg[100];
  char inData[80];
  
  MQTT(){

      JsonObject& json = configManager::getConfig("");
      
      if(json.success()){
        strcpy(mqtt_server, json["mqtt_server"]);
        strcpy(mqtt_port, json["mqtt_port"]);
        strcpy(mqtt_username, json["mqtt_username"]);
        strcpy(mqtt_password, json["mqtt_password"]);
        client.setServer(mqtt_server, int(mqtt_port));
        client.setCallback(MQTT::mqttCallback);
      }
    
    
  }
  void setup(){}
  
  void mqttLog(String message){

        String stringMessage = "{'message':' "+message+"'}";
        char mex[stringMessage.length()]; 
        stringMessage.toCharArray(mex,stringMessage.length());
        client.publish(mqtt_channel_server_log, mex);
  }
  void setupMqtt(){
    
      JsonObject& json = configManager::getConfig("");
      
      if(json.success()){
        strcpy(mqtt_server, json["mqtt_server"]);
        strcpy(mqtt_port, json["mqtt_port"]);
        strcpy(mqtt_username, json["mqtt_username"]);
        strcpy(mqtt_password, json["mqtt_password"]);
        client.setServer(mqtt_server, int(mqtt_port));
        client.setCallback(MQTT::mqttCallback);
      }
  }
  void reconnect() {
    // Loop until we're reconnected
    while (!client.connected()) {
        SerialComunication::info("MQTT", "reconnect", "Attempting MQTT connection...");
      // Attempt to connect
      if (client.connect(mqtt_server, mqtt_username, mqtt_password)) { // credential
        SerialComunication::info("MQTT", "reconnect", "MQTT connection done.");
        // Once connected, publish an announcement on log topic...
        this->mqttLog("LaserGun online");
        //and Subscribe 
        client.subscribe(mqtt_channel_client);
      } else {
        SerialComunication::info("MQTT", "reconnect", "failed mqtt connection.");
        SerialComunication::info("MQTT", "reconnect",String(client.state()) );
        SerialComunication::info("MQTT", "reconnect", " try again in 5 seconds");
        // Wait 5 seconds before retrying
        delay(5000);
      }
    }
  }
   
  static void mqttCallback(char* topic, byte* payload, unsigned int length) {
    SerialComunication::info("MQTT", "mqttCallback", "Message FROM");
    SerialComunication::info("MQTT", "mqttCallback", String(topic));
    

     String str_payload = "";
     for (int i=0;i<length;i++) {str_payload+= (char)payload[i];}

    SerialComunication::info("MQTT", "mqttCallback", str_payload);
    
    delay(2000);
     //
      // Step 1: Reserve memory space
      //
      StaticJsonBuffer<200> jsonBuffer;
      
      //
      // Step 2: Deserialize the JSON string
      //
      JsonObject& root = jsonBuffer.parseObject(str_payload);
      const int event = root["event"];

      //printText("ENVENT :" +event);
        
     switch(event)
     {
        //SCH one - 1 - Trigger Circular 
        case 1:{ 
          
          break;
        }
     }

  }
};
