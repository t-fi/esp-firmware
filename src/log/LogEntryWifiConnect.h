#ifndef LOGENTRYWIFICONNECT_H
#define LOGENTRYWIFICONNECT_H

#include <ArduinoJson.h>
#include <string>

#include "../JsonService.h"
#include "LogEntry.h"

class LogEntryWifiConnect : public LogEntry {
public:
    LogEntryWifiConnect(std::string ssid,
        std::string password,
        LogEntryLevel type = LogEntryLevel::Info) : LogEntry()
    {
        this->ssid = ssid;
        this->password = password;
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
        logEntry["text"] = std::string("Trying to connect to " + this->ssid).c_str();
        this->message = JsonService::getString(json);
    }
private:
    std::string ssid;
    std::string password;
    LogEntryLevel type;
};

#endif // LOGENTRYWIFICONNECT_H
