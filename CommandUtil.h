#ifndef COMMANDUTIL_H
#define COMMANDUTIL_H

#include <Arduino.h>

#include "WifiUtil.h"

class SetColorCommand {
  
};

class FlashCommand {
public:
    FlashCommand(String url);
    String url;
};

class ConfigureWifiCommand {
public:
    ConfigureWifiCommand(String ssid, String password);
    String ssid;
    String password;
};

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

