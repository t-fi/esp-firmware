#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <FS.h>

typedef struct WifiCredentials {
    char* ssid;
    char* password;
} WifiCredentials;

typedef struct LedColor {
    int red;
    int green;
    int blue;
    int warmWhite;
} LedColor;

WiFiServer server(420);

String readFile(String name) {
    String data = "";
    File f = SPIFFS.open(name, "r");
    if (f) {
        data = f.readStringUntil(EOF);
        f.close();
    }

    return data;
}

void setupWifi() {
    WifiCredentials* credentials = getWifiCredentials();
    connectToWifi(credentials);
    free(credentials);
}

void setup() {
    Serial.begin(9600);
    delay(100);
    startSpiffs();
    // loadConfiguration();
    setupWifi();
}

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

WifiCredentials* getWifiCredentials() {
    WifiCredentials* credentials = (WifiCredentials*) malloc(sizeof(WifiCredentials));

    JsonObject &json = getFileAsJson("/wifiCred");
    String ssid = json["ssid"];
    String password = json["password"];

    char* ssid_char = (char*) malloc(32 * sizeof(char));
    char* password_char = (char*) malloc(64 * sizeof(char));

    ssid.toCharArray(ssid_char, 32);
    password.toCharArray(password_char, 64);

    credentials->ssid = ssid_char;
    credentials->password = password_char;

    return credentials;
}

void connectToWifi(WifiCredentials* credentials) {
    Serial.printf("Connecting to %s ", credentials->ssid);
    WiFi.begin(credentials->ssid, credentials->password);
    int c = 0;
    while (WiFi.status() != WL_CONNECTED && c < 40) {
        delay(500);
        c++;
        Serial.print(".");

        if (c == 40){
            setupAccessPoint();
        }
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("");
        Serial.println("WiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
        server.begin();
    }
}

void updateFirmware(String path){
    Serial.println(path);
    if (path != "") {
        for (int i = 0; i < 10; ++i) {
            Serial.println("Fetching firmware...");
            int result = ESPhttpUpdate.update(path);
            Serial.println(result);
            Serial.println(ESPhttpUpdate.getLastError());

            if (result == 2) {
                delay(100);
                ESP.restart();
            }
        }
    }
}

void setupAccessPoint(){
    WiFi.mode(WIFI_AP);
    Serial.print("Setting soft-AP ... ");

    if(WiFi.softAP("ESPsoftAP_01", "12345678")) {
        Serial.println("Ready");
        server.begin();
    } else {
        Serial.println("Failed!");
    }
}

void startSpiffs(){
    SPIFFS.begin();
}

JsonObject& getFileAsJson(String filePath) {
    String configString = readFile(filePath);
    JsonObject& json = parseJson(configString);

    return json;
}

void restart(){
    delay(100);
    ESP.restart();
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

void formatFileSystem(){
    SPIFFS.format();
    SPIFFS.begin();
}

void configureWifi(JsonObject& json) {
    File f = SPIFFS.open("/wifiCred", "w");
    if (f) {
        char buffer[256];
        json.printTo(buffer, sizeof(buffer));
        f.print(buffer);
        f.close();
        Serial.println("Successfully updated Wifi credentials.");
    }
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

void configureEsp(JsonObject& json) {
    File f = SPIFFS.open("/config.json", "w");
    if (f) {
        // buffer of 4096 crashes esp
        char buffer[2048];
        json.printTo(buffer, sizeof(buffer));
        f.print(buffer);
        f.close();
        Serial.println("Successfully updated config.");
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
