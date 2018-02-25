#include "ServerUtil.h"

String ServerUtil::receive(WiFiServer& server)
{
    WiFiClient client = server.available();
    if (!client)
        return "";
    Serial.println("A client connected");
    String message = client.readStringUntil('\r');
    Serial.println(message);
    client.stop();

    return message;
}
