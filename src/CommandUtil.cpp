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
    json["ssid"] = command.ssid;
    json["password"] = command.password;
    JsonUtil::save("/wifiCredentials.json", json);
    this->wifi.updateCredentials(command.ssid, command.password);
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
        Serial.println(json["toggle"]["id"].as<int>());
        //toggle(json["toggle"]["componentId"].as<int>());
    }
    else if (json.containsKey("setColor")) {
        word pwms[16];
        for (int i = 0; i < 16; ++i) {
            switch (i % 4) {
                case 0:
                    pwms[i] = json["setColor"]["red"].as<int>();
                    break;
                case 1:
                    pwms[i] = json["setColor"]["green"].as<int>();
                    break;
                case 2:
                    pwms[i] = json["setColor"]["blue"].as<int>();
                    break;
                case 3:
                    pwms[i] = json["setColor"]["warmWhite"].as<int>();
                    break;
            }
        }

    }
}
