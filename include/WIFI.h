#ifndef WIFI_H
#define WIFI_H

#include <Arduino.h>
#include <PubSubClient.h>               // library for publish and subscribe
#include <WiFiEsp.h>                    // library for setting up esp 01 as client at arduino frameworks
#include <Wire.h>                       // pre-installed library
#include <LiquidCrystal_I2C.h>          // library for lcdi2c

extern const char* ssid;                // wifi ssid
extern const char* password;                      // wifi password
extern const char* mqtt_server;
extern int statusWiFi;
extern char server[];

extern LiquidCrystal_I2C lcd;
extern WiFiEspClient espClient;
extern PubSubClient client;

class WIFI
{
    public:
        // connect to wifi
        void initWifi();
        // loop to reconnect to mqtt broker if disconnected
        void reconnect();
        /*  arguments(char name, char stat)
            post data to database*/
        void postToDB(char name, bool stat);
    private:
        char UID;
};


#endif