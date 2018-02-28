#ifndef LOGENTRY_H
#define LOGENTRY_H

#include <Arduino.h>
#include <ESP8266WiFi.h>

uint8_t OUT_HTTP_POST = 0x01;
uint8_t OUT_SERIAL = 0x02;

enum class IoTypes {
    read = 1,
    write = 2
};

enum class LogTypes {
    FileRead = 1,
    FileWrite = 2
};

class LogEntry {
public:
    virtual void write() = 0;
    void send() {

    }
};

class LogEntryFile : public LogEntry {
public:
    LogEntryFile(IoTypes type, String path, String data, uint8_t output = OUT_HTTP_POST) {
        this->type = type;
        this->path = path;
        this->data = data;
        this->output = output;
        this->setMessage();
    }

    void write() override {

    }
private:
    IoTypes type;
    String path;
    String data;
    uint8_t output;
    String message;

    String setMessage() {
        switch (this->type) {
            case IoTypes::read:
                this->message = "";
                break;
            case IoTypes::write:
                this->message = "";
                break;
            default:
                this->message = "Unknown I/O type.";
                break;
        }
    }
};
#endif // LOGENTRY_H
