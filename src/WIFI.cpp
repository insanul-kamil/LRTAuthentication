#include "WIFI.h"

void WIFI::initWifi()
{
    delay(10);          // optional delay

    Serial.println("initializing WiFi module...");

    WiFi.init(&Serial1);

    // check for the presence of the shield
    if (WiFi.status() == WL_NO_SHIELD)              
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

    // attempt to connect to WiFi network
    while (statusWiFi != WL_CONNECTED)           
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
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.println();

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("WiFi connected");
}

// loop to reconnect to mqtt broker if disconnected
void WIFI::reconnect()            
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

        const char * clientId = "InsanulKamil";

        // Attempt to connect
        if (client.connect(clientId))
        {
            //TODO: update subscribed topic
            Serial.println("connected");

            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Connected to");
            lcd.setCursor(0,1);
            lcd.print("MQTT broker");

            //client.publish("outTopic", "hello world");

            // resubscribe
            client.subscribe("lrt/order/location");
            client.subscribe("lrt/passenger/app/user");

        } else 
        {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");

            delay(5000);
        }
    }
}

void WIFI::postToDB()
{
    // Connect to the server (your computer or web page)  
    if (espClient.connect(server, 80)) 
    {
        espClient.print("GET /write_data.php?"); // This
        espClient.print("value="); // This
        espClient.print(photocellReading); // And this is what we did in the testing section above. We are making a GET request just like we would from our browser but now with live data from the sensor
        espClient.println(" HTTP/1.1"); // Part of the GET request
        espClient.println("Host: 192.168.0.11"); // IMPORTANT: If you are using XAMPP you will have to find out the IP address of your computer and put it here (it is explained in previous article). If you have a web page, enter its address (ie.Host: "www.yourwebpage.com")
        espClient.println("Connection: close"); // Part of the GET request telling the server that we are over transmitting the message
        espClient.println(); // Empty line
        espClient.println(); // Empty line
        espClient.stop();    // Closing connection to server

    }else 
    {
        // If Arduino can't connect to the server (your computer or web page)
        Serial.println("--> connection failed\n");
    }

    // WARNING = you need to give a delay before posting it to the database
    // or else the server might not capture it.
}