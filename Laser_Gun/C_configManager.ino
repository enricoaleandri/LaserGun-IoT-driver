String configFileName = "/config.json";

class configManager {

  public :

   static JsonObject& getConfig(String fileName){

     DynamicJsonBuffer jsonBuffer;
     char emptyJson[] = "}";
     if(fileName == ""){
        fileName = configFileName;
     }
     if (SPIFFS.begin()) {
        SerialComunication::info("WifiManaged", "WifiManagedInit", "mounted file system");
        if (SPIFFS.exists(fileName)) {
          //file exists, reading and loading
          SerialComunication::info("WifiManaged", "WifiManagedInit", "reading config file");
          File configFile = SPIFFS.open(fileName, "r");
          if (configFile) {
            SerialComunication::info("WifiManaged", "WifiManagedInit", "opened config file");
            size_t size = configFile.size();
            // Allocate a buffer to store contents of the file.
            std::unique_ptr<char[]> buf(new char[size]);
    
            configFile.readBytes(buf.get(), size);
            
            JsonObject& json = jsonBuffer.parseObject(buf.get());
            json.printTo(Serial);
            if (json.success()) {
              SerialComunication::info("WifiManaged", "WifiManagedInit", "\nparsed json");    
              
              return json;
            } else {
              SerialComunication::info("WifiManaged", "WifiManagedInit", "failed to load json config");
              return jsonBuffer.parseObject(emptyJson);
            }
          }else{
           
              SerialComunication::info("WifiManaged", "WifiManagedInit", "No Data in the file");  
              return jsonBuffer.parseObject(emptyJson);
          }
        } else {
          SerialComunication::info("WifiManaged", "WifiManagedInit", "File doesnt exist");  
          return jsonBuffer.parseObject(emptyJson);
        }
      } else {
        SerialComunication::info("WifiManaged", "WifiManagedInit", "failed to mount FS");
        return jsonBuffer.parseObject(emptyJson);
      }
  }
 
};

