#include "ServerUtil.h"

Message* ServerUtil::receive(WiFiServer& server)
{
    WiFiClient client = server.available();
    if (!client)
        return (Message*)0;
    // Serial.println("A client connected");

    char* buffer = new char[1024];
    int size;
    for (size = 0; size < 1024;) {
        int byte = client.read();
        if (byte == -1) continue;
        if (byte == 10) break;
        // Serial.println(byte);
        buffer[size] = byte;
        ++size;
    }
    // Serial.print("Size: ");
    // Serial.println(size);
    client.stop();

    return new Message(buffer, size);
}
