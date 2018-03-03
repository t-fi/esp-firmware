#ifndef LOGENTRYFILEIO_H
#define LOGENTRYFILEIO_H

#include <ArduinoJson.h>
#include <string>

#include "JsonUtil.h"
#include "LogEntry.h"

enum class IOType {
    read = 1,
    write = 2
};

class LogEntryFileIO : public LogEntry {
public:
    LogEntryFileIO(IOType ioType, std::string path, std::string data,
        LogEntryType type = LogEntryType::Info) {
        this->type = type;
        this->path = path;
        this->data = data;
        this->output = output;
        this->ioType = ioType;
        this->setMessage();
    }

private:
    LogEntryType type;
    IOType ioType;
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
        std::string jsonString = JsonUtil::getString(json);
        this->message += jsonString;
    }
};

#endif // LOGENTRYFILEIO_H
