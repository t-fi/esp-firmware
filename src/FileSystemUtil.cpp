#include <FS.h>

#include "FileSystemUtil.h"
#include "log/LogEntryFileIO.h"

std::string FileSystemUtil::read(std::string path)
{
    String data = "";
    File f = SPIFFS.open(path.c_str(), "r");
    if (f) {
        data = f.readStringUntil(EOF);
        f.close();
    }

    char buffer[1024];
    data.toCharArray(buffer, 1024);
    std::string content(buffer);
    LogEntryFileIO(IOType::read, path, content).send();
    return content;
}

void FileSystemUtil::write(std::string path, std::string content)
{
    LogEntryFileIO(IOType::write, path, content).send();
    File f = SPIFFS.open(path.c_str(), "w");
    if (f) {
        f.print(content.c_str());
        f.close();
    }
}

void FileSystemUtil::format()
{
    SPIFFS.format();
    SPIFFS.begin();
}
