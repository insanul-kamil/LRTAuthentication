/* main.cpp made by Muhammad Insanul Kamil bin Syaidul Azam, 21/5/2021 */

/* Libraries */
#include <Arduino.h>
#include <PubSubClient.h>               // library for publish and subscribe
#include <Wire.h>
#include <LiquidCrystal_I2C.h>          // library for lcdi2c
#include <DHT.h>                        // library for recording temp and humidity
#include <WiFiEsp.h>                    // library for setting up esp 01 as client at arduino frameworks
#include <SPI.h>                        // spi comm for arduino
#include <MFRC522.h>                    // library for RFID card reader
#include <Servo.h>                      // library for controlling servo

/* Header file */
#include "LCDI2C.h"
#include "WIFI.h"
#include "RFID.h"
#include "GATE.h"

/* Global var */
LiquidCrystal_I2C lcd(0x27);                            // set the lcdi2c address t0 0x27
const char* ssid = "arduino-5GHz@unifi";                // wifi ssid
const char* password = "fatimahz";                      // wifi password
const char* mqtt_server = "broker.mqtt-dashboard.com";  // broker that will be connected to esp-01
const int SS_PIN = 53;
const int RST_PIN = 5;

/* Callback function header */
void callback(char* topic, byte* payload, unsigned int length) 
{
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (int i=0;i<length;i++) {
        Serial.print((char)payload[i]);
    }
    Serial.println();
}

/* Instance */
LCDI2C lcdi2c;                      // for LCDI2C header file
WIFI wifi;                          // for WiFi header file
WiFiEspClient espClient;
PubSubClient client(espClient);
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
Servo gate;

void setup()
{
    Serial.begin(9600);                     // init serial comm 
    Serial1.begin(9600);                    // init serial comm for ESP01

    lcdi2c.initLCD(16,2);                   // init lcdi2c
    lcd.print("test");

    SPI.begin();                            // Initiate  SPI bus
    mfrc522.PCD_Init();                     // Initiate MFRC522

    wifi.initWifi();                        // connecting to wifi

    client.setServer(mqtt_server, 1883);    // setting mqtt server

    // TODO : set callback method after finish setting the mqtt broker
    // maybe set it in the main loop to avoid any error

    client.setCallback(callback);           // set callback when receiving message

    gate.attach(4);                         // init servo at pin 4
 
    // TODO : add option for debugging for debugging
}

void loop()
{
    // if client not connectd to mqtt broker, reconnect.
    if (!client.connected()) 
    {
        wifi.reconnect();
    }
    
    client.loop();



}

