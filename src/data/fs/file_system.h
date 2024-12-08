class FileSystem
{
public:
    static bool spiffsAvailable();
    static bool writeToFile(uint8_t *value, size_t length);
    static String readFromFile();
    static void cleanFile();
};