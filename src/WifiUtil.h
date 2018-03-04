#ifndef WIFI_H
#define WIFI_H

#include <string>

class WifiUtil {
public:
    std::string getSsid();
    std::string getPassword();

    void setup();
    void getCredentials();
    void connect();
    void setupAccessPoint();
    void updateCredentials(std::string newSsid, std::string newPassword);

private:
    std::string ssid;
    std::string password;
    void printStatus();
};

#endif // WIFI_H
