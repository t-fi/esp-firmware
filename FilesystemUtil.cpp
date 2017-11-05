#include <FS.h>

String readFile(String name) {
    String data = "";
    File f = SPIFFS.open(name, "r");
    if (f) {
        data = f.readStringUntil(EOF);
        f.close();
    }

    return data;
}

void formatFileSystem() {
    SPIFFS.format();
    SPIFFS.begin();
}
