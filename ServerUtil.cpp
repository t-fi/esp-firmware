#include "ServerUtil.h"

String ServerUtil::receive(WiFiServer& server)
{
    WiFiClient client = server.available();
    if (!client)
        return "";
    Serial.println("A client connected");
    //String message = readRequest(&client);
    String message = client.readStringUntil('\n');
    //    message = message.substring(0, message.length()-1);

    Serial.println(message);
    client.stop();
    return message;
}

