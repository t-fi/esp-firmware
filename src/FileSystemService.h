#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <string>

#include "log/LogService.h"

class FileSystemService {
public:
    FileSystemService(LogService& logService) : logService(logService) {}
    LogService& logService;
    std::string read(std::string path);
    void write(std::string path, std::string content);
    void format();
};

#endif // FILESYSTEM_H
