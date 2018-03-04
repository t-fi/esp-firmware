#ifndef JSON_H
#define JSON_H

#include <ArduinoJson.h>
#include <string>

#include "FileSystemService.h"

class JsonService {
public:
    JsonService(FileSystemService& fileSystemService)
    : fileSystemService(fileSystemService) {}

    FileSystemService& fileSystemService;
    JsonObject& parse(const std::string jsonString);
    JsonObject& parseFile(const std::string filePath);
    void save(const std::string path, const JsonObject& json);
    static std::string getString(const JsonObject& json);
};

#endif // JSON_H
