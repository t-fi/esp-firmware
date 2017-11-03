# ESP Firmware

## Arduino IDE: Libraries

* [Arduino Core - ESP8266](https://github.com/esp8266/Arduino)
* [Library - JSON](https://github.com/bblanchon/ArduinoJson/) (Can be downloaded using Arduino IDE library manager)
* [Library - PWM driver](https://github.com/adafruit/Adafruit-PWM-Servo-Driver-Library) (Can be downloaded using Arduino IDE library manager)


## Arduino IDE: Helpful Tools

* [SPIFFS filesystem uploader](https://github.com/esp8266/arduino-esp8266fs-plugin)

## Setting Up

* Clone repository
* Resolve all library dependencies
* Build
* Have fun

## Notes

### WiFi Credentials

If you want to use the board with specific credentials (without the need of the server to configure the board) you can upload the credentials as *wifiCredentials.json* to the SPIFFS filesystem. 
The file should look like this:

```
{
	"ssid": "yourSsid",
	"password": "yourSafePassword"
}
```

