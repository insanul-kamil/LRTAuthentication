/* main.cpp made by Muhammad Insanul Kamil bin Syaidul Azam, 21/5/2021 */

/* Pinout*/
/* ESP-01 */
/* flashing (programming mode)  normal*/
/* GND --- gnd                  Vcc --- 3.3V*/
/* gp2 --- not connected        GND --- GND */
/* gp0 --- gnd                  Rx --- Tx*/
/* Rx --- Rx                    Tx --- Rx*/
/* Rx --- Rx                    Ch_EN --- 3.3v*/
/* CH_PD --- 3.3v               */
/* RST --- not connected        */
/* Vcc --- 3.3v                 */
/* when gpio-0 is grounded, ESP will be set in programming mode*/
/* disconnect gpio-0 to let it run normally.*/
/* if wifi didn't connect, it maybe because of Serial2 is busy.*/
/* try to restart the arduino.*/
/* MFRC522 */
/**/
/**/
/**/
/**/
/**/
/**/
/**/
/**/
/*  topic to publish

    MONITOR/COMMUNITY
    - monitor/temp (status)
    - monitor/humidity (status)
    - monitor/co2 (status)

    LRT(BOOKING SYSTEM)
    - lrt/user (order-subscribe)
    - lrt/passenger/counter (status)
    - lrt/passenger/status (status)
    - lrt/order/currentPlace (order-subscribe)
    - lrt/order/goingPlace (order-subscribe)
    - lrt/order/confirmation (order-subscribe)
    - lrt/order/status (16 - order - publish)


    PARKING MONITOR

    what to publish to mqtt
    - passenger counter

    what to post to db
    - passenger history everytime user enter(user, where to where, time)
    - light usage(optional)

    what we need to callback
    (maybe switches)
    - name order 
    - place order
    
    parking monitor abstract
	this sketch will share the vacancy of any parking, and wil allow user to plan
	where to park. this will save user a lot of time.

	maybe user can order the parkin?


	Component:
	- Arduino Uno
	- IR Sensor x5
	- wifi esp-01
	- servo

	Order:
	parkingM/park/p1
	parkingM/park/p2
	parkingM/park/counter
	parkingM/park/desc

	parkingM/order/parkinput(sub)
	parkingM/order/name(sub)
	parkingM/order/confirmation(sub)
*/

/* Libraries */
#include <Arduino.h>
#include <Wire.h>            // pre-installed library
#include <SPI.h>             // spi comm for arduino
#include <Vector.h>          // library to use vector class
#include <Servo.h>           // servo lib
#include <SimpleDHT.h>

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
#define max_digit 5 
unsigned long startMillis;
unsigned long startMillis1;
const int mq7 = 9;
unsigned long currentMillis;
const unsigned long period = 200;              //the value is a number of milliseconds
const char *ssid = "arduino@unifi";            // wifi ssid
const char *password = "fatimahz";             // wifi password
const char *mqtt_server = "broker.hivemq.com"; // broker that will be connected to esp-01
const int buzzer = 8;                          // buzzer pin
const int SS_PIN = 53;                         // mfrc522 ss pin
const int RST_PIN = 5;                         // mfrc522 rst pin
char server[] = "192.168.0.184";
bool detectedCard = false;       // variable to store if the card is detected
bool access = false;             // variable to store user access
int passengerCounter = 0;        // var storing how many passenger in the lrt
int statusWiFi = WL_IDLE_STATUS; // var storing wifi status (const)
const int numUser = 4;           // var storing total num of user
char *temp_uid;                  // temperary var
char *scannedUID;                // var storing card UID
char *username;                  // var storing user name if his name exist in the databases
char buff;                       // var storing response from serial2
bool detectedFaces;              // var storing if faces detected or not
int temp_var = 0;
char *receivedChars;
char lastChars[30] = "";
uint8_t fingerprintID;
bool detectedFinger;
int id_;
char num_char[max_digit + sizeof(char)];
char buffer[3];
int bufff;
int inFlag = false;
int outFlag = false;
int pos = 0;
int parkCounter = 0;
const int irOUT = 40;
const int ir1 = 42;
const int ir2 = 43;
const int irIN = 44;
const int led1 = 2;
const int led2 = 3;
const int led3 = 7;
const int parkServo = 4;
const int DHT = 8;
const int MQ7 = 9;
int counter = 0;
int lastINData = 1;
int lastOUTData = 1;
int currentData = 1;
int park1 = 1;
int park2 = 2;
int lastP1Data = 1;
int lastP2Data = 1;
bool parkConformationFlag = 0;
const int period1 = 2000;
int pinDHT11 = 8;
SimpleDHT11 dht11(pinDHT11);
int mq7Input = 0;

/* Instance */
WiFiEspClient espClient;                                       // esp-01 instance
PubSubClient client(espClient);                                // pubseb client
MFRC522 mfrc522(SS_PIN, RST_PIN);                              // Create MFRC522 instance.
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); // set the lcdi2c address t0 0x27
LCDI2C lcdi2c;                                                 // for LCDI2C header file
BUZZER buzz;                                                   // buzzer instance(buzzer header file)
WIFI wifi;                                                     // for WiFi header file
RFID rfid;                                                     // for RFID header file
ACCESS userAccess;                                             // instance for ACCESS header file
User userr;                                                    // instance for User header file
FINGERPRINT fingerprint;                                       // instance for fingerprint
Servo servo;

/* User */
User user0(0, "Insanul Kamil", "0F 36 FD C7", 0);
User user1(1, "Aniq Danial", "test", 1);
User user2(2, "Iman", "ims", 2);
User user3(3, "Azhar", "75 9F 7D C6", 3);
User user[numUser];
char *uid[numUser];
char *name[numUser];
/* this is for detecting which user who in the lrt atm.
    1 == in, 0 == out*/
bool enterStatus[numUser];
// flag for user
bool userInput;
// for storing temp id from mqtt
int userID = 0;
int currentPlaceID = 0;
int parkingID = 0;
int goingPlaceID = 0;
bool confirmationFlag = false;
bool bookingFlag = false;

// receive and handle mqtt message
void callback(char *topic, byte *payload, unsigned int length)
{
    /*  this loop below can be simplyfied but i dont know why im not doing it
        topic that we need to use:
        - lrt/order/user (14 - order-subscribe)
        - lrt/order/currentPlace (22 - order-subscribe)
        - lrt/order/goingPlace (20 - order-subscribe)
        - lrt/order/confirmation (22 - order-subscribe)
        - lrt/order/status (16 - order - publish)

    */

    // terminating byte
    payload[length] = '\0';
    // converting byte to char
    // detecting if topic is equal to "lrt/order/user", then check for the
    // payload associted with them confirm the user
    if (strcmp(topic, "lrt/order/user") == 0)
    {
        // if payload length equal 0, it means there is no payload received.
        // else payload = orderID
        if (length == 0)
        {
            Serial.println("No Payload sent.");
        }
        else
        {
            bufff = payload[0];
            userID = bufff - 48;
            Serial.print(userID);
        }
    }

    // if topic is equal to "lrt/order/currentPlace", then check the payload.
    if (strcmp(topic, "lrt/order/currentPlace") == 0)
    {
        // if payload length equal 0, it means there is no payload received.
        // else payload = currentPlaceID
        if (length == 0)
        {
            Serial.println("No Payload sent.");
        }
        else
        {
            bufff = payload[0];
            currentPlaceID = bufff - 48;
            Serial.print(currentPlaceID);
        }
    }

    // if topic is equal to "lrt/order/goingPlace", then check the payload.
    if (strcmp(topic, "lrt/order/goingPlace") == 0)
    {
        // if payload length equal 0, it means there is no payload received.
        // else payload = goingPlaceID
        if (length == 0)
        {
            Serial.println("No Payload sent.");
        }
        else
        {
            bufff = payload[0];
            goingPlaceID = bufff - 48;
            Serial.print(goingPlaceID);
        }
    }

    // if topic is equal to "lrt/order/confirmation", then check the payload.
    if (strcmp(topic, "lrt/order/confirmation") == 0)
    {
        // if payload length equal 0, it means there is no payload received.
        // else confirmation equal true
        if (length == 0)
        {
            Serial.println("No Payload sent.");
        }
        else
        {
            if (confirmationFlag == true)
            {
                confirmationFlag = false;
            }
            else if (confirmationFlag == false)
            {
                confirmationFlag = true;
            }
        }
        // debugging
        Serial.println(confirmationFlag);
    }

    if (strcmp(topic, "parkingM/order/confirmation") == 0)
    {
        // if payload length equal 0, it means there is no payload received.
        // else confirmation equal true
        if (length == 0)
        {
            Serial.println("No Payload sent.");
        }
        else
        {
            if (parkConformationFlag == true)
            {
                parkConformationFlag = false;
            }
            else if (parkConformationFlag == false)
            {
                parkConformationFlag = true;
            }
        }
        // debugging
        Serial.println(parkConformationFlag);
    }
    
    if (strcmp(topic, "parkingM/order/parkinput") == 0)
    {
        // if payload length equal 0, it means there is no payload received.
        // else payload = parkinput
        if (length == 0)
        {
            Serial.println("No Payload sent.");
        }
        else
        {
            bufff = payload[0];
            parkingID = bufff - 48;
            Serial.print(currentPlaceID);
        }
        // debugging
        Serial.println(parkingID);
    }

}

char *faceConformation(int ID)
{
    detectedFaces = true;
    access = true;

    char *temp_name = userAccess.GetUserName(ID, name); // get user name

    id_ = ID;
    enterStatus[ID] = 1;

    Serial.println();
    Serial.print(temp_name);
    Serial.print(" face detected");

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(temp_name);
    lcd.setCursor(0, 1);
    lcd.print("Face detected");

    return temp_name;
}

char *fingerConformation(int ID)
{
    access = true;

    char *temp_name = userAccess.GetUserName(ID, name); // get user name

    id_ = ID;
    enterStatus[ID] = 1;

    Serial.println();
    Serial.print(temp_name);
    Serial.print(" finger detected");

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(temp_name);
    lcd.setCursor(0, 1);
    lcd.print("finger detected");

    return temp_name;
}

void setup()
{
    Serial.println("Starting initialization...");

    // record starting time
    startMillis = millis();

    // init serial comm for debugging
    Serial.begin(9600);
    // init serial comm for ESP01
    Serial1.begin(115000);

    // init fingerprint. if fingerprint sensor not found, loop will stop here
    Serial.println("Detecting Fingerprint...");
    fingerprint.InitFingerprint();

    // init lcdi2c
    Serial.println("Starting LCD...");
    lcdi2c.initLCD(16, 2);
    lcd.print("test");

    // Initiate  SPI bus
    Serial.println("Starting MFRC522...");
    SPI.begin();

    //	Initiate IRsensor
    Serial.println("Initializing ir and led...");
    pinMode(ir1, INPUT);
    pinMode(ir2, INPUT);
    pinMode(irIN, INPUT);
    pinMode(irOUT, INPUT);

    //	initiate led
    pinMode(led1, OUTPUT);
    pinMode(led2, OUTPUT);
    pinMode(led3, OUTPUT);

    //  initiate dht and mq7

    // Initiate MFRC522
    mfrc522.PCD_Init();

    // connecting to wifi
    wifi.initWifi();

    Serial.println("Connecting to MQTT server...");
    // setting mqtt server
    client.setServer(mqtt_server, 1883);
    // set callback when receiving message
    client.setCallback(callback);
    // if client is not connecttd to broker, connect
    while (!client.connected())
    {
        if (client.connect("InsanulKamil"))
        {
            Serial.println("Connected to mqtt broker");
        }
        else
        {
            Serial.print("failed to connect");
            Serial.print(client.state());
            delay(2000);
        }
    }
    /*  this loop below can be simplyfied but i dont know why im not doing it
        topic that we need to use:
        - lrt/order/user (14 - order-subscribe)
        - lrt/order/currentPlace (22 - order-subscribe)
        - lrt/order/goingPlace (20 - order-subscribe)
        - lrt/order/confirmation (22 - order-subscribe)
        - lrt/order/status (16 - order - publish)

    */
    client.subscribe("lrt/order/user");
    client.subscribe("lrt/order/currentPlace");
    client.subscribe("lrt/order/goingPlace");
    client.subscribe("lrt/order/confirmation");
    client.subscribe("lrt/order/status");
    client.subscribe("parkingM/order/parkinput");
    client.subscribe("parkingM/order/name");
    client.subscribe("parkingM/order/confirmation");

    client.publish("lrt/passenger/counter", "0");
    client.publish("lrt/passenger/status", "Empty");

    servo.attach(parkServo); // init servo at pin 4

    buzz.initBuzzer(buzzer); // init buzzer

    Serial.println("Declaring user...");

    user[0] = user0;
    user[1] = user1;
    user[2] = user2;
    user[3] = user3;

    Serial.println("User:");
    for (int i = 0; i < numUser; i++)
    {
        // put this loop anywhere else because it's not suitable here
        temp_uid = user[i].getUID();
        uid[i] = temp_uid;
        Serial.print(i);

        name[i] = user[i].getName();
        Serial.println(name[i]);
    }

    Serial.println("Initialization complete\n");
}

// todo: implement buzzer
void loop()
{
    // if client disconnected to mqtt broker, reconnect.

    if (!client.connected())
    {
        wifi.reconnect();
    }

    client.loop();

    id_ = 0;
    access = false;
    detectedFaces = false;
    *scannedUID = '\0';
    *receivedChars = '\0';
    fingerprintID = 0;
    detectedFinger = false;
    bufff = 0;
    //parkConformationFlag = false;

    lcd.setCursor(0, 0);
    lcd.print("   Welcome to  ");
    lcd.setCursor(0, 1);
    lcd.print("     LRTJB!");

    userAccess.recvSerial();
    // get data from serial monitor to be read. conforming face detected.
    receivedChars = userAccess.getData();
    // read rfid card. return true if rfid card detected
    detectedCard = rfid.readCard();
    // get fingerprint id from sensor
    fingerprintID = fingerprint.getFingerprintID();

    inFlag = digitalRead(irIN);
    outFlag = digitalRead(irOUT);

    park1 = digitalRead(ir1);
    park2 = digitalRead(ir2);
        


    currentMillis = millis();

    if (currentMillis - startMillis1 >= period1)  //test whether the period has elapsed
    {
        byte temperature = 0;
        byte humidity = 0;
        int err = SimpleDHTErrSuccess;

        if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) 
        {
            return;
        }

        mq7Input = analogRead(mq7);
        
        sprintf(buffer, "%d", temperature);
        client.publish("monitor/temp",buffer );

        sprintf(buffer, "%d", humidity);
        client.publish("monitor/humidity", buffer);

        sprintf(buffer, "%d", mq7Input);
        client.publish("monitor/co2", buffer);

        startMillis1 = currentMillis;
    }

    if (currentMillis - startMillis >= period)
    {
        // loop to check any duplicated content
        //  if duplicate detected, passthis loop.
        if (lastINData == inFlag)
        {
            // pass
        }
        else
        {
            // if the var is not the same, do something here
            // there should be a door here but there is no door because
            // the delay will mess the code up
            if (lastINData == 0 && inFlag == 1 )
            {
                Serial.print(parkCounter);
                parkCounter++;
                sprintf(buffer, "%d", parkCounter);
                client.publish("parkingM/park/counter",buffer);

            }
        }
        // for out ir sensor
        if (lastOUTData == outFlag)
        {// pass
        }
        else
        {
            // if the var is not the same, do something here
            // there should be a door here but there is no door because
            // the delay will mess the code up
            if (lastOUTData == 0 && outFlag == 1 )
            {
                Serial.print(parkCounter);
                parkCounter--;
                sprintf(buffer, "%d", parkCounter);
                client.publish("parkingM/park/counter",buffer);
            }
        }

        if (lastP1Data == park1)
        {// pass
            if (lastP1Data  == 0)
            {
                client.publish("parkingM/park/p1","on");
            }
        }
        else
        {
            
            if (lastP1Data == 0 && park1 == 1 )
            {
                client.publish("parkingM/park/p1", "off");
            }
        }
        
        if (lastP2Data == park2)
        {// pass
            if (lastP2Data  == 0)
            {
                client.publish("parkingM/park/p2","on");
            }
        }
        else
        {
            if (lastP2Data == 0 && park2 == 1 )
            {
                client.publish("parkingM/park/p2", "off");
            }
        }
        
        lastP1Data = park1;
        lastP2Data = park2;
        lastOUTData = outFlag;
        lastINData = inFlag;
        startMillis = currentMillis;
    }
 
    if (confirmationFlag == true)
    {
        // making flag for detecting if the form is fully filled
        // should i make a flag or i shoudl just do it down here?
        if (userID > -1 && currentPlaceID > -1 && goingPlaceID > -1 && confirmationFlag == true)
        {
            bookingFlag = true;
        }
        else
        {
            // show status msg here
            bookingFlag = false;
        }

        // confirmation loop need to get all the data and process.
        // check if all the data is received, else booking failed
        if (bookingFlag == true)
        {
            client.publish("lrt/order/status", "Order Successful");
        }
        else
        {
            client.publish("lrt/order/status", "Order Failed - fill the form");
        }
    }

    if (parkConformationFlag == true)
    {
        client.publish("parkingM/order/status", "Order confirmed.");
        parkConformationFlag = false;
    }
    

    /*  faces id will be retrieved from serial monitor.
        if number that received from serial monitor is not equal to nothing,
        it means there is there is user.*/
    if (strcmp(receivedChars, "") != 0)
    {
        detectedFaces = true;
    }
    
    /*  fingerprint id will be retrieved from sensor range of 1 - 19.
        if fingerprint that detected is below 19, 
        that means there is user. 
        20 or above meaning user not exist in database */
    if (fingerprintID < 20)
    {
        detectedFinger = true;
    }

    // FINGER
    // getting detected finger username.
    if (detectedFinger && detectedFaces == false && detectedCard == false)
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
        id_ = fingerprintID - 1;

        if (enterStatus[id_] == true)
        {
            enterStatus[id_] = false;
        }
        else if (enterStatus[id_] == false)
        {
            enterStatus[id_] = true;
        }
    }

    // FACES
    // getting detected number from serial to get username.
    if (detectedFaces && detectedFinger == false && detectedCard == false)
    {
        // loop for detecting duplicate of face id
        // i think there is a problem here
        if (strcmp(receivedChars, lastChars))
        {
            strcpy(lastChars, receivedChars);

            // this loop need to be improve because it's counter intuitive
            if (strcmp(receivedChars, "1") == 0) // face recognized = kamil
            {
                username = faceConformation(0);
                id_ = 0;
            }
            else if (strcmp(receivedChars, "2") == 0)
            {
                username = faceConformation(1);
                id_ = 1;
            }
            else if (strcmp(receivedChars, "3") == 0)
            {
                username = faceConformation(2);
                id_ = 2;
            }
            else if (strcmp(receivedChars, "4") == 0)
            {
                username = faceConformation(3);
                id_ = 3;
            }

            if (enterStatus[id_] == true)
            {
                enterStatus[id_] = false;
            }
            else if (enterStatus[id_] == false)
            {
                enterStatus[id_] = true;
            }

            /*  this loop is for setting the var that will be user in access loop.*/
        }
    }

    // CARD
    // ampersand is used too detecting duplicate sensor usage.
    // if multiple sensor usage detected at the same time,
    // it will pass this function.
    if (detectedCard && detectedFaces == false && detectedFinger == false)
    {
        // getting uid that have been scanned
        scannedUID = rfid.getUID();
        // getting access status
        access = userAccess.userExist(scannedUID, uid, name);
        // getting user name
        username = userAccess.getUserName();
        // id_ is used to confirm the user entered the lrt
        id_ = userAccess.getUserID();

        if (enterStatus[id_] == true)
        {
            enterStatus[id_] = false;
        }
        else if (enterStatus[id_] == false)
        {
            enterStatus[id_] = true;
        }

        /*  this loop is for setting the var that will be user in access loop.*/
    }

    /*  at the access loop, do this:
        - open gate
        - counter++
        - mqtt publish(counter)
        - update db person history(name)
        - buzzer sound*/
    if (access)
    {
        // this loop is to detect the user is out from lrt
        // if user in the lrt, enterstat == 0, bye
        // if user out the lrt, enterstat == 1, hi
        if (enterStatus[id_] == 0)
        {
            buzz.trueSound();
            passengerCounter--;

            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Have a nice day");
            lcd.setCursor(0, 1);
            lcd.print(username);

            Serial.print("\nOUT - Authorized\n");
            delay(3000);
        }
        if (enterStatus[id_] == 1)
        {
            buzz.trueSound();
            passengerCounter++;
            temp_var = 0;

            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("    Welcome");
            lcd.setCursor(0, 1);
            lcd.print(username);

            Serial.println("\nIN - Authorized\n");
            delay(3000); // maybe change this delay to a beep or delay with a beep

            // what data to post?
            // name.
            //wifi.postToDB(*username, enterStatus[id_]);
        }
        Serial.print(id_);

        /*  if user detected scanning card or anything,
            get name,time and post to the log table*/
        /* this loop is for publishing lrt status */
        if (passengerCounter == 0)
        {
            client.publish("lrt/passenger/status", "Empty");
        }
        else if (passengerCounter < 3 && passengerCounter > 0)
        {
            client.publish("lrt/passenger/status", "Uncongested");
        }
        else if (passengerCounter < 6 && passengerCounter > 4)
        {
            client.publish("lrt/passenger/status", "Semi-full");
        }
        else if (passengerCounter < 11 && passengerCounter > 5)
        {
            client.publish("lrt/passenger/status", "Congested");
        }
        lcd.clear();

        sprintf(num_char, "%d", passengerCounter);
        client.publish("lrt/passenger/counter", num_char);
    }
    else
    {
        temp_var = 0;
    }
}
