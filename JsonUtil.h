#ifndef JSON_H
#define JSON_H

#include <ArduinoJson.h>

class JsonUtil {
public:
    static JsonObject& parse(String jsonString);
    static JsonObject& parseFile(String filePath);
    static void save(String path, JsonObject& json);

private:
    JsonUtil() {}
};

#endif // JSON_H

