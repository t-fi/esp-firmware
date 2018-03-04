#ifndef LOGSERVICE_H
#define LOGSERVICE_H

#include <queue>

#include "LogEntry.h"

// Forward Declarations to resolve circular dependency
class WifiService;
class EspService;

class LogService {
public:
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
};

#endif // LOGSERVICE_H
