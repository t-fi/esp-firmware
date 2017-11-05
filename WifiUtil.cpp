#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <FS.h>

#include "WifiUtil.h"
#include "JsonUtil.h"
#include "EspUtil.h"

String WifiUtil::getSsid()
{
}

String WifiUtil::getPassword()
{
}

void WifiUtil::setup()
{
    getCredentials();
    connect();
}

void WifiUtil::getCredentials()
{
    JsonObject& json = JsonUtil::parseFile("/wifiCredentials.json");
    String ssidString = json["ssid"];
    String passwordString = json["password"];
    ssid = ssidString;
    password = passwordString;
}

void WifiUtil::connect()
{
    char ssidArr[128];
    char passwordArr[128];
    ssid.toCharArray(ssidArr, 128);
    password.toCharArray(passwordArr, 128);

    Serial.printf("Connecting to %s ", ssidArr);
    WiFi.begin(ssidArr, passwordArr);
    int c = 0;
    while (WiFi.status() != WL_CONNECTED && c < 40) {
        delay(500);
        c++;
        Serial.print(".");

        if (c == 40) {
            setupAccessPoint();
        }
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("");
        Serial.println("WiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
    }
}

void WifiUtil::setupAccessPoint()
{
    WiFi.mode(WIFI_AP);
    Serial.print("Setting soft-AP ... ");

    if (WiFi.softAP("ESPsoftAP_01", "12345678")) {
        Serial.println("Ready");
    }
    else {
        Serial.println("Failed! Restarting Esp ...");
        EspUtil::restart();
    }
}

void WifiUtil::updateCredentials(String newSsid, String newPassword)
{
    ssid = newSsid;
    password = newPassword;
}
