
#include <FS.h>                   //this needs to be first, or it all crashes and burns...

#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino

//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager

#include <ArduinoJson.h>  

//define your default values here, if there are different values in config.json, they are overwritten.
char mqtt_server[40] = "m20.cloudmqtt.com";
char mqtt_port[6] = "16292";
char mqtt_username[9];
char mqtt_password[13];
bool shouldSaveConfig = false;
    
class WifiManaged {
    public :
    


    //callback notifying us of the need to save config
    static void saveConfigCallback () {
      SerialComunication::info("WifiManaged", "saveConfigCallback", "Should save config");
      shouldSaveConfig = true;
    }
    
    static void WifiManagedInit () {
      //clean FS, for testing
      //SPIFFS.format();

      //read configuration from FS json
      SerialComunication::info("WifiManaged", "WifiManagedInit", "mounting FS...");
    
      if (SPIFFS.begin()) {
        SerialComunication::info("WifiManaged", "WifiManagedInit", "mounted file system");
        if (SPIFFS.exists("/config.json")) {
          //file exists, reading and loading
          SerialComunication::info("WifiManaged", "WifiManagedInit", "reading config file");
          File configFile = SPIFFS.open("/config.json", "r");
          if (configFile) {
            SerialComunication::info("WifiManaged", "WifiManagedInit", "opened config file");
            size_t size = configFile.size();
            // Allocate a buffer to store contents of the file.
            std::unique_ptr<char[]> buf(new char[size]);
    
            configFile.readBytes(buf.get(), size);
            DynamicJsonBuffer jsonBuffer;
            JsonObject& json = jsonBuffer.parseObject(buf.get());
            json.printTo(Serial);
            if (json.success()) {
              SerialComunication::info("WifiManaged", "WifiManagedInit", "\nparsed json");
    
              strcpy(mqtt_server, json["mqtt_server"]);
              strcpy(mqtt_port, json["mqtt_port"]);
              strcpy(mqtt_username, json["mqtt_username"]);
              strcpy(mqtt_password, json["mqtt_password"]);
    
            } else {
              SerialComunication::info("WifiManaged", "WifiManagedInit", "failed to load json config");
            }
          }
        }
      } else {
        SerialComunication::info("WifiManaged", "WifiManagedInit", "failed to mount FS");
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
    
      //set static ip
      wifiManager.setSTAStaticIPConfig(IPAddress(192,168,1,255), IPAddress(192,168,1,1), IPAddress(255,255,255,0));
      
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
    
        File configFile = SPIFFS.open("/config.json", "w");
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


