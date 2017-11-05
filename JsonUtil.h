#ifndef JSON_H
#define JSON_H

#include <ArduinoJson.h>

JsonObject& parseJson(String jsonString);
JsonObject& getFileAsJson(String filePath);

#endif // JSON_H


