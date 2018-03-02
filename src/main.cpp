#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <ESP8266WiFi.h>
#include <FS.h>
#include <Wire.h>

#include "Enums.h"
#include "WifiUtil.h"
#include "JsonUtil.h"
#include "FileSystemUtil.h"
#include "EspUtil.h"
#include "ServerUtil.h"
#include "CommandUtil.h"
#include "LedDriverRev1.h"

WifiUtil wifi;
CommandUtil command(wifi);
WiFiServer server(420);

void setup()
{
    // Serial.begin(9600);
    delay(100);
    SPIFFS.begin();
    wifi.setup();
    server.begin();
    // Wire.begin(2, 7);
    // Wire.setClock(400000);
}

void handleEcho(String message)
{
    if (message == "new")  {
        Serial.println("thisistherealshit!");
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
    Message* message = ServerUtil::receive(server);
    if (message) {
        // for (size_t i = 0; i < message->length; ++i) {
        //     Serial.printf("%x ", message->payload[i]);
        // }
        // Serial.printf("\n");
        LedDriverRev1 driver;
        driver.parseColors(message->payload, message->length);
    }
    // Serial.println(message->length);

    // handleEcho(message);
    //
    // JsonObject& json = JsonUtil::parse(message);
    // if (json.containsKey("command")) {
    //     command.parse(json["command"]);
    // }
    delete message;
}
