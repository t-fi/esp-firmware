#include "JsonService.h"
#include "FileSystemService.h"

JsonObject& JsonService::parse(const std::string jsonString)
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

JsonObject& JsonService::parseFile(const std::string filePath)
{
    std::string configString = this->fileSystemService.read(filePath);
    return parse(configString);
}

void JsonService::save(const std::string path, const JsonObject& json)
{
    std::string jsonString = this->getString(json);
    this->fileSystemService.write(path, jsonString);
}

std::string JsonService::getString(const JsonObject& json)
{
    size_t bufferSize = json.measureLength() + 1; // +1: termination char
    char jsonString[bufferSize];
    json.printTo(jsonString, bufferSize);
    return std::string(jsonString);
}
