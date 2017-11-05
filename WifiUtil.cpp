#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <FS.h>

#include "WifiUtil.h"
#include "JsonUtil.h"
#include "EspUtil.h"

void setupWifi() {
    WifiCredentials* credentials = getWifiCredentials();
    connectToWifi(credentials);
    free(credentials);
}

WifiCredentials* getWifiCredentials() {
    WifiCredentials* credentials = (WifiCredentials*) malloc(sizeof(WifiCredentials));

    JsonObject &json = getFileAsJson("/wifiCredentials.json");
    String ssid = json["ssid"];
    String password = json["password"];

    char* ssid_char = (char*) malloc(32 * sizeof(char));
    char* password_char = (char*) malloc(64 * sizeof(char));

    ssid.toCharArray(ssid_char, 32);
    password.toCharArray(password_char, 64);

    credentials->ssid = ssid_char;
    credentials->password = password_char;

    return credentials;
}

void connectToWifi(WifiCredentials* credentials) {
    Serial.printf("Connecting to %s ", credentials->ssid);
    WiFi.begin(credentials->ssid, credentials->password);
    int c = 0;
    while (WiFi.status() != WL_CONNECTED && c < 40) {
        delay(500);
        c++;
        Serial.print(".");

        if (c == 40){
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

void setupAccessPoint() {
    WiFi.mode(WIFI_AP);
    Serial.print("Setting soft-AP ... ");

    if (WiFi.softAP("ESPsoftAP_01", "12345678")) {
        Serial.println("Ready");
    } else {
        Serial.println("Failed! Restarting Esp ...");
        restart();
    }
}

void configureWifi(JsonObject& json) {
    File f = SPIFFS.open("/wifiCredentials.json", "w");
    if (f) {
        char buffer[256];
        json.printTo(buffer, sizeof(buffer));
        f.print(buffer);
        f.close();
        Serial.println("Successfully updated Wifi credentials.");
    }
}
