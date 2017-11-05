#include "JsonUtil.h"
#include "FilesystemUtil.h"

JsonObject& parseJson(String jsonString) {
    DynamicJsonBuffer JSONBuffer;
    JsonObject& parsed = JSONBuffer.parseObject(jsonString);

    if (!parsed.success()) {
        const int BUFFER_SIZE = JSON_OBJECT_SIZE(0);
        StaticJsonBuffer<BUFFER_SIZE> jsonBuffer;
        return JSONBuffer.parseObject("{}");
    }
    return parsed;
}

JsonObject& getFileAsJson(String filePath) {
    String configString = readFile(filePath);
    JsonObject& json = parseJson(configString);

    return json;
}
