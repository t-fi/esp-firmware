#ifndef WIFI_H
#define WIFI_H

#include <ESP8266httpUpdate.h>
#include <string>

#include "log/LogService.h"
#include "JsonService.h"
#include "EspService.h"

class WifiService {
public:
    WifiService(LogService& logService, JsonService& jsonService,
        EspService& espService)
    : logService(logService), jsonService(jsonService),
        espService(espService) {}
    std::string getSsid();
    std::string getPassword();

    void setup();
    void getCredentials();
    void connect();
    void setupAccessPoint();
    void updateCredentials(std::string newSsid, std::string newPassword);
    bool isConnected() { return WiFi.status() == WL_CONNECTED; }
private:
    LogService& logService;
    JsonService& jsonService;
    EspService& espService;
    std::string ssid;
    std::string password;

    void printStatus();
};

#endif // WIFI_H
