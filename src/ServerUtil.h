#ifndef SERVERUTIL_H
#define SERVERUTIL_H

#include <ESP8266WiFi.h>

struct Message {
    Message(char* payload, int length) {
        this->payload = payload;
        this->length = length;
    }
    ~Message() {
        delete[] payload;
    }

    char* payload;
    int length;
};

class ServerUtil {
public:
    static Message* receive(WiFiServer& server);
};

#endif // SERVERUTIL_H
