#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#define UDP_PACKET_SIZE 6

const char* ssid = "KDG-328AB";
const char* password = "yB8cCwu24wwQ";

Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver(0x40);

WiFiUDP Udp;
unsigned int localUdpPort = 420;  // local port to listen on
uint8_t incomingPacket[6];  // buffer for incoming packets

uint16_t c1, c2;

void setup() {
  Serial.begin(9600);
  Serial.println();

  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected");

  Udp.begin(localUdpPort);
  Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);

  pwm1.begin();
  pwm1.setPWMFreq(1600);  // This is the maximum PWM frequency
}

void loop()
{
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {   
    // receive incoming UDP packets
    //Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    Udp.read(incomingPacket, UDP_PACKET_SIZE);
    kjn(incomingPacket);
  }
}

inline void kjn(uint8_t* color){
  //for(int i=0; i<UDP_PACKET_SIZE; i++){
    //printBits(color[i]);
    //Serial.println();
  //}
  for(int i=0; i<UDP_PACKET_SIZE; i+=3){
    c2 = ((uint16_t)color[2+i]) | ((((uint16_t)color[1+i]) & (uint16_t)15)<<(uint16_t)8);
    c1 = (((uint16_t)color[1+i])>>(uint16_t)4) | (((uint16_t)color[0+i])<<(uint16_t)4);
    //Serial.printf("%d: %d\n", (i*2)/3, c1);
    //Serial.printf("%d: %d\n", (i*2)/3+1, c2); 

    pwm1.setPWM((i*2)/3, 0, c1);
    pwm1.setPWM((i*2)/3+1, 0, c2);
  }
}
Q
void printBits(uint8_t myByte){
 for(byte mask = 0x80; mask; mask >>= 1){
   if(mask & myByte)
       Serial.print('1');
   else
       Serial.print('0');
 }
 Serial.print(' ');
}

