#include "LogService.h"
#include "../WifiService.h"

void LogService::setWifiService(WifiService* wifiService)
{
    this->wifiService = wifiService;
}

void LogService::setEspService(EspService* espService)
{
    this->espService = espService;
}

void LogService::log(LogEntry* logEntry)
{
    this->queue.push(logEntry);
}

void LogService::daemon()
{
    if (this->queue.size() < 1) return;
    if (this->canSend()) {
        LogEntry* logEntry = this->queue.front();
        logEntry->setMessage(this->espService->getId());
        if (this->output & LOG_OUT_HTTP) logEntry->send();
        if (this->output & LOG_OUT_SERIAL) Serial.println(logEntry->message.c_str());
        this->queue.pop();
        delete logEntry;
    }
}

bool LogService::canSend()
{
    return this->isInitialized() && this->wifiService->isConnected();
}

bool LogService::isInitialized()
{
    return this->espService && this->wifiService;
}
