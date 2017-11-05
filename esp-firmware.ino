#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <ESP8266WiFi.h>
#include <FS.h>

#include "Enums.h"
#include "WifiUtil.h"
#include "JsonUtil.h"
#include "FileSystemUtil.h"
#include "EspUtil.h"
#include "ServerUtil.h"
#include "CommandUtil.h"

WifiUtil wifi;
CommandUtil command(wifi);
WiFiServer server(420);

void setup()
{
    Serial.begin(9600);
    delay(100);
    SPIFFS.begin();
    wifi.setup();
    server.begin();
}

void handleEcho(String message)
{
    if (message == "new") {
        Serial.println("4Real! 00ld firmware!");
    }
    if (message.substring(0, 6) == "update") {
        Serial.println("Updating..");
        Serial.println(message.substring(6));
        String path = "";
        path += message.substring(6);
        //path = "http://192.168.0.119:80/arduino.bin";
        Serial.println(path);
        int result = ESPhttpUpdate.update(path);
        Serial.println(result);
        Serial.println(ESPhttpUpdate.getLastError());
    }
    if (message == "reset") {
        EspUtil::restart();
    }
    if (message == "format") {
        FileSystemUtil::format();
    }
    if (message == "read") {
        Serial.print("ssid: ");
        Serial.println(wifi.getSsid());
        Serial.print("key: ");
        Serial.println(wifi.getPassword());
    }
    if (message == "readConfig") {
        String config = FileSystemUtil::read("/config.json");
        Serial.println(config);
    }
}

void loop()
{
    String message = ServerUtil::receive(server);
    handleEcho(message);

    JsonObject& json = JsonUtil::parse(message);
    if (json.containsKey("command")) {
        command.parse(json["command"]);
    }
}
