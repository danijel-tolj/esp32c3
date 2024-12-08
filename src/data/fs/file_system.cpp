#include <SPIFFS.h>
#include "file_system.h"

bool FileSystem::spiffsAvailable() {
    if (!SPIFFS.begin(true)) {
      Serial.println("An error has occurred while mounting SPIFFS");
       return false;
    }
    return true;
}

bool FileSystem::writeToFile(const char* text){
    File file = SPIFFS.open("/data.txt", FILE_APPEND);
    if (file) {
        file.println(text);
        file.close();
        return true;
    }
    return false;
}

String FileSystem::readFromFile(){
    File file = SPIFFS.open("/data.txt", FILE_READ);
    if (file) {
        String text = ""; 

        while (file.available()) {
          text.concat(file.readString());
        }
        file.close();

        return text;
    }
    return "Error";
}
