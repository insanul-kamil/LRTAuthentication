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
#include <Vector.h>                     // library to use vector class

/* Header file */
#include "LCDI2C.h"
#include "WIFI.h"
#include "RFID.h"
#include "GATE.h"
#include "BUZZZER.h"
#include "USER.h"
#include "ACCESS.h"
#include "FINGERPRINT.h"

/* Global var */
unsigned long startMillis;
unsigned long currentMillis;
const char* ssid = "arduino@unifi";                     // wifi ssid
const char* password = "fatimahz";                      // wifi password
const char* mqtt_server = "broker.mqtt-dashboard.com";  // broker that will be connected to esp-01
const int buzzer = 8;                                   // buzzer pin
const int SS_PIN = 53;                                  // mfrc522 ss pin
const int RST_PIN = 5;                                  // mfrc522 rst pin
bool detectedCard = false;                              // variable to store if the card is detected
bool access = false;                                    // variable to store user access
int passengerCounter = 0;                               // var storing how many passenger in the lrt
int statusWiFi = WL_IDLE_STATUS;                        // var storing wifi status (const)
const int numUser = 4;                                  // var storing total num of user
char *temp_uid;                                         // temperary var
char *scannedUID;                                       // var storing card UID
char *username;                                         // var storing user name if his name exist in the databases
char buff;                                              // var storing response from serial2
bool detectedFaces;                                     // var storing if faces detected or not
int temp_var = 0;
char *receivedChars;
char lastChars[30] = "";
uint8_t fingerprintID;
bool detectedFinger;
int id_;

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
ACCESS userAccess;                  // instance for ACCESS header file
User userr;                         // instance for User header file 
FINGERPRINT fingerprint;            // instance for fingerprint

/* User */
User user0(0, "Insanul Kamil", "0F 36 FD C7", 0);
User user1(1, "Aniq Danial", "test", 1);
User user2(2, "Iman", "ims", 2);
User user3(3, "Azhar", "75 9F 7D C6", 3);
User user[numUser];
char *uid[numUser];
char *name[numUser];
int enterStatus[numUser];


void callback(char* topic, byte* payload, unsigned intlength)
{

}

char *faceConformation(int ID)
{
    detectedFaces = true;
    access = true;

    char* temp_name = userAccess.GetUserName(ID, name);   // get user name
    
    id_ = ID;
    enterStatus[ID] = 1;

    Serial.println();
    Serial.print(temp_name);
    Serial.print(" face detected");

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(temp_name);
    lcd.setCursor(0,1);
    lcd.print("Face detected");

    return temp_name;

}

char *fingerConformation(int ID)
{
    access = true;

    char* temp_name = userAccess.GetUserName(ID, name);   // get user name
    
    id_ = ID;
    enterStatus[ID] = 1;

    Serial.println();
    Serial.print(temp_name);
    Serial.print(" finger detected");

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(temp_name);
    lcd.setCursor(0,1);
    lcd.print("finger detected");

    return temp_name;
}

void setup()
{
    Serial.println("Starting initialization...");

    startMillis = millis();                     // record starting time

    Serial.begin(9600);                     // init serial comm for debugging
    Serial1.begin(115000);                    // init serial comm for ESP01
    
    Serial.println("Detecting Fingerprint...");
    fingerprint.InitFingerprint();          // init fingerprint. if fingerprint sensor not found, loop will stop here

    Serial.println("Starting LCD...");
    lcdi2c.initLCD(16,2);                   // init lcdi2c
    lcd.print("test");

    Serial.println("Starting MFRC522...");
    SPI.begin();                            // Initiate  SPI bus
    mfrc522.PCD_Init();                     // Initiate MFRC522

    wifi.initWifi();                        // connecting to wifi

    Serial.println("Connecting to MQTT server...");
    client.setServer(mqtt_server, 1883);    // setting mqtt server
    client.setCallback(callback);           // set callback when receiving message

    while (!client.connected())      
    {
        if(client.connect("InsanulKamil")) 
        {
            Serial.println("connected");
        }else{
            Serial.print("failed state ");
            Serial.print(client.state());
            delay(2000);
        }
    }

    servo.attach(4);                        // init servo at pin 4
    gate.gateClose();                       // start pos

    buzz.initBuzzer(buzzer);                // init buzzer

    Serial.println("Declaring user...");

    user[0] = user0;
    user[1] = user1;
    user[2] = user2;
    user[3] = user3;

    Serial.println("User:");
    for(int i = 0; i < numUser; i++)
    {
        // put this loop anywhere else because it's not suitable here
        temp_uid = user[i].getUID();
        uid[i] = temp_uid;
        Serial.print(i);

        name[i] = user[i].getName();
        Serial.println(name[i]);

    }
    // TODO : add option for debugging for debugging
    Serial.println("Initialization complete\n");
}


void loop()
{
    // if client not connectd to mqtt broker, reconnect.
    if (!client.connected()) 
    {
        wifi.reconnect();
        Serial.println("Connecting to broker...");
    }

    client.loop();

    id_ = 0;
    access = false;
    detectedFaces = false;
    *scannedUID = '\0';
    *receivedChars = '\0';
    fingerprintID = 0;
    detectedFinger = false;
    
    lcd.setCursor(0,0);
    lcd.print("   Welcome to");
    lcd.setCursor(0,1);
    lcd.print("     LRTJB!");

    userAccess.recvSerial();
    receivedChars = userAccess.getData();               // get data from serial monitor to be read
    detectedCard = rfid.readCard();                     // read rfid card. return true if rfid card detected
    fingerprintID = fingerprint.getFingerprintID();     // get fingerprint id from sensor

    if(fingerprintID < 20)
    {
        detectedFinger = true;
        
    }

    if(strcmp(receivedChars, "") != 0)
    {
        detectedFaces = true;
    }


    if( detectedFinger && detectedFaces == false && detectedCard == false)
    {
        switch (fingerprintID)
        {
        case 1:
            username = fingerConformation(0);
            break;
        case 2:
            username = fingerConformation(1);
            break;
        case 3:
            username = fingerConformation(2);
            break;
        case 4:
            username = fingerConformation(3);
            break;
        default:
            break;
        }
    }
    
    if(detectedFaces && detectedFinger == false && detectedCard == false)
    {
        if(strcmp(receivedChars, lastChars))    // loop for detecting duplicate of face id
        {
            strcpy(lastChars, receivedChars);
            
            // maybe this loop can be changed to forloop?
            if(strcmp(receivedChars, "1") == 0)         // face recognized = kamil
            {
                username = faceConformation(0);

            }else if(strcmp(receivedChars, "2") == 0)
            {
                username = faceConformation(1);

            }else if(strcmp(receivedChars, "3") == 0)
            {
                username = faceConformation(2);

            }else if(strcmp(receivedChars, "4") == 0)
            {
                username = faceConformation(3);

            }
        }     
    }

    if(detectedCard && detectedFaces == false && detectedFinger == false)                  //
    {
        scannedUID = rfid.getUID();                             // getting uid that have been scanned
        access = userAccess.userExist(scannedUID, uid, name);   // getting access status
        username = userAccess.getUserName();                    // getting user name

    }

    if(access)
    {
        if(enterStatus[id_] == 1)
        {
            buzz.trueSound();
            passengerCounter--;

            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Have a nice day");
            lcd.setCursor(0,1);
            lcd.print(username);

            Serial.print("\nOUT - Authorized\n");
            delay(3000);
        }else
        {
            buzz.trueSound();
            passengerCounter++;
            temp_var = 0;

            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("    Welcome");
            lcd.setCursor(0,1);
            lcd.print(username);

            Serial.print("\nIN - Authorized\n");
            delay(3000);            // maybe change this delay to a beep or delay with a beep
            
            // TODO: push passengerCounter to mqtt 
            // TODO: add func to add data to database
        }
        gate.gateOpen();
        lcd.clear();

    }else
    {
        gate.gateClose();
        temp_var = 0;
    }
}
