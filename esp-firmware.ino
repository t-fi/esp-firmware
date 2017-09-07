
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>

#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include  <FS.h>

typedef struct wifiCredentials {
    char* ssid;
    char* password;
} wifiCredentials;

WiFiServer server(420);

void connectToWifi();
bool defaultState=false;

String readFile(String name){
    String data = "";
    File f = SPIFFS.open( name, "r");
    if(f){
        data = f.readStringUntil('\n');
        f.close();
    }
    return data;
}

void setup() {
    // Start serial and wait for things to calm down
    Serial.begin(9600);
    delay(100);
    Serial.println("");

    // initialize SPIFFS
    startSpiffs();

    // loadConfiguration();

    // connect to hardcoded wifi
    setupWifi();
    //TODO: read wifi and password from config
    //TODO: if there is no config, start hosting wifi with ssid espId
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

wifiCredentials* getWifiCredentials() {
    wifiCredentials* credentials = (wifiCredentials*) malloc(sizeof(wifiCredentials));

    Serial.println(readFile("/wifiCred"));
    JsonObject &json = parseJson(readFile("/wifiCred"));
    String ssid = json["command"]["configureWifi"]["ssid"];
    String password = json["command"]["configureWifi"]["password"];

    Serial.println(ssid);
    Serial.println(password);

    char* ssid_char = (char*) malloc(32 * sizeof(char));
    char* password_char = (char*) malloc(64 * sizeof(char));

    ssid.toCharArray(ssid_char, 32);
    password.toCharArray(password_char, 64);

    credentials->ssid = ssid_char;
    credentials->password = password_char;

    return credentials;
}

void setupWifi() {
    wifiCredentials* credentials = getWifiCredentials();
    connectToWifi(credentials);
}

void connectToWifi(wifiCredentials* credentials) {
    char* ssid = credentials->ssid;
    char* password = credentials->password;

    Serial.printf("Connecting to %s ", ssid);
    WiFi.begin(ssid, password);
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
    if(path != ""){
        for(int i=0; i<10; i++){
            Serial.println("Fetching firmware...");
            int result = ESPhttpUpdate.update(path);
            Serial.println(result);
            Serial.println(ESPhttpUpdate.getLastError());
            if(result == 2){
                delay(100);
                ESP.restart();
            }
        }
    }
}

void setupAccessPoint(){
    WiFi.mode(WIFI_AP);
    Serial.print("Setting soft-AP ... ");
    bool result = WiFi.softAP("ESPsoftAP_01", "12345678");
    defaultState = result;
    if(result)
    {
        Serial.println("Ready");
    }
    else
    {
        Serial.println("Failed!");
    }
    server.begin();
}

void startSpiffs(){
    SPIFFS.begin();
}
void loadConfiguration(){
    File f = SPIFFS.open("/config", "r");
    if (!f) {
        Serial.println("file open failed");
    }



    Serial.println("====== test write=========");
    f.print("write test");
    f.close();
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

void formmatFileSystem(){
    SPIFFS.format();
    SPIFFS.begin();
}

void loop() {
    String msg = getTcpMessage();
    //delay(100);

    if (msg == "new") {
        Serial.println("4Real! 00ld firmware!");
    }

    if (msg.substring(0, 6) == "update") {
        Serial.println("Updating..");
        Serial.println(msg.substring(6));
        String path = "";
        path += msg.substring(6);
        //path = "http://192.168.0.119:80/arduino.bin";
        Serial.println(path);
        int result = ESPhttpUpdate.update(path);
        Serial.println(result);
        Serial.println(ESPhttpUpdate.getLastError());
    }
    if (msg == "reset") {
        restart();
    }
    if (msg == "format") {
        formmatFileSystem();
    }
    if (msg == "read") {
        File f = SPIFFS.open("/wifiCred", "r");
        if (!f) {
            Serial.println("file open failed");
        }
        Serial.println("====== Reading from wifiCred=======");
        String s = f.readStringUntil('\n');
        Serial.println(s);
        JsonObject &j = parseJson(s);
        String ssid = j["command"]["configureWifi"]["ssid"];
        String key = j["command"]["configureWifi"]["password"];
        Serial.println("ssid: " );
        Serial.print(ssid);
        Serial.println("key: " );
        Serial.print(key);
        Serial.println("End of file");
    }

    JsonObject &json = parseJson(msg);



    String deserialized = json["esp"]["id"];

    String ssid = json["command"]["configureWifi"]["ssid"];
//    Serial.println(ssid);
//    ssid = ssid.substring(9);
    if (json["command"]["configureWifi"]["ssid"] != "") {
            String ssid = json["command"]["configureWifi"]["ssid"];
            String key = json["command"]["configureWifi"]["password"];

            File f = SPIFFS.open("/wifiCred", "w");
            if (f) {
                f.print(msg);
                f.close();
                Serial.println("/wifiCred");
                Serial.println(ssid);
                Serial.println(key);
            }
    }


    String path = json["command"]["flash"]["url"];
    if(path == "http://192.168.0.119:80/arduino.bong") {
        updateFirmware(path);
    }

    if (deserialized != "") {
        Serial.println(deserialized);
        File f = SPIFFS.open("/f.txt", "w");
        if (!f) {
            Serial.println("file open failed");
        }
        Serial.println("====== Writing to SPIFFS file =========");
        f.print(deserialized);
        f.close();
    }
}
