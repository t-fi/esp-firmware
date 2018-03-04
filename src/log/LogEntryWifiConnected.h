#ifndef LOGENTRYWIFICONNECTED_H
#define LOGENTRYWIFICONNECTED_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <string>

#include "../JsonService.h"
#include "LogEntry.h"

class LogEntryWifiConnected : public LogEntry {
public:
    LogEntryWifiConnected(std::string ssid,
        IPAddress address,
        LogEntryLevel type = LogEntryLevel::Info) : LogEntry()
    {
        this->ssid = ssid;
        this->address = address;
        this->type = type;
    }

    virtual void setMessage(const int espId) override {
        DynamicJsonBuffer buffer;
        JsonObject& json = buffer.createObject();
        json["action"] = "insert";
        JsonObject& logEntry = json.createNestedObject("logEntry");
        logEntry["id"] = espId;
        logEntry["ssid"] = this->ssid.c_str();
        logEntry["type"] = static_cast<int>(this->type);

        logEntry["text"] = std::string("Successfully connected to " + this->ssid
            + ". IP Address: " + this->getIpString()).c_str();
        this->message = JsonService::getString(json);
    }
private:
    std::string ssid;
    IPAddress address;
    LogEntryLevel type;

    std::string getIpString() {
        char ipAddress[16];
        snprintf(ipAddress, 16, "%d.%d.%d.%d", this->address[0],
        this->address[1], this->address[2], this->address[3]);
        return std::string(ipAddress);
    }
};

#endif // LOGENTRYWIFICONNECTED_H
