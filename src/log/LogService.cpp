#include "LogService.h"
#include "../WifiService.h"

void LogService::setWifi(WifiService* wifiService) {
    this->wifiService = wifiService;
    this->isInitialized = true;
}

void LogService::log(LogEntry logEntry) {
    if (this->canSend()) logEntry.send();
    else this->queue.push(logEntry);
}

void LogService::daemon() {
    if (this->queue.size() < 1) return;
    if (this->canSend()) {
            this->queue.front().send();
            this->queue.pop();
    }
}

bool LogService::canSend() {
    return this->isInitialized && this->wifiService->isConnected();
}
