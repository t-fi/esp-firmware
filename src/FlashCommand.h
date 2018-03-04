#ifndef FLASHCOMMAND_H
#define FLASHCOMMAND_H

#include <string>

class FlashCommand {
public:
    FlashCommand(std::string url);
    std::string url;
};

#endif // FLASHCOMMAND_H
