#ifndef SERVERUTIL_H
#define SERVERUTIL_H

#include <Arduino.h>
#include <ESP8266WiFi.h>

struct Message {
    Message(uint8_t* payload, int length) {
        this->payload = payload;
        this->length = length;
    }
    ~Message() {
        delete[] payload;
    }

    uint8_t* payload;
    int length;
};

class ServerUtil {
public:
    static Message* receive(WiFiServer& server);
};

#endif // SERVERUTIL_H
