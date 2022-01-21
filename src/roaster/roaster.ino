/*
 * Sketch by tjburrows as fork of YokuSan and LeFish from kaffee-netz.de
 * Project thread: https://www.youtube.com/watch?v=pTog0-5Rx7I
 * YokuSun initial project dropbox: https://www.dropbox.com/sh/431hbvdjr7k1vzb/AACo8r3lY0x2tSdt06G-mDh7a?dl=0
 * LeFish project: https://github.com/LeFish1/ESP8266_CoffeeRoaster 
 * 
 * 
*  _    _                           _____                 _   _             
*| |  | |                         |  __ \               | | (_)            
*| |__| | __ _ _ __  _ __  _   _  | |__) |___   __ _ ___| |_ _ _ __   __ _ 
*|  __  |/ _` | '_ \| '_ \| | | | |  _  // _ \ / _` / __| __| | '_ \ / _` |
*| |  | | (_| | |_) | |_) | |_| | | | \ \ (_) | (_| \__ \ |_| | | | | (_| |
*|_|  |_|\__,_| .__/| .__/ \__, | |_|  \_\___/ \__,_|___/\__|_|_| |_|\__, |
*             | |   | |     __/ |                                     __/ |
*            |_|   |_|    |___/                                     |___/ 
* 
*/

#include <Arduino.h>
#include <FS.h>

/*Dimmer Config 
 * ATTENTION: be aware this lib needs to be modified inorder to work with ESP8266
 * Either use the one provided in github or refer to https://github.com/RobotDynOfficial/RBDDimmer/issues/14
 */
#include <RBDdimmer.h>
const int outputPin = 5;
const int zerocross = 2; // for boards with CHANGEBLE input pins
dimmerLamp dimmer(outputPin, zerocross); //initialase port for dimmer for ESP8266, ESP32, Arduino due boards


//Wifi libs 
#include <ESP8266WiFi.h>
#include <WebSocketsServer.h> //Websockets Lib by links2004

//Thermocouple
#include <MAX6675_Thermocouple.h> //Lib by YuriiSaliov
#include <RunningMedian.h> //Lib by RobTillaart

//JSON for Artisan Websocket implementation
#include "ArduinoJson.h"

//Ticker to execute actions at defined intervals
#include "TickTwo.h" //ESP8266 compatible version of Ticker by sstaub
 
WebSocketsServer webSocket = WebSocketsServer(8080);

//Wifi Config 
const char *ssid     = "YOURSSID";
const char *password = "YOURPW";

// Set your Static IP address
IPAddress local_IP(192, 168, 0, 47);
// Set your Gateway IP address
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);

//Initialize needed variables
int DimmerVal = 0;
float BeanTemp = 21.0;
float randomnumber = 0;
 
#define DEBUG_SERIAL Serial

/*
 * Smoothed bean temperature measurement using MAX6675_Thermocouple by Yurii Salimov
 * https://github.com/YuriiSalimov/MAX6675_Thermocouple
 * implementation by LeFish
 */
int SCK_PIN = 15;
int BeanTemp_CS_PIN = 13;
int SO_PIN = 12;
 
Thermocouple* originThermocouple = NULL;
RunningMedian Med_BeanTemp = RunningMedian(6);
void measureBeanTemp()
{
  //BeanTemp = SmoothBeanTemp->readCelsius();
  float this_BeanTemp = originThermocouple->readCelsius();
  Med_BeanTemp.add(this_BeanTemp);
}


//Define Artisan Websocket events to exchange data
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

  //Artisan schickt Anfrage als TXT
  //TXT zu JSON lt. https://forum.arduino.cc/t/assistance-parsing-and-reading-json-array-payload-websockets-solved/667917

  const size_t capacity = JSON_OBJECT_SIZE(3) + 60; //Memory pool
  DynamicJsonDocument doc(capacity);

    switch(type) {
        case WStype_DISCONNECTED:
            Serial.printf("[%u] Disconnected!\n", num);
            break;
        case WStype_CONNECTED:
            {
                IPAddress ip = webSocket.remoteIP(num);
                Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        
                // send message to client
                webSocket.sendTXT(num, "Connected");
            }
            break;
        case WStype_TEXT:
            { 
            //DEBUG WEBSOCKET
            //Serial.printf("[%u] get Text: %s\n", num, payload);

            //Extract Values lt. https://arduinojson.org/v6/example/http-client/
            //Artisan Anleitung: https://artisan-scope.org/devices/websockets/

            deserializeJson(doc, (char *)payload);

            //2 Nodes einzeln (Feld: "command"): "getBT" und "getDimmerVal"
            //oder
            // alle Nodes auf einmal (Feld: "command"): "getData"
            //char* entspricht String
            String command = doc["command"].as<char*>();
            
            Serial.printf("Command received: %s \n",command);  
            
            long ln_id = doc["id"].as<long>();
            //Get BurnerVal from Artisan over Websocket
            if(!doc["BurnerVal"].isNull())
            {
              //Serial.print("BurnerVal: ");
              //Serial.println(doc["BurnerVal"].as<long>());  
              DimmerVal = doc["BurnerVal"].as<long>();
              dimmer.setPower((int)DimmerVal);
              Serial.printf("Dimmer power set to: %u \n",(int)DimmerVal);
            }

            //Send Values to Artisan over Websocket
            JsonObject root = doc.to<JsonObject>();
            JsonObject data = root.createNestedObject("data");
            if(command == "getBT")
            {
              root["id"] = ln_id;
              data["BT"] = Med_BeanTemp.getMedian();
              //randomnumber = rand()%200;
              //data["BT"] = randomnumber;
              //DEBUG_SERIAL.printf("getBT created BT: %f \n", randomnumber);
              DEBUG_SERIAL.printf("getBT reads BT: %f \n", Med_BeanTemp.getMedian());
            }
            else if(command == "getDimmerVal")
            {
              root["id"] = ln_id;
              data["DimmerVal"] = float(DimmerVal);
              Serial.printf("getDIMMERVAL current DV: %f \n", float(DimmerVal));
            }
            else if(command == "getData")
            {
              root["id"] = ln_id;
              data["BT"] = Med_BeanTemp.getMedian();
              //randomnumber = rand()%200;
              //data["BT"] = randomnumber;
              data["DimmerVal"] = float(DimmerVal);
              Serial.printf("getData created BT: %f and current DimmerVal: %f \n", Med_BeanTemp.getMedian(), float(DimmerVal));
            }

            char buffer[200]; // create temp buffer
            size_t len = serializeJson(doc, buffer);  // serialize to buffer

            //DEBUG WEBSOCKET
            //Serial.println(buffer);

            webSocket.sendTXT(num, buffer);

            // send message to client
            // webSocket.sendTXT(num, "message here");

            // send data to all connected clients
            // webSocket.broadcastTXT("message here");
            }
            break;
        case WStype_BIN:
            Serial.printf("[%u] get binary length: %u\n", num, length);
            hexdump(payload, length);

            // send message to client
            // webSocket.sendBIN(num, payload, length);
            break;
    }
}

/*
 *Ticker implementations  
 *Measure Bean Temperature every 250ms
 */
 
void func250MS() {
    measureBeanTemp();
  }
  
TickTwo ticker250MS(func250MS, 250, 0, MILLIS);
 
void setup() {
    DEBUG_SERIAL.begin(115200);
 
//  DEBUG_SERIAL.setDebugOutput(true);
 
    DEBUG_SERIAL.println();
    DEBUG_SERIAL.println();
    DEBUG_SERIAL.println();
 
    for(uint8_t t = 4; t > 0; t--) {
        DEBUG_SERIAL.printf("[SETUP] BOOT WAIT %d...\n", t);
        DEBUG_SERIAL.flush();
        delay(1000);
    }

    //Thermocouple
    originThermocouple = new MAX6675_Thermocouple(SCK_PIN, BeanTemp_CS_PIN, SO_PIN);

    //Start Tickers
    ticker250MS.start();

    //Initialise dimmer
    dimmer.begin(NORMAL_MODE, ON);  
  
    // Configures static IP address
    if (!WiFi.config(local_IP, gateway, subnet)) {
        Serial.println("STA Failed to configure");
    } else {
        Serial.println("STA configured");
      }
    WiFi.begin(ssid, password);
 
    while ( WiFi.status() != WL_CONNECTED ) {
      delay ( 500 );
      Serial.print ( "." );
    }
    DEBUG_SERIAL.print("Local IP: "); DEBUG_SERIAL.println(WiFi.localIP());
    // server address, port and URL
    webSocket.begin();
 
    // event handler
    webSocket.onEvent(webSocketEvent);
}
 
unsigned long lastUpdate = millis();
 
 
void loop() {
    webSocket.loop();
    ticker250MS.update();
}
