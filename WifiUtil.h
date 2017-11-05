#ifndef WIFI_H
#define WIFI_H

#include <ArduinoJson.h>
#include <ESP8266WiFi.h>

typedef struct WifiCredentials {
    char* ssid;
    char* password;
} WifiCredentials;

void setupWifi();
WifiCredentials* getWifiCredentials();
void connectToWifi(WifiCredentials* credentials);
void setupAccessPoint();
void configureWifi(JsonObject& json);

#endif // WIFI_H


