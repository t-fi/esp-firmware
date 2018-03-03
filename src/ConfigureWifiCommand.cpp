#include "ConfigureWifiCommand.h"

ConfigureWifiCommand::ConfigureWifiCommand(std::string ssid, std::string password)
{
    this->ssid = ssid;
    this->password = password;
}
