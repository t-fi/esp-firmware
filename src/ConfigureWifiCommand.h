#ifndef CONFIGUREWIFICOMMAND_H
#define CONFIGUREWIFICOMMAND_H

#include <string>

class ConfigureWifiCommand {
public:
    ConfigureWifiCommand(std::string ssid, std::string password);
    std::string ssid;
    std::string password;
};

#endif // CONFIGUREWIFICOMMAND_H
