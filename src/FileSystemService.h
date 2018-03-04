#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <string>

#include "log/LogService.h"

class FileSystemService {
public:
    FileSystemService(LogService& logService) : logService(logService) {}
    std::string read(const std::string path);
    void write(const std::string path, const std::string content);
    void format();
private:
    LogService& logService;
};

#endif // FILESYSTEM_H
