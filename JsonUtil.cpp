#include "JsonUtil.h"
#include "FileSystemUtil.h"

JsonObject& JsonUtil::parse(String jsonString)
{
    DynamicJsonBuffer JSONBuffer;
    JsonObject& parsed = JSONBuffer.parseObject(jsonString);

    if (!parsed.success()) {
        const int BUFFER_SIZE = JSON_OBJECT_SIZE(0);
        StaticJsonBuffer<BUFFER_SIZE> jsonBuffer;
        return JSONBuffer.parseObject("{}");
    }
    return parsed;
}

JsonObject& JsonUtil::parseFile(String filePath)
{
    String configString = FileSystemUtil::read(filePath);
    JsonObject& json = parse(configString);

    return json;
}

void JsonUtil::save(String path, JsonObject& json)
{
    char buffer[2048];
    json.printTo(buffer, sizeof(buffer));
    FileSystemUtil::write(path, buffer);
}

