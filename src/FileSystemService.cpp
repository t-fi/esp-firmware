#include <FS.h>

#include "FileSystemService.h"
#include "log/LogEntryFileIO.h"

std::string FileSystemService::read(const std::string path)
{
    File f = SPIFFS.open(path.c_str(), "r");
    if (f) {
        char buffer[f.size()];
        for (int i = 0; i < f.size(); ++i) {
            char asciiCode = f.read();
            if (asciiCode == 10) {
                buffer[i] = '\0';
                break;
            }
            buffer[i] = asciiCode;
        }

        f.close();
        std::string payload(buffer);
        if (path != "/wifiCredentials.json") {
            LogEntryFileIO* logEntry = new LogEntryFileIO(IOType::read, path, payload);
            this->logService.log(logEntry);
        }

        return payload;
    }

    return std::string();
}

void FileSystemService::write(const std::string path, const std::string content)
{
    LogEntryFileIO* logEntry = new LogEntryFileIO(IOType::write, path, content);
    this->logService.log(logEntry);
    File f = SPIFFS.open(path.c_str(), "w");
    if (f) {
        f.print(content.c_str());
        f.close();
    }
}

void FileSystemService::format()
{
    SPIFFS.format();
    SPIFFS.begin();
}
