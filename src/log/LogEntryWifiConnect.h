#ifndef LOGENTRYWIFICONNECT_H
#define LOGENTRYWIFICONNECT_H

#include <ArduinoJson.h>
#include <string>

#include "LogEntry.h"

class LogEntryWifiConnect : public LogEntry {
public:
    LogEntryWifiConnect(std::string ssid, std::string password, LogEntryType type = LogEntryType::Info) {
        this->ssid = ssid;
        this->password = password;
        this->type = type;
        this->setMessage();
    }

private:
    std::string ssid;
    std::string password;
    LogEntryType type;

    void setMessage() {
        this->message = "LogUpdate=";
        DynamicJsonBuffer buffer;
        JsonObject& json = buffer.createObject();
        json["action"] = "insert";
        JsonObject& logEntry = json.createNestedObject("logEntry");
        logEntry["ssid"] = this->ssid.c_str();
        logEntry["type"] = static_cast<int>(this->type);
        logEntry["text"] = std::string("Trying to connect to " + this->ssid).c_str();
        std::string jsonString = JsonUtil::getString(json);
        this->message += jsonString;
    }
};

#endif // LOGENTRYWIFICONNECT_H
