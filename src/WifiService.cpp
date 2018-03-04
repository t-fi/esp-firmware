#include <ESP8266HTTPClient.h>
#include <Arduino.h>

#include "WifiService.h"
#include "JsonService.h"
#include "EspService.h"
#include "log/LogEntryWifiConnect.h"
#include "log/LogEntryWifiConnected.h"

std::string WifiService::getSsid()
{
    return this->ssid;
}

std::string WifiService::getPassword()
{
    return this->password;
}

void WifiService::setup()
{
    this->getCredentials();
    this->connect();
}

void WifiService::getCredentials()
{
    const JsonObject& json = this->jsonService.parseFile("/wifiCredentials.json");
    std::string ssidString(json["ssid"].as<char*>());
    std::string passwordString(json["password"].as<char*>());
    this->ssid = ssidString;
    this->password = passwordString;
}

void WifiService::connect()
{
    LogEntryWifiConnect* logEntry = new LogEntryWifiConnect(this->getSsid(), this->getPassword());
    this->logService.log(logEntry);
    WiFi.begin(getSsid().c_str(), getPassword().c_str());
    int c = 0;
    while (WiFi.status() != WL_CONNECTED && c < 40) {
        delay(500);
        c++;

        if (c == 40) {
            if (this->espService.getRestartCount() == 5) {
                setupAccessPoint();
            } else {
                this->espService.setRestartCount(this->espService.getRestartCount() + 1);
                this->espService.restart();
            }
        }
    }

    if (WiFi.status() == WL_CONNECTED) {
        LogEntryWifiConnected* logEntry = new LogEntryWifiConnected(this->getSsid(), WiFi.localIP());
        this->logService.log(logEntry);
    }

    this->espService.setRestartCount(0);
}

void WifiService::setupAccessPoint()
{
    WiFi.mode(WIFI_AP);
    // Serial.print("Setting up soft-AP ... ");

    if (WiFi.softAP("ESPsoftAP_01", "12345678")) {
        // Serial.println("Done.");
    }
    else {
        // Serial.println("Failed! Restarting Esp ...");
        this->espService.restart();
    }
}

void WifiService::updateCredentials(std::string newSsid, std::string newPassword)
{
    ssid = newSsid;
    password = newPassword;
}
