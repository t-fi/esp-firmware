#include <ESP8266httpUpdate.h>
#include <FS.h>

#include "EspService.h"

void EspService::updateEsp(std::string path)
{
    Serial.println(path.c_str());
    if (path != "") {
        for (int i = 0; i < 10; ++i) {
            // Serial.println("Fetching firmware...");
            int result = ESPhttpUpdate.update(path.c_str());
            // Serial.println(result);
            // Serial.println(ESPhttpUpdate.getLastError());

            if (result == 2) {
                delay(100);
                ESP.restart();
            }
        }
    }
}

void EspService::restart()
{
    delay(100);
    ESP.restart();
}

void EspService::updateConfig(JsonObject& json)
{
    this->jsonService.save("/config.json", json);
}

bool EspService::isConnected(int componentId)
{
    bool isConnected = false;
    JsonObject& config = this->jsonService.parseFile("/config.json");
    JsonArray& components = config["esp"]["components"];

    for (auto component : components) {
        if (componentId == component["componentId"].as<int>()) {
            isConnected = true;
        }
    }

    return isConnected;
}

int EspService::getRestartCount()
{
    std::string restartCount = this->fileSystemService.read("/restartCount");
    return atoi(restartCount.c_str());
}

void EspService::setRestartCount(int restartCount)
{
    char restartCountChar[1];
    itoa(restartCount, restartCountChar, 10);
    std::string restartCountString(restartCountChar);
    this->fileSystemService.write("/restartCount", restartCountString);
}
