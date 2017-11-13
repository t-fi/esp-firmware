#ifndef COMMANDUTIL_H
#define COMMANDUTIL_H

#include <Arduino.h>
#include "WifiUtil.h"
#include "FlashCommand.h"
#include "ConfigureWifiCommand.h"
#include "SetColorCommand.h"

class CommandUtil {
public:
    CommandUtil(WifiUtil wifi);
    void parse(JsonObject& json);
private:
    WifiUtil* wifi;
    void handle(FlashCommand command);
    void handle(ConfigureWifiCommand command);
    void handle(SetColorCommand command);
};

#endif // COMMANDUTIL_H
