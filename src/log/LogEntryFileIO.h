#ifndef LOGENTRYFILEIO_H
#define LOGENTRYFILEIO_H

#include <ArduinoJson.h>
#include <string>

#include "../JsonService.h"
#include "LogEntry.h"

enum class IOType {
    read = 1,
    write = 2
};

class LogEntryFileIO : public LogEntry {
public:
    LogEntryFileIO(IOType ioType,
        std::string path,
        std::string data,
        LogEntryLevel type = LogEntryLevel::Info) : LogEntry()
    {
        this->type = type;
        this->path = path;
        this->data = data;
        this->ioType = ioType;
    }

    virtual void setMessage(const int espId) override {
        DynamicJsonBuffer buffer;
        JsonObject& json = buffer.createObject();
        json["action"] = "insert";
        JsonObject& logEntry = json.createNestedObject("logEntry");
        logEntry["id"] = espId;
        logEntry["type"] = static_cast<int>(this->type);
        logEntry["path"] = this->path.c_str();
        logEntry["payload"] = this->data.c_str();

        switch (this->ioType) {
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

        this->message = JsonService::getString(json);
    }
private:
    LogEntryLevel type;
    IOType ioType;
    std::string path;
    std::string data;
};

#endif // LOGENTRYFILEIO_H
