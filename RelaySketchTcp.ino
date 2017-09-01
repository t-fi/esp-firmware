#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include "FS.h"

#define RelayPin D1

const char* ssid     = "";
const char* password = "";

WiFiServer server(4210);

void connectToWifi();

void setup() {
  Serial.begin(115200);
  connectToWifi();
  server.begin();
  pinMode(RelayPin, OUTPUT);
  digitalWrite(RelayPin, LOW);
  SPIFFS.begin();
  configure();
}

void configure() {
  
}

void httpPost(String data) {
  HTTPClient http;
  http.begin("http://192.168.178.43/esp-web-app/public");
  http.addHeader("Content-Type", "text/plain");
  int httpCode = http.POST(data);
  String payload = http.getString();
  http.end();
}

void connectToWifi() {
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

JsonObject& parseJson(String jsonString) {
  const int BUFFER_SIZE = JSON_OBJECT_SIZE(20) + JSON_ARRAY_SIZE(100);
  StaticJsonBuffer<BUFFER_SIZE> JSONBuffer;
  JsonObject& parsed = JSONBuffer.parseObject(jsonString);
  
  if (!parsed.success()) {
    Serial.println("Parsing failed");
  }

  return parsed;
}

String readRequest(WiFiClient* client)
{
  String request = "";

  while (client->connected())
  {
    while (client->available())
    {
      char c = client->read();
//      Serial.write(c);

      if ('\n' == c)
      {
        return request;
      }

      request += c;
    }
  }
  
  return request;
}

String getTcpMessage() {
  WiFiClient client = server.available();
  if (!client) return "";
  Serial.println("A client connected");
  String message = readRequest(&client);
  
  return message;
}

void toggleRelay(int componentId) {
  
}

void evaluateTcpMessage(JsonObject& json) {
  if (json["esp"] != NULL) {
    if (!SPIFFS.exists("/config.json") {
      File configuration = SPIFFS.open("/config.json", "w+");
      if (!configuration) {
        Serial.println("Error creating configuration file.");
      } else {
        Serial.println("Successfully created configuration file.");
      }
    
      configuration.close();
    } else {
      Serial.println("Loading configuration...");
    }
  }

  if (json["action"] == "toggle") {
//    toggleRelay(json["componentId"]);
  }
}

void loop() {
  String message = getTcpMessage();

  if (message != "") {
    Serial.println("message " + message);
    JsonObject& json = parseJson(message);

    evaluateTcpMessage(json);
  
    digitalWrite(RelayPin, !digitalRead(RelayPin));
  }
}

