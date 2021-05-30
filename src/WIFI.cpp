#include "WIFI.h"

void WIFI::initWifi()
{
    delay(10);          // optional delay

    Serial.println("initializing WiFi module...");

    WiFi.init(&Serial1);

    if (WiFi.status() == WL_NO_SHIELD)              // check for the presence of the shield
    {
        Serial.println("WiFi shield not present");
        Serial.println("restart the program.");
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("ESP-01 not");
        lcd.setCursor(0,1);
        lcd.print("present");
        
        // don't continue
        while (true);
    }

    while (statusWiFi != WL_CONNECTED)           // attempt to connect to WiFi network
    {

        Serial.print("Attempting to connect to WPA SSID: ");
        Serial.println(ssid);

        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("connecting to");
        lcd.setCursor(0,1);
        lcd.print(ssid);
    
        // Connect to WPA/WPA2 network
        statusWiFi = WiFi.begin(ssid, password);

    }

    Serial.println("...");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("WiFi connected");
}

void WIFI::reconnect()            // loop to reconnect to mqtt broker if disconnected
{
    // Loop until we're reconnected to mqtt broker
    while (!client.connected()) 
    {
        Serial.print("Attempting MQTT connection...");

        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Attempting MQTT");
        lcd.setCursor(0,1);
        lcd.print("connection");

        // Create a random client ID
        String clientId = "ESP8266Client-";
        clientId += String(random(0xffff), HEX);

        // Attempt to connect
        if (client.connect(clientId.c_str()))
        {
            //TODO: update subscribed topic
            Serial.println("connected");

            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Connected to");
            lcd.setCursor(0,1);
            lcd.print("MQTT broker");

            // Once connected, publish an announcement...
            client.publish("outTopic", "hello world");

            // ... and resubscribe
            client.subscribe("greenBottles/#");

        } else 
        {
        Serial.print("failed, rc=");
        Serial.print(client.state());
        Serial.println(" try again in 5 seconds");

        // Wait 5 seconds before retrying
        delay(5000);
        }
    }
}