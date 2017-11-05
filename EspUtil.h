#ifndef ESPUTIL_H
#define ESPUTIL_H

#include <Arduino.h>
#include <ArduinoJson.h>

void updateFirmware(String path);
void restart();
void configureEsp(JsonObject& json);

#endif // ESPUTIL_H


