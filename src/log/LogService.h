#ifndef LOGSERVICE_H
#define LOGSERVICE_H

#include <queue>

#include "LogEntry.h"

class WifiService;

class LogService {
public:
    void log(LogEntry logEntry);
    void setWifi(WifiService* wifiService);
    void daemon();
private:
    bool canSend();

    bool isInitialized = false;
    WifiService* wifiService;
    std::queue<LogEntry> queue;
};

#endif // LOGSERVICE_H
