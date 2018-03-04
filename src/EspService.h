#ifndef ESPUTIL_H
#define ESPUTIL_H

#include <ArduinoJson.h>
#include <string>

#include "JsonService.h"
#include "FileSystemService.h"
#include "ConfigurationService.h"

class EspService {
public:
    EspService(FileSystemService& fileSystemService, JsonService& jsonService,
    ConfigurationService& configurationService)
    : fileSystemService(fileSystemService), jsonService(jsonService),
    configurationService(configurationService) {}
    void updateEsp(const std::string path);
    void restart();
    void updateConfig(const JsonObject& json);
    const int getRestartCount();
    void setRestartCount(const int restartCount);
    bool isConnected(const int componentId);

    const int getId() { return this->id;  }
private:
    FileSystemService& fileSystemService;
    JsonService& jsonService;
    ConfigurationService& configurationService;

    const int id = this->configurationService.getId();
};

#endif // ESPUTIL_H
