#ifndef LOGENTRY_H
#define LOGENTRY_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <string>

uint8_t OUT_HTTP_POST = 0x01;
uint8_t OUT_SERIAL = 0x02;

enum class IOType {
    read = 1,
    write = 2
};

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

class LogEntryFile : public LogEntry {
public:
    LogEntryFile(IOType type, std::string path, std::string data,
        LogEntryType entryType = LogEntryType::Info,
        uint8_t output = OUT_HTTP_POST) {
        this->type = type;
        this->path = path;
        this->data = data;
        this->output = output;
        this->setMessage();
    }

private:
    IOType type;
    std::string path;
    std::string data;
    uint8_t output;

    void setMessage() {
        this->message = "LogUpdate=";
        DynamicJsonBuffer buffer;
        JsonObject& json = buffer.createObject();
        json["action"] = "insert";
        JsonObject& logEntry = json.createNestedObject("logEntry");
        logEntry["type"] = static_cast<int>(this->type);
        logEntry["path"] = this->path.c_str();
        logEntry["payload"] = this->data.c_str();

        switch (this->type) {
            case IOType::read:
                logEntry["text"] = std::string("Reading file (" + this->path + "). Payload: " + this->data).c_str();
                break;
            case IOType::write:
                logEntry["text"] = std::string("Writing file (" + this->path + "). Payload: " + this->data).c_str();
                break;
            default:
                logEntry["text"] = "Unknown I/O type.";
                break;
        }
        char jsonString[2048];
        json.printTo(jsonString);
        std::string str(jsonString);
        this->message += str;
    }
};

#endif // LOGENTRY_H
