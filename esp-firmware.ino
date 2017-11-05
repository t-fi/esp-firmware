#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <ESP8266WiFi.h>
#include <FS.h>

#include "Enums.h"
#include "WifiUtil.h"
#include "JsonUtil.h"
#include "FilesystemUtil.h"
#include "EspUtil.h"

WiFiServer server(420);

typedef struct LedColor {
    int red;
    int green;
    int blue;
    int warmWhite;
} LedColor;

void setup() {
    Serial.begin(9600);
    delay(100);
    SPIFFS.begin();
    setupWifi();
    server.begin();
}

String getTcpMessage() {
    WiFiClient client = server.available();
    if (!client) return "";
    Serial.println("A client connected");
    //String message = readRequest(&client);
    String message = client.readStringUntil('\n');
//    message = message.substring(0, message.length()-1);

    Serial.println(message);
    client.stop();
    return message;
}

void parseFlashCommand(JsonObject& json) {
    String path = json["url"];
    if(path == "http://192.168.0.119:80/arduino.bong") {
        updateFirmware(path);
    }
}

bool isConnected(int componentId) {
    bool isConnected = false;
    JsonObject& config = getFileAsJson("/config.json");
    JsonArray& components = config["esp"]["components"];

    for (auto component : components) {
        if (componentId == component["componentId"].as<int>()) {
            isConnected = true;
        }
    }

    return isConnected;
}

bool isRelay(int componentId) {
    bool isRelay = false;

    if (isConnected(componentId)) {
        // TODO: Use config to check if the device is of correct type to perform action
    }

    return isRelay;
}

bool isLedStrip(int componentId) {
    bool isLedStrip = false;

    if (isConnected(componentId)) {
        // TODO: Use config to check if the device is of correct type to perform action
    }

    return isLedStrip;
}

void toggle(int componentId) {
    if (isRelay(componentId)) {
        // TODO: Logic to toggle on gpio or whatever
    }
}

void setColor(int componentId, LedColor* ledColor) {
    if (isLedStrip(componentId)) {
        // TODO: Set color to r,g,b,ww
    }
}

void setFade(int componentId) {
    if (isLedStrip(componentId)) {
        // TODO: Set a fade
    }
}

void parseSetColorCommand(JsonObject& json) {
    int componentId = json["componentId"].as<int>();
    if (json.containsKey("fade")) {
        setFade(componentId);
    } else {
        LedColor* ledColor = (LedColor*) malloc(sizeof(LedColor));
        ledColor->red = json["red"].as<int>();
        ledColor->green = json["green"].as<int>();
        ledColor->blue = json["blue"].as<int>();
        ledColor->warmWhite = json["warmWhite"].as<int>();
        setColor(componentId, ledColor);
        free(ledColor);
    }
}

void handleCommands(JsonObject& json){
    if(json.containsKey("flash")){
        parseFlashCommand(json["flash"]);
    } else if (json.containsKey("configureEsp")) {
        configureEsp(json["configureEsp"]);
    } else if (json.containsKey("configureWifi")) {
        configureWifi(json["configureWifi"]);
    } else if (json.containsKey("toggle")) {
        toggle(json["toggle"]["componentId"].as<int>());
    } else if (json.containsKey("setColor")) {
        parseSetColorCommand(json["setColor"]);
    }
}

void handleEcho(String message) {
    if (message == "new") {
        Serial.println("4Real! 00ld firmware!");
    }

    if (message.substring(0, 6) == "update") {
        Serial.println("Updating..");
        Serial.println(message.substring(6));
        String path = "";
        path += message.substring(6);
        //path = "http://192.168.0.119:80/arduino.bin";
        Serial.println(path);
        int result = ESPhttpUpdate.update(path);
        Serial.println(result);
        Serial.println(ESPhttpUpdate.getLastError());
    }
    if (message == "reset") {
        restart();
    }
    if (message == "format") {
        formatFileSystem();
    }
    if (message == "read") {
        WifiCredentials* credentials = getWifiCredentials();

        Serial.print("ssid: " );
        Serial.println(credentials->ssid);
        Serial.print("key: " );
        Serial.println(credentials->password);
        Serial.println("End of file");
        free(credentials);
    }
    if (message == "readConfig") {
        String config = readFile("/config.json");
        Serial.println(config);
        isConnected(1);
    }
}

void loop() {
    String message = getTcpMessage();
    handleEcho(message);

    JsonObject &json = parseJson(message);
    if (json.containsKey("command")) {
        handleCommands(json["command"]);
    }
}
