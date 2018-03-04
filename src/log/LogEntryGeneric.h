#ifndef LOGENTRYGENERIC_H
#define LOGENTRYGENERIC_H

#include <ArduinoJson.h>
#include <string>

#include "../JsonService.h"
#include "LogEntry.h"

class LogEntryGeneric : public LogEntry {
public:
    LogEntryGeneric(std::string text,
        LogEntryLevel type = LogEntryLevel::Info) : LogEntry()
    {
        this->text = text;
        this->type = type;
    }

    virtual void setMessage(const int espId) override {
        DynamicJsonBuffer buffer;
        JsonObject& json = buffer.createObject();
        json["action"] = "insert";
        JsonObject& logEntry = json.createNestedObject("logEntry");
        logEntry["id"] = espId;
        logEntry["type"] = static_cast<int>(this->type);
        logEntry["text"] = this->text.c_str();
        this->message = JsonService::getString(json);
    }
private:
    std::string text;
    LogEntryLevel type;
};

#endif // LOGENTRYGENERIC_H
