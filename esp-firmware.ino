
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>

#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include  <FS.h>

//const char* ssid = "KDG-328AB";
//const char* password = "yB8cCwu24wwQ";

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
    connectToWifi();
    //TODO: read wifi and password from config
    //TODO: if there is no config, start hosting wifi with ssid espId
}

  JsonObject& parseJson(String jsonString) {
    const int BUFFER_SIZE = JSON_OBJECT_SIZE(20) + JSON_ARRAY_SIZE(100);
    StaticJsonBuffer<BUFFER_SIZE> JSONBuffer;
    JsonObject& parsed = JSONBuffer.parseObject(jsonString);

    if (!parsed.success()) {
        //Serial.println("Parsing failed");
    }

    return parsed;
}

void connectToWifi() {
    Serial.println(readFile("/wifiCred"));
    JsonObject &json = parseJson(readFile("/wifiCred"));
    String ssid = json["command"]["configureWifi"]["ssid"];
    String password = json["command"]["configureWifi"]["password"];

    //String ssid = "KDG-328AB";
    //String password = "yB8cCwu24wwQ";
    Serial.println(ssid);
    Serial.println(password);

    char* ssid_char;
    char* password_char;

    ssid.toCharArray(ssid_char, 32);
    password.toCharArray(password_char, 32);

    Serial.printf("Connecting to %s ", ssid_char);
    WiFi.begin(ssid_char, password_char);
    int c = 0;
    while (WiFi.status() != WL_CONNECTED && c < 10) {
        delay(500);
        c++;
        Serial.print(".");

        if (c == 10){
            setupDefaultWifi();
        }
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    server.begin();
}


//String readRequest(WiFiClient* client)
//{
//    String request = "";
//
//    while (client->connected() && client->available())
//    {
//        char c = client->read();
//
//        if ('\n' == c) return request;
//        request += c;
//    }
//    return request;
//writeToFile(String s){
//
void updateFirmware(String path){
//    String path = "";
//    File f = SPIFFS.open("/f.txt", "r");
//    if (f) {
//        Serial.println("====== Reading from SPIFFS file =======");
//        path = f.readStringUntil('\n');
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
//    }
}

void setupDefaultWifi(){
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
    //server.begin();
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
    message = message.substring(0, message.length()-1);

    Serial.println(message);
    //client.stop();
    return message;
}

//}





//}

void loop() {
//    if(defaultState){
//        String msg = getTcpMessage();
//        if (msg.substring(0, 6) == "update") {
//            Serial.println("Updating..");
//            Serial.println(msg.substring(6));
//            String path = "";
//            path += msg.substring(6);
//            //path = "http://192.168.0.119:80/arduino.bin";
//            Serial.println(path);
//            int result = ESPhttpUpdate.update(path);
//            Serial.println(result);
//            Serial.println(ESPhttpUpdate.getLastError());
//        }
//    }else {
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
        SPIFFS.format();
        SPIFFS.begin();
    }
    if (msg == "write") {
        File f = SPIFFS.open("/f.txt", "w");
        if (!f) {
            Serial.println("file open failed");
        }
        Serial.println("====== test write=========");
        f.print("write test");
        f.close();
    }
    if (msg == "read") {
        File f = SPIFFS.open("/f.txt", "r");
        if (!f) {
            Serial.println("file open failed");
        }
        Serial.println("====== Reading from SPIFFS file =======");
        String s = f.readStringUntil('\n');
        Serial.println(s);
    }
    JsonObject &json = parseJson(msg);
    String deserialized = json["esp"]["id"];

        String ssid = json["command"]["configureWifi"]["ssid"];
        Serial.println(ssid);
        ssid = ssid.substring(9);
    //if (json["command"]["configureWifi"]["ssid"] == "KDG-328AB"){
    if (ssid == "KDG-328AB"){
        //String ssid = json["command"]["configureWifi"]["ssid"];
        String key = json["command"]["configureWifi"]["password"];

        File f = SPIFFS.open("/wifiCred", "w");
        if(f){
            f.print(msg);
            f.close();
            Serial.println("/wifiCred");
            Serial.println(ssid);
            Serial.println(key);
      }
    }
    String path = json["command"]["flash"]["url"];
    if(path == "http://192.168.0.119:80/arduino.bong") {
//        File f = SPIFFS.open("/path", "w");
//        if(f
//            f.print(path);
//            f.close();
//        }
        updateFirmware(path);
//            Serial.println("Updating..");
//            //Serial.println(msg.substring(6));
//            //path = "http://192.168.0.119:80/arduino.bin";
//            Serial.println(path);
//            int result = ESPhttpUpdate.update(path);
//            Serial.println(result);
//            Serial.println(ESPhttpUpdate.getLastError());
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
    //}
}
