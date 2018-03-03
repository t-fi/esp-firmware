#include <FS.h>

#include "FileSystemUtil.h"

String FileSystemUtil::read(String path)
{
    String data = "";
    File f = SPIFFS.open(path, "r");
    if (f) {
        data = f.readStringUntil(EOF);
        f.close();
    }

    return data;
}

void FileSystemUtil::write(String path, String content)
{
    File f = SPIFFS.open(path, "w");
    if (f) {
        char buffer[content.length() + 1]; // +1: termination char
        content.toCharArray(buffer, content.length() + 1);
        f.print(buffer);
        f.close();
    }
}

void FileSystemUtil::format()
{
    SPIFFS.format();
    SPIFFS.begin();
}
