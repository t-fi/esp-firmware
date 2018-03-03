#include "JsonUtil.h"
#include "FileSystemUtil.h"

JsonObject& JsonUtil::parse(std::string jsonString)
{
    DynamicJsonBuffer JSONBuffer;
    JsonObject& parsed = JSONBuffer.parseObject(jsonString.c_str());

    if (!parsed.success()) {
        const int BUFFER_SIZE = JSON_OBJECT_SIZE(0);
        StaticJsonBuffer<BUFFER_SIZE> jsonBuffer;
        return JSONBuffer.parseObject("{}");
    }

    return parsed;
}

JsonObject& JsonUtil::parseFile(std::string filePath)
{
    std::string configString = FileSystemUtil::read(filePath);
    return parse(configString);
}

void JsonUtil::save(std::string path, JsonObject& json)
{
    std::string jsonString = JsonUtil::getString(json);
    FileSystemUtil::write(path, jsonString);
}

std::string JsonUtil::getString(JsonObject& json)
{
    char jsonString[2048];
    json.printTo(jsonString);
    return std::string(jsonString);
}
