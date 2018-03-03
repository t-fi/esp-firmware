#include <FS.h>

#include "FileSystemUtil.h"

std::string FileSystemUtil::read(std::string path)
{
    String data = "";
    File f = SPIFFS.open(path.c_str(), "r");
    if (f) {
        data = f.readStringUntil(EOF);
        f.close();
    }

    char buffer[2048];
    data.toCharArray(buffer, 2048);
    return std::string(buffer);
}

void FileSystemUtil::write(std::string path, std::string content)
{
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
