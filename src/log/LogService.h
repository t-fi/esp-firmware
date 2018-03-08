#ifndef LOGSERVICE_H
#define LOGSERVICE_H

#define LOG_OUT_HTTP 0x01
#define LOG_OUT_SERIAL 0x02

#include <queue>

#include "LogEntry.h"

// Forward Declarations to resolve circular dependency
class WifiService;
class EspService;

class LogService {
public:
    LogService(uint8_t output) : output(output) {}
    void log(LogEntry* logEntry);
    void setWifiService(WifiService* wifiService);
    void setEspService(EspService* espService);
    void daemon();
private:
    bool canSend();
    bool isInitialized();

    WifiService* wifiService = nullptr;
    EspService* espService = nullptr;
    std::queue<LogEntry*> queue;
    const uint8_t output;
};

#endif // LOGSERVICE_H
