#include <SPIFFS.h>
#include "file_system.h"

const String filePath = "/data.txt";

bool FileSystem::spiffsAvailable()
{
    if (!SPIFFS.begin(true))
    {
        Serial.println("An error has occurred while mounting SPIFFS");
        return false;
    }
    return true;
}

bool FileSystem::writeToFile(uint8_t *text, size_t length)
{
    File file = SPIFFS.open(filePath, FILE_APPEND);
    if (file)
    {
        file.write(text, length);
        file.close();
        return true;
    }
    return false;
}

String FileSystem::readFromFile()
{
    File file = SPIFFS.open(filePath, FILE_READ);
    if (file)
    {
        String text = "";

        Serial.println("Binary data in hex:");
        while (file.available())
        {
            uint8_t byte = file.read(); // Read each byte

            text.concat(byte);
        }

        return text;
    }
    return "Error";
}

void FileSystem::cleanFile()
{
    File file = SPIFFS.open(filePath, FILE_WRITE);
    if (file)
    {
        file.close();
    }
}
