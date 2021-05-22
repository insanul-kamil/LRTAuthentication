#include "WIFI.h"
#include "main.cpp"

void WIFI::initWifi()
{
    delay(10);

    // start to connect to a wifi network
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("connecting to");
    lcd.setCursor(0,1);
    lcd.print(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("WiFi connected");
}

void reconnect()            // loop to reconnect to mqtt broker if disconnected
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