#ifndef LOGENTRY_H
#define LOGENTRY_H

#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <string>

enum class LogEntryType {
    Info = 1,
    Warning = 2,
    Error = 3
};

class LogEntry {
protected:
    std::string message;
public:
    void send() {
        HTTPClient client;
        client.begin("http://192.168.1.253/esp-web-app/front/");
        client.addHeader("Content-Type", "application/x-www-form-urlencoded");
        client.POST(this->message.c_str());
        Serial.println(client.getString());
        client.end();
    }
};

#endif // LOGENTRY_H
