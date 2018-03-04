#ifndef COMMANDUTIL_H
#define COMMANDUTIL_H

#include <ArduinoJson.h>
#include "WifiService.h"
#include "FlashCommand.h"
#include "ConfigureWifiCommand.h"
#include "SetColorCommand.h"
#include "JsonService.h"

enum class CommandType {
    SetColor = 1,
    SetColorFade = 2,
    Json = 123
};

class CommandService {
public:
    CommandService(WifiService& wifiService, JsonService& jsonService,
        EspService& espService)
        : wifiService(wifiService), jsonService(jsonService),
        espService(espService) {}
    void parse(JsonObject& json);
    CommandType getType(char type);
private:
    WifiService& wifiService;
    JsonService& jsonService;
    EspService& espService;
    void handle(FlashCommand command);
    void handle(ConfigureWifiCommand command);
    void handle(SetColorCommand command);
};

#endif // COMMANDUTIL_H
