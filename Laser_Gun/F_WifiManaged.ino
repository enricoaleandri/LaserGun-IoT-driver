

//MQTT mqtt;
bool shouldSaveConfig = false;
class WifiManaged {
    public :
    //callback notifying us of the need to save config
    static void saveConfigCallback () {
      SerialComunication::info("WifiManaged", "saveConfigCallback", "Should save config");
      shouldSaveConfig = true;
    }
    static void configModeCallback (WiFiManager *myWiFiManager) {
      SerialComunication::info("WifiManaged", "configModeCallback", "Entered config mode");
      //mqtt.setupMqtt();// reset configuration retrived from WifiManager configuration
      //Serial.println(WiFi.softAPIP());
      //Serial.println(myWiFiManager->getConfigPortalSSID());
    }
    
    static void WifiManagedInit () {
      //clean FS, for testing
      //SPIFFS.format();
      char* mqtt_server = "m20.cloudmqtt.com"; // heroku app custom mosquito
      char* mqtt_username = "kjmlqogz"; // heroku app custom mosquito
      char* mqtt_password = "PMHIJ5JGoY2g"; // heroku app custom mosquito
      char* mqtt_port = "11181"; // heroku app custom mosquito

      //read configuration from FS json
      SerialComunication::info("WifiManaged", "WifiManagedInit", "mounting FS...");

      JsonObject& json = configManager::getConfig("");

      if(json.success()){
        SerialComunication::info("WifiManaged", "WifiManagedInit", "Config retrived!");
        if(json["mqtt_server"] != "" && json["mqtt_port"] != ""
            && json["mqtt_username"] != "" && json["mqtt_password"] != ""){
              
            SerialComunication::info("WifiManaged", "WifiManagedInit", "MQTT config found!");
            strcpy(mqtt_server, json["mqtt_server"]);
            strcpy(mqtt_port, json["mqtt_port"]);
            strcpy(mqtt_username, json["mqtt_username"]);
            strcpy(mqtt_password, json["mqtt_password"]);
        }
      }
      //end read
      // The extra parameters to be configured (can be either global or just in the setup)
      // After connecting, parameter.getValue() will get you the configured value
      // id/name placeholder/prompt default length
      WiFiManagerParameter custom_mqtt_server("server", "mqtt server", mqtt_server, 40);
      WiFiManagerParameter custom_mqtt_port("port", "mqtt port", mqtt_port, 6);
      WiFiManagerParameter custom_mqtt_username("mqtt Username", "mqtt_username", mqtt_username, 9);
      WiFiManagerParameter custom_mqtt_password("mqtt Password", "mqtt_password", mqtt_password, 14);
    
      //WiFiManager
      //Local intialization. Once its business is done, there is no need to keep it around
      WiFiManager wifiManager;
    
      //set config save notify callback
      wifiManager.setSaveConfigCallback(WifiManaged::saveConfigCallback);

      //set AutoConect callback for reconnect MQTT server with new configuration
      wifiManager.setAPCallback(WifiManaged::configModeCallback);
    
      //set static ip
      //wifiManager.setSTAStaticIPConfig(IPAddress(10,0,1,99), IPAddress(10,0,1,1), IPAddress(255,255,255,0));
      
      //add all your parameters here
      wifiManager.addParameter(&custom_mqtt_server);
      wifiManager.addParameter(&custom_mqtt_port);
      wifiManager.addParameter(&custom_mqtt_username);
      wifiManager.addParameter(&custom_mqtt_password);
    
      //reset settings - for testing
      //wifiManager.resetSettings();
    
      //set minimu quality of signal so it ignores AP's under that quality
      //defaults to 8%
      //wifiManager.setMinimumSignalQuality();
      
      //sets timeout until configuration portal gets turned off
      //useful to make it all retry or go to sleep
      //in seconds
      //wifiManager.setTimeout(120);
    
      //fetches ssid and pass and tries to connect
      //and goes into a blocking loop awaiting configuration
      if (!wifiManager.autoConnect("LaserGUN-Enrico", "123456789")) {
        SerialComunication::info("WifiManaged", "WifiManagedInit", "failed to connect and hit timeout");
        delay(3000);
        //reset and try again, or maybe put it to deep sleep
        ESP.reset();
        delay(5000);
      }
    
      //if you get here you have connected to the WiFi
      SerialComunication::info("WifiManaged", "WifiManagedInit", "Connected...  :D :D");
    
      //read updated parameters
      strcpy(mqtt_server, custom_mqtt_server.getValue());
      strcpy(mqtt_port, custom_mqtt_port.getValue());
      strcpy(mqtt_username, custom_mqtt_username.getValue());
      strcpy(mqtt_password, custom_mqtt_password.getValue());
    
      //save the custom parameters to FS
      if (shouldSaveConfig) {
        SerialComunication::info("WifiManaged", "WifiManagedInit", "saving config");
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.createObject();
        json["mqtt_server"] = mqtt_server;
        json["mqtt_port"] = mqtt_port;
        json["mqtt_username"] = mqtt_username;
        json["mqtt_password"] = mqtt_password;
    
        File configFile = SPIFFS.open(configFileName, "w");
        if (!configFile) {
          SerialComunication::info("WifiManaged", "WifiManagedInit", "failed to open config file for writing");
        }
    
        json.printTo(Serial);
        json.printTo(configFile);
        configFile.close();
        //end save
      }
    
      SerialComunication::info("WifiManaged", "WifiManagedInit", "local ip");
      SerialComunication::info("WifiManaged", "WifiManagedInit", String(WiFi.localIP()));
    } 
};


