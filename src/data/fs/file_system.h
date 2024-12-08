class FileSystem
{
    public:
        static bool spiffsAvailable();
        static bool writeToFile(const char* value);
        static String readFromFile();
};