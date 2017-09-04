#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include  <FS.h>

const char* ssid = "KDG-328AB";
const char* password = "yB8cCwu24wwQ";

WiFiServer server(420);

void setup() {
    Serial.begin(9600);
    delay(100);
    Serial.println("");

    SPIFFS.begin();

    con2wifi();
}

void con2wifi(){
    Serial.printf("Connecting to %s ", ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    server.begin();
}

void startSpiffs(){}

void restart(){
    delay(100);
    ESP.restart();
}

String getTcpMessage() {
    WiFiClient client = server.available();
    if (!client) return "";
    Serial.println("A client connected");
    String message = readRequest(&client);

    return message;
}

String readRequest(WiFiClient* client)
{
    String request = "";

    while (client->connected())
    {
        while (client->available())
        {
            char c = client->read();
            //Serial.write(c);

            if ('\n' == c)
            {
                return request;
            }
            request += c;
        }
    }
    return request;
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

void loop() {
    String msg = getTcpMessage();
    delay(100);
    if(msg == "reset"){
        restart();
    }
    if(msg == "format"){
        SPIFFS.format();
        SPIFFS.begin();
    }
    if(msg == "write"){
        File f = SPIFFS.open("/f.txt", "w");
        if (!f) {
            Serial.println("file open failed");
        }
        Serial.println("====== test write=========");
        f.print("write test");
        f.close();
    }
    if(msg == "read"){
        File f = SPIFFS.open("/f.txt", "r");
        if (!f) {
            Serial.println("file open failed");
        }
        Serial.println("====== Reading from SPIFFS file =======");
        String s=f.readStringUntil('\n');
        Serial.println(s);
    }
    JsonObject& json = parseJson(msg);
    String deserialized = json["esp"]["id"];
    if(deserialized != ""){
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