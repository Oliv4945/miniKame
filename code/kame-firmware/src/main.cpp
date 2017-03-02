#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Servo.h>
#include "WemosMiniD1.h"
#include "minikame.h"



// Wifi Access Point configuration
const char* ssid = "kame";
const char* password = "asdf";


MiniKame robot( /* FLI */ W_D4, /* FRI */ W_D2,
                /* FLO */ W_D3, /* FR0 */ W_D1,
                /* BLI */ W_D8, /* BRI */ W_D6,
                /* BLO */ W_D7, /* BRO */ W_D5
              );
ESP8266WebServer server(80);

// Declare functions
void handleRoot();
void handleNotFound();

void setup() {
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid, password);
    server.begin();
    Serial.begin(115200);

    Serial.print("MiniKame starting...");
    delay(1000);
    robot.init();
    Serial.println("OK");


    Serial.print("IP - ");
    Serial.println(WiFi.localIP());
    Serial.print("MDNS - ");
    if (MDNS.begin("kame")) {
      Serial.println("OK");
    } else {
      Serial.println("NOK");
    }

    // Define web pages callbacks
    server.on("/", handleRoot);
    server.on("/inline", [](){
      server.send(200, "text/plain", "this works as well");
    });
    server.onNotFound(handleNotFound);

    server.begin();
    Serial.println("HTTP - Server started\n");
}

void loop() {
    server.handleClient();

    /*
    WiFiClient client = server.available();
    if (!client) {
        IPAddress myIP = WiFi.softAPIP();
        Serial.print("AP IP address: ");
        Serial.println(myIP);
        delay(1000);
    }
    while (client.connected()) {
        if (running){
            Serial.println("running");
            if (client.available()) {
                while(client.available()) input = client.readStringUntil('+');
                parseData(input);
            }
            else {
                Serial.println("Keep Moving");
                parseData(input);
            }
        }
        else{
            Serial.println("Normal mode");
            if (client.available()) {
                while(client.available()) input = client.readStringUntil('+');
                parseData(input);
            }
            else robot.home();
        }
    }
    */
}

/*
void parseData(String data){

    switch (data.toInt()){

        case 1: // Up
            robot.walk(1,550);
            running = 1;
            break;

        case 2: // Down
            break;

        case 3: // Left
            robot.turnL(1,550);
            running = 1;
            break;

        case 4: // Right
            robot.turnR(1,550);
            running = 1;
            break;

        case 5: // STOP
            running = 0;
            break;

        case 6: // heart
            robot.pushUp(2,2000);
            break;

        case 7: // fire
            robot.upDown(4,250);
            break;

        case 8: // skull
            robot.jump();
            break;

        case 9: // cross
            robot.hello();
            break;

        case 10: // punch
            robot.frontBack(4,200);
            break;

        case 11: // mask
            robot.dance(2,1000);
            break;

        default:
            robot.home();
            break;
    }
}
*/


void handleRoot() {
  server.send(200, "text/plain", "kame control");}

void handleNotFound(){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}
