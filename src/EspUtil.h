#ifndef ESPUTIL_H
#define ESPUTIL_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <string>

class EspUtil {
public:
    static void updateEsp(String path);
    static void restart();
    static void updateConfig(JsonObject& json);
    static int getRestartCount();
    static void setRestartCount(int restartCount);

    bool isConnected(int componentId);
};

#endif // ESPUTIL_H
