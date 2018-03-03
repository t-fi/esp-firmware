#ifndef JSON_H
#define JSON_H

#include <ArduinoJson.h>
#include <string>

class JsonUtil {
public:
    static JsonObject& parse(std::string jsonString);
    static JsonObject& parseFile(std::string filePath);
    static void save(std::string path, JsonObject& json);
    static std::string getString(JsonObject& json);
private:
    JsonUtil() {}
};

#endif // JSON_H
