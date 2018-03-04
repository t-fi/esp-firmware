#include "ConfigurationService.h"

const int ConfigurationService::getId()
{
    JsonObject& json = this->jsonService.parseFile("/config.json");
    // return json["esp"]["id"].as<int>();
    return 1;
}
