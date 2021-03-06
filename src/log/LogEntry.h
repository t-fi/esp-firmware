#ifndef LOGENTRY_H
#define LOGENTRY_H

#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <string>

enum class LogEntryLevel {
    Info = 1,
    Warning = 2,
    Error = 3
};

class LogEntry {
public:
    std::string message;
    virtual void setMessage(const int espId) = 0;
    void send() {
        HTTPClient client;
        client.begin("http://192.168.178.91/esp-web-app/front/index.php");
        client.addHeader("Content-Type", "application/x-www-form-urlencoded");
        client.POST(std::string("LogUpdate=" + this->message).c_str());
        // Serial.println(std::string("LogUpdate=" + this->message).c_str());
        // Serial.println(client.getString());
        client.end();
    }
};

#endif // LOGENTRY_H
