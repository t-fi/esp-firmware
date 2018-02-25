#ifndef WIFI_H
#define WIFI_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>

class WifiUtil {
public:
    String getSsid();
    String getPassword();

    void setup();
    void getCredentials();
    void connect();
    void setupAccessPoint();
    void updateCredentials(String newSsid, String newPassword);

private:
    String ssid;
    String password;
    void printStatus();
};

#endif // WIFI_H

