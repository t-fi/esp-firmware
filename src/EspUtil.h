#ifndef ESPUTIL_H
#define ESPUTIL_H

#include <ArduinoJson.h>
#include <string>

class EspUtil {
public:
    static void updateEsp(std::string path);
    static void restart();
    static void updateConfig(JsonObject& json);
    static int getRestartCount();
    static void setRestartCount(int restartCount);
    bool isConnected(int componentId);
};

#endif // ESPUTIL_H
