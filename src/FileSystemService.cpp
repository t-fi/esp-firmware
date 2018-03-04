#include <FS.h>

#include "FileSystemService.h"
#include "log/LogEntryFileIO.h"

std::string FileSystemService::read(std::string path)
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
        LogEntryFileIO(IOType::read, path, payload).send();
        return payload;
    }

    return std::string();
}

void FileSystemService::write(std::string path, std::string content)
{
    LogEntryFileIO(IOType::write, path, content).send();
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
