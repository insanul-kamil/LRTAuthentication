/* main.cpp made by Muhammad Insanul Kamil bin Syaidul Azam, 21/5/2021 */

/* Pinout*/
/* ESP-01 */
/* flashing (programming mode)  normal*/
/* GND --- gnd                  Vcc --- 3.3V*/
/* gp2 --- not connected        GND --- GND */
/* gp0 --- gnd                  Rx --- Tx*/
/* Rx --- Rx                    Tx --- Tx*/
/* Rx --- Rx                    Ch_EN --- 3.3v*/
/* CH_PD --- 3.3v               */
/* RST --- not connected        */
/* Vcc --- 3.3v                 */
/* when gpio-0 is grounded, ESP will be set in programming mode*/
/* disconnect gpio-0 to let it run normally.*/
/**/
/* MFRC522 */
/**/
/**/
/**/
/**/
/**/
/**/
/**/
/**/

/* Libraries */
#include <Arduino.h>
#include <Wire.h>                       // pre-installed library
#include <Adafruit_Sensor.h>            // this library is required to use dht sensor
#include "DHT.h"                        // library for recording temp and humidity
#include <SPI.h>                        // spi comm for arduino

/* Header file */
#include "LCDI2C.h"
#include "WIFI.h"
#include "RFID.h"
#include "GATE.h"
#include "BUZZZER.h"
#include "USER.h"

/* Instance */
WiFiEspClient espClient;            // esp-01 instance
PubSubClient client(espClient);     // pubseb client

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);        // set the lcdi2c address t0 0x27
LCDI2C lcdi2c;                      // for LCDI2C header file

Servo servo;                        // servo instance
GATE gate;                          // gate instance(servo header file)

BUZZER buzz;                        // buzzer instance(buzzer header file)

WIFI wifi;                          // for WiFi header file

RFID rfid;                          // for RFID header file

/* Global var */
unsigned long startMillis;                              //some global variables available anywhere in the program
unsigned long currentMillis;
const unsigned long period = 1000;                      //the value is a number of milliseconds
const char* ssid = "arduino@unifi";                // wifi ssid
const char* password = "fatimahz";                      // wifi password
const char* mqtt_server = "broker.mqtt-dashboard.com";  // broker that will be connected to esp-01
const int buzzer = 8;                                   // buzzer pin
const int SS_PIN = 53;                                  // mfrc522 ss pin
const int RST_PIN = 5;                                  // mfrc522 rst pin
bool access = false;                                    // variable to store user access
int passengerCounter = 0;
int statusWiFi = WL_IDLE_STATUS;

void callback(char* topic, byte* payload, unsigned intlength)
{

}

void setup()
{
    startMillis = millis();                 // recording start time

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

    servo.attach(4);                        // init servo at pin 4
    gate.gateClose();                       // start pos

    buzz.initBuzzer(buzzer);                // init buzzer

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

    access = rfid.readCardUID();

    if(access)
    {
        buzz.trueSound();
        gate.gateOpen();

    }else{
        buzz.falseSound();
        gate.gateClose();

    }

}

