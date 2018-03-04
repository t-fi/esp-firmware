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
    void parse(const JsonObject& json);
    const CommandType getType(const char type);
private:
    WifiService& wifiService;
    JsonService& jsonService;
    EspService& espService;
    void handle(const FlashCommand command);
    void handle(const ConfigureWifiCommand command);
    void handle(const SetColorCommand command);
};

#endif // COMMANDUTIL_H
