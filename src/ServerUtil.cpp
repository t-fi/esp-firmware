#include "ServerUtil.h"

String ServerUtil::receive(WiFiServer& server)
{
    WiFiClient client = server.available();
    if (!client)
        return "";
    Serial.println("A client connected");
    String message = client.readStringUntil('\n');
    Serial.println(message);
    if (message.length() > 0) {
        char buffer[message.length()];
        message.toCharArray(buffer, message.length());
        for (int i = 0; i < message.length(); ++i) {
            Serial.printf("%d", buffer[i]);
        }
    }

    client.stop();

    return message;
}
