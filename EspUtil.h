#ifndef ESPUTIL_H
#define ESPUTIL_H

#include <Arduino.h>
#include <ArduinoJson.h>

class EspUtil {
public:
    static void updateEsp(String path);
    static void restart();
    static void updateConfig(JsonObject& json);
    bool isConnected(int componentId);
};

#endif // ESPUTIL_H

