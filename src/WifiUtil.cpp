#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <FS.h>

#include "WifiUtil.h"
#include "JsonUtil.h"
#include "EspUtil.h"

std::string WifiUtil::getSsid()
{
    return ssid;
}

std::string WifiUtil::getPassword()
{
    return password;
}

void WifiUtil::setup()
{
    getCredentials();
    connect();
}

void WifiUtil::getCredentials()
{
    JsonObject& json = JsonUtil::parseFile("/wifiCredentials.json");
    std::string ssidString(json["ssid"].as<char*>());
    std::string passwordString(json["password"].as<char*>());
    ssid = ssidString;
    password = passwordString;
}

void WifiUtil::connect()
{
    // Serial.printf("Connecting to \"%s\" ", ssidArr);
    // Serial.printf("using password: \"%s\"\n", passwordArr);
    WiFi.begin(getSsid().c_str(), getPassword().c_str());
    int c = 0;
    while (WiFi.status() != WL_CONNECTED && c < 40) {
        delay(500);
        c++;

        if (c == 40) {
            if (EspUtil::getRestartCount() == 5) {
                setupAccessPoint();
            } else {
                EspUtil::setRestartCount(EspUtil::getRestartCount() + 1);
                EspUtil::restart();
            }
        }
    }

    if (WiFi.status() == WL_CONNECTED) {
        printStatus();
    } else {
        EspUtil::setRestartCount(0);
    }
}

void WifiUtil::setupAccessPoint()
{
    WiFi.mode(WIFI_AP);
    // Serial.print("Setting up soft-AP ... ");

    if (WiFi.softAP("ESPsoftAP_01", "12345678")) {
        // Serial.println("Done.");
    }
    else {
        // Serial.println("Failed! Restarting Esp ...");
        EspUtil::restart();
    }
}

void WifiUtil::updateCredentials(std::string newSsid, std::string newPassword)
{
    ssid = newSsid;
    password = newPassword;
}

void WifiUtil::printStatus()
{
    // Serial.println("");
    // Serial.println("WiFi connected");
    // Serial.println("IP address: ");
    // Serial.println(WiFi.localIP());
}
