#ifndef SERVERUTIL_H
#define SERVERUTIL_H

#include <Arduino.h>
#include <ESP8266WiFi.h>

class ServerUtil {
public:
    static String receive(WiFiServer& server);
};

#endif // SERVERUTIL_H

