#ifndef ESPUTIL_H
#define ESPUTIL_H

#include <ArduinoJson.h>
#include <string>

#include "JsonService.h"
#include "FileSystemService.h"

class EspService {
public:
    EspService(FileSystemService& fileSystemService, JsonService& jsonService)
    : fileSystemService(fileSystemService), jsonService(jsonService) {}
    void updateEsp(std::string path);
    void restart();
    void updateConfig(JsonObject& json);
    int getRestartCount();
    void setRestartCount(int restartCount);
    bool isConnected(int componentId);
private:
    FileSystemService& fileSystemService;
    JsonService& jsonService;
};

#endif // ESPUTIL_H
