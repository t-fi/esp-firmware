#include "ConfigureWifiCommand.h"

ConfigureWifiCommand::ConfigureWifiCommand(String ssid, String password)
{
    this->ssid = ssid;
    this->password = password;
}
