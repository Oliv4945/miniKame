// #define WIFI_AP_MODE

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#ifndef WIFI_AP_MODE
  #include <WiFiManager.h>
#endif
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Servo.h>
#include "WemosMiniD1.h"
#include "minikame.h"
#include "../tools/root.h"



// Wifi Access Point configuration
const char* ssid = "kameControl";
const char* password = "kamecontrol";
const uint32_t udpBufferSize = 256;

bool running = 0;
byte packetBuffer[udpBufferSize];
String input;
WiFiUDP udp;

MiniKame robot( /* FLI */ W_D4,
                /* FRI */ W_D2,
                /* FLO */ W_D3,
                /* FR0 */ W_D1,
                /* BLI */ W_D8,
                /* BRI */ W_D6,
                /* BLO */ W_D7,
                /* BRO */ W_D5
              );


// Declare functions
void handleCommands(byte command, byte strength);

void setup() {
  Serial.begin(115200);
  
  Serial.print("MiniKame starting...");
  robot.init();
  robot.home();


  // robot.walk(5,550);
  // robot.turnR(5,550);
  // robot.hello();
  // robot.home();

    /*#ifdef WIFI_AP_MODE
      WiFi.mode(WIFI_AP);
      WiFi.softAP(ssid, password);
    #else
      WiFiManager wifiManager;
      wifiManager.autoConnect(ssid);
    #endif
    */
  /*
  // Connect to AP
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Connected to WiFi");
  */
  // Act as AP
  WiFi.softAP("miniKame", "logokame");
  


    // UDP server
    udp.begin(2000);
    Serial.println("UDP - Server started\n");


    Serial.print("IP - ");
    Serial.println(WiFi.localIP());
    Serial.print("MDNS - ");
    if (MDNS.begin("kame")) {
      Serial.println("OK");
    } else {
      Serial.println("NOK");
    }
}

void loop() {
  uint16_t noReadBytes;

  // If a packet is available
  uint32_t noAvailableBytes = udp.parsePacket();
  if (noAvailableBytes > 0) {
    // Not just a byte FIFO, read until last available packet
    while (noAvailableBytes > 0) {
      if (noAvailableBytes > udpBufferSize) noAvailableBytes = udpBufferSize; // Check if buffer is large enough
      noReadBytes = udp.read(packetBuffer, noAvailableBytes);
      noAvailableBytes = udp.parsePacket();
    }

    /*
    Serial.print("noAvailableBytes: ");
    Serial.println(noAvailableBytes);
    
    for (uint16_t i = 0; i < noReadBytes; i++) {
      Serial.print("0x");
      Serial.print(packetBuffer[i], HEX);
      Serial.print(", ");
    }
    Serial.println(' ');
    */
    handleCommands(packetBuffer[noReadBytes-2], packetBuffer[noReadBytes-1]);
  } else {
    robot.home();
  }
}


void handleCommands(byte command, byte strength) {

  // Compute speed
  // 550 minimum
  // strength comes in from 0 to 100%, have to invert it
  uint16_t speed = (uint16_t) (550.0 + (100.0 - ((float) strength))/100.0 * 1000.0);

  Serial.print("Command: ");
  Serial.print(command);
  Serial.print("\t-\tStrength: ");
  Serial.print(strength);
  Serial.print("\t-\tSpeed: ");
  Serial.println(speed);

  switch (command) {
    case 'i':
      robot.walk(1, speed);
      break;
    case 'j':
      robot.turnL(1, speed);
      break;
    case 'l':
      robot.turnR(1, speed);
      break;
    case 'a':
      robot.hello();
      break;
    case 'z': // heart
      robot.pushUp(2, 2000);
      break;
    case 'e': // fire
      robot.upDown(4, 250);
      break;
    case 'q': // skull
        robot.jump();
        break;
    case 's': // punch
        robot.frontBack(4,200);
        break;
    case 'd': // mask
        robot.dance(2,1000);
        break;
  }
}
