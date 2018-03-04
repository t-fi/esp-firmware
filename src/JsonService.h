#ifndef JSON_H
#define JSON_H

#include <ArduinoJson.h>
#include <string>

#include "FileSystemService.h"

class JsonService {
public:
    JsonService(FileSystemService& fileSystemService) : fileSystemService(fileSystemService) {}

    FileSystemService& fileSystemService;
    JsonObject& parse(std::string jsonString);
    JsonObject& parseFile(std::string filePath);
    void save(std::string path, JsonObject& json);
    static std::string getString(JsonObject& json);
};

#endif // JSON_H
