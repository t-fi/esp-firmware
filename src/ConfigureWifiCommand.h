#ifndef CONFIGUREWIFICOMMAND_H
#define CONFIGUREWIFICOMMAND_H

#include <Arduino.h>

class ConfigureWifiCommand {
public:
    ConfigureWifiCommand(String ssid, String password);
    String ssid;
    String password;
};

#endif // CONFIGUREWIFICOMMAND_H
