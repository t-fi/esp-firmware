#ifndef COMMANDUTIL_H
#define COMMANDUTIL_H

#include <ArduinoJson.h>
#include "WifiService.h"
#include "FlashCommand.h"
#include "ConfigureWifiCommand.h"
#include "SetColorCommand.h"
#include "JsonService.h"

class CommandService {
public:
    CommandService(WifiService& wifiService, JsonService& jsonService,
        EspService& espService)
        : wifiService(wifiService), jsonService(jsonService), espService(espService) {}
    void parse(JsonObject& json);
private:
    WifiService& wifiService;
    JsonService& jsonService;
    EspService& espService;
    void handle(FlashCommand command);
    void handle(ConfigureWifiCommand command);
    void handle(SetColorCommand command);
};

#endif // COMMANDUTIL_H
