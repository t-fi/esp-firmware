#include <ESP8266httpUpdate.h>
#include <FS.h>

#include "EspUtil.h"
#include "JsonUtil.h"
#include "FileSystemUtil.h"

void EspUtil::updateEsp(String path)
{
    Serial.println(path);
    if (path != "") {
        for (int i = 0; i < 10; ++i) {
            Serial.println("Fetching firmware...");
            int result = ESPhttpUpdate.update(path);
            Serial.println(result);
            Serial.println(ESPhttpUpdate.getLastError());

            if (result == 2) {
                delay(100);
                ESP.restart();
            }
        }
    }
}

void EspUtil::restart()
{
    delay(100);
    ESP.restart();
}

void EspUtil::updateConfig(JsonObject& json)
{
    JsonUtil::save("/config.json", json);
}

bool EspUtil::isConnected(int componentId)
{
    bool isConnected = false;
    JsonObject& config = JsonUtil::parseFile("/config.json");
    JsonArray& components = config["esp"]["components"];

    for (auto component : components) {
        if (componentId == component["componentId"].as<int>()) {
            isConnected = true;
        }
    }

    return isConnected;
}

int EspUtil::getRestartCount()
{
    std::string restartCount = FileSystemUtil::read("/restartCount");
    return atoi(restartCount.c_str());
}

void EspUtil::setRestartCount(int restartCount)
{
    char restartCountChar[1];
    itoa(restartCount, restartCountChar, 10);
    std::string restartCountString(restartCountChar);
    FileSystemUtil::write("/restartCount", restartCountString);
}
