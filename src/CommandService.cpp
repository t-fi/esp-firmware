#include <string>

#include "CommandService.h"
#include "EspService.h"

const CommandType CommandService::getType(const char type)
{
    CommandType commandType;

    switch (type) {
        case 0:
            commandType = CommandType::SetColor;
            break;
        case 123:
            commandType = CommandType::Json;
            break;
        default:
            commandType = CommandType::Unknown;
    }

    return commandType;
}

void CommandService::handle(const FlashCommand command)
{
    this->espService.updateEsp(command.url);
}

void CommandService::handle(const ConfigureWifiCommand command)
{
    DynamicJsonBuffer buffer;
    JsonObject& json = buffer.createObject();
    json["ssid"] = command.ssid.c_str();
    json["password"] = command.password.c_str();
    this->jsonService.save("/wifiCredentials.json", json);
    this->wifiService.updateCredentials(command.ssid, command.password);
}

void CommandService::parse(const JsonObject& json)
{
    if (json.containsKey("flash")) {
        std::string url(json["flash"]["url"].as<char*>());
        FlashCommand command(url);
        handle(command);
    }
    else if (json.containsKey("configureEsp")) {
        this->espService.updateConfig(json["configureEsp"]);
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
