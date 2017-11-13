#include "CommandUtil.h"
#include "JsonUtil.h"
#include "EspUtil.h"
#include "WifiUtil.h"

CommandUtil::CommandUtil(WifiUtil wifi) {
    this->wifi = &wifi;
}

void CommandUtil::handle(FlashCommand command)
{
    if (command.url == "http://192.168.0.119:80/arduino.bong") {
        EspUtil::updateEsp(command.url);
    }
}

void CommandUtil::handle(ConfigureWifiCommand command)
{
    wifi->updateCredentials(command.ssid, command.password);
    DynamicJsonBuffer buffer;
    JsonObject& json = buffer.createObject();
    json["ssid"] = command.ssid;
    json["password"] = command.password;
    JsonUtil::save("/wifiCredentials.json", json);
}

void CommandUtil::handle(SetColorCommand command)
{
    //int componentId = json["componentId"].as<int>();
    //if (json.containsKey("fade")) {
        // Call setFade() method of led class
    //}
    //else {
        // Call setColor() method of led class
    //}
}

void CommandUtil::parse(JsonObject& json)
{
    if (json.containsKey("flash")) {
        String url = json["flash"]["url"];
        FlashCommand command(url);
        handle(command);
    }
    else if (json.containsKey("configureEsp")) {
        EspUtil::updateConfig(json["configureEsp"]);
    }
    else if (json.containsKey("configureWifi")) {
        String ssid = json["configureWifi"]["ssid"];
        String password = json["configureWifi"]["password"];
        ConfigureWifiCommand command(ssid, password);
        handle(command);
    }
    else if (json.containsKey("toggle")) {
        //toggle(json["toggle"]["componentId"].as<int>());
    }
    else if (json.containsKey("setColor")) {
        //parseSetColorCommand(json["setColor"]);
    }
}
