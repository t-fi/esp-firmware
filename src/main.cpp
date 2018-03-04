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
EspService espService(fileSystemService, jsonService);
WifiService wifiService(logService, jsonService, espService);
CommandService command(wifiService, jsonService, espService);
WiFiServer server(420);

Thread logDaemon;
StaticThreadController<1> threads(&logDaemon);

void logDaemonWrapper() {
    logService.daemon();
}

void setup()
{
    Serial.begin(9600);
    delay(100);
    SPIFFS.begin();
    logService.setWifi(&wifiService);
    wifiService.setup();
    server.begin();

    logDaemon.setInterval(100);
    logDaemon.onRun(logDaemonWrapper);
}

void handleEcho(String message)
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
        // for (size_t i = 0; i < message->length; ++i) {
        //     Serial.printf("%x ", message->payload[i]);
        // }
        // Serial.printf("\n");
        // LedDriverRev1 driver;
        // driver.parseColors(message->payload, message->length);
        delete message;
    }
    // Serial.println(message->length);

    // handleEcho(message);
    //
    // JsonObject& json = JsonUtil::parse(message);
    // if (json.containsKey("command")) {
    //     command.parse(json["command"]);
    // }

    delay(500);
}
