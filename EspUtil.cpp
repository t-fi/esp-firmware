#include <ESP8266httpUpdate.h>
#include <FS.h>

#include "EspUtil.h"
#include "JsonUtil.h"

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
    File f = SPIFFS.open("/config.json", "w");
    if (f) {
        // buffer of 4096 crashes esp01
        char buffer[2048];
        json.printTo(buffer, sizeof(buffer));
        f.print(buffer);
        f.close();
        Serial.println("Successfully updated config.");
    }
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
