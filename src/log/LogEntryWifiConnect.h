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
        LogEntryLevel type = LogEntryLevel::Info)
    {
        this->ssid = ssid;
        this->password = password;
        this->type = type;
        this->setMessage();
    }

private:
    std::string ssid;
    std::string password;
    LogEntryLevel type;

    void setMessage() {
        DynamicJsonBuffer buffer;
        JsonObject& json = buffer.createObject();
        json["action"] = "insert";
        JsonObject& logEntry = json.createNestedObject("logEntry");
        logEntry["ssid"] = this->ssid.c_str();
        logEntry["type"] = static_cast<int>(this->type);
        logEntry["text"] = std::string("Trying to connect to " + this->ssid).c_str();
        this->message = JsonService::getString(json);
    }
};

#endif // LOGENTRYWIFICONNECT_H
