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

extern LiquidCrystal_I2C lcd;
extern WiFiEspClient espClient;
extern PubSubClient client;

class WIFI
{
    public:
        void initWifi();            // connect to wifi
        void reconnect();           // loop to reconnect to mqtt broker if disconnected

    private:
        char UID;
};


#endif