#ifndef FLASHCOMMAND_H
#define FLASHCOMMAND_H

#include <Arduino.h>

class FlashCommand {
public:
    FlashCommand(String url);
    String url;
};

#endif // FLASHCOMMAND_H
