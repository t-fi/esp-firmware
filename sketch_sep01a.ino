#include <ESP8266WiFi.h>

const char* ssid = "KDG-328AB";
const char* password = "yB8cCwu24wwQ";

WiFiServer server(420);

void setup(){
Serial.begin(9600);
  delay(100);
  Serial.println("hi1");
  Serial.println("hi2");

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

  for(int i = 0; i < 100; i++){
   getTcpMessage();
   delay(500);
  }

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
      Serial.write(c);

      if ('\n' == c)
      {
        return request;
      }
      request += c;
    }
  } 
  return request;
}


void loop() {
  // put your main code here, to run repeatedly:

}
