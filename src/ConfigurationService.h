#ifndef CONFIGURATIONSERVICE_H
#define CONFIGURATIONSERVICE_H

#include <ArduinoJson.h>
#include <string>

#include "JsonService.h"
#include "FileSystemService.h"
#include "ConfigurationService.h"

class ConfigurationService {
public:
    ConfigurationService(JsonService& jsonService)
    : jsonService(jsonService) {}
    const int getId();
private:
    JsonService& jsonService;
};

#endif // CONFIGURATIONSERVICE_H
