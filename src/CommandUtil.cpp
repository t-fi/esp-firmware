#include <string>

#include "CommandUtil.h"
#include "JsonUtil.h"
#include "EspUtil.h"
#include "WifiUtil.h"

CommandUtil::CommandUtil(WifiUtil& wifi) {
    this->wifi = wifi;
}

void CommandUtil::handle(FlashCommand command)
{
    EspUtil::updateEsp(command.url);
}

void CommandUtil::handle(ConfigureWifiCommand command)
{
    DynamicJsonBuffer buffer;
    JsonObject& json = buffer.createObject();
    json["ssid"] = command.ssid.c_str();
    json["password"] = command.password.c_str();
    JsonUtil::save("/wifiCredentials.json", json);
    this->wifi.updateCredentials(command.ssid, command.password);
}

void CommandUtil::parse(JsonObject& json)
{
    if (json.containsKey("flash")) {
        std::string url(json["flash"]["url"].as<char*>());
        FlashCommand command(url);
        handle(command);
    }
    else if (json.containsKey("configureEsp")) {
        EspUtil::updateConfig(json["configureEsp"]);
    }
    else if (json.containsKey("configureWifi")) {
        std::string ssid(json["configureWifi"]["ssid"].as<char*>());
        std::string password(json["configureWifi"]["password"].as<char*>());
        ConfigureWifiCommand command(ssid, password);
        handle(command);
    }
    else if (json.containsKey("toggle")) {
        // Serial.println(json["toggle"]["id"].as<int>());
        //toggle(json["toggle"]["componentId"].as<int>());
    }
}
