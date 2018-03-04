#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <ESP8266WiFi.h>
#include <FS.h>
#include <Thread.h>
#include <StaticThreadController.h>

#include "log/LogService.h"
#include "Enums.h"
#include "WifiService.h"
#include "JsonService.h"
#include "FileSystemService.h"
#include "EspService.h"
#include "ServerUtil.h"
#include "CommandService.h"
#include "LedDriverRev1.h"

LogService logService;
FileSystemService fileSystemService(logService);
JsonService jsonService(fileSystemService);
ConfigurationService configurationService(jsonService);
EspService espService(fileSystemService, jsonService, configurationService);
WifiService wifiService(logService, jsonService, espService);
CommandService commandService(wifiService, jsonService, espService);
WiFiServer server(420);
LedDriverRev1 driver;

Thread logDaemon;
Thread fadeDaemon;
StaticThreadController<2> threads(&logDaemon, &fadeDaemon);

void logDaemonWrapper() {
    logService.daemon();
}

void fadeDaemonWrapper() {
    // driver.fade();
}

void setup()
{
    Serial.begin(9600);
    delay(100);
    SPIFFS.begin();
    wifiService.setup();
    logService.setWifiService(&wifiService);
    logService.setEspService(&espService);
    server.begin();

    logDaemon.setInterval(100);
    logDaemon.onRun(logDaemonWrapper);
    fadeDaemon.enabled = false;
    fadeDaemon.setInterval(0);
    fadeDaemon.onRun(fadeDaemonWrapper);
}

void handleEcho(std::string message)
{
    if (message == "new")  {
        Serial.println("thisistherealshit!");
    }
    if (message == "reset") {
        espService.restart();
    }
    if (message == "format") {
        fileSystemService.format();
    }
    if (message == "read") {
        Serial.print("ssid: ");
        Serial.println(wifiService.getSsid().c_str());
        Serial.print("key: ");
        Serial.println(wifiService.getPassword().c_str());
    }
    if (message == "readConfig") {
        std::string config = fileSystemService.read("/config.json");
        Serial.println(config.c_str());
    }
}

void loop()
{
    threads.run();
    Message* message = ServerUtil::receive(server);
    if (message) {
        CommandType type = commandService.getType(message->payload[0]);
        switch(type) {
            case CommandType::SetColor:
                fadeDaemon.enabled = false;
                driver.parseColors((uint8_t*)message->payload, message->length);
                break;
            case CommandType::SetColorFade:
                fadeDaemon.enabled = true;
                break;
            case CommandType::Json:
            {
                JsonObject& json = jsonService.parse(std::string(message->payload));
                if (json.containsKey("command"))
                    commandService.parse(json["command"]);
                break;
            }
            default:
                handleEcho(std::string(message->payload));
                break;
        }

        delete message;
    }
}
