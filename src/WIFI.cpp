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
    Serial.print(WiFi.localIP());
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

        const char * clientId = "hasdfkasdfa";

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
            
            // resubscribe
            client.subscribe("lrt/order/user");
            client.subscribe("lrt/order/currentPlace");
            client.subscribe("lrt/order/goingPlace");
            client.subscribe("lrt/order/confirmation");
            client.subscribe("lrt/order/status");
            
        } else 
        {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");

            delay(5000);
        }
    }
}

/*  arguments(char name, char stat)
    post data to database*/
void WIFI::postToDB(char name,bool stat)
{
    // Connect to the server (your computer or web page)  
    if (espClient.connect(server, 80)) 
    {
        espClient.print("GET /write_data.php?");
        espClient.print("name=");
        espClient.print(name);
        espClient.print(";");
        espClient.print("stat=");
        espClient.print(stat);
        espClient.println(" HTTP/1.1"); 
        espClient.println("Host: 192.168.0.184");
        espClient.println("Connection: close"); 
        espClient.println();
        espClient.println(); 
        espClient.stop();    // Closing connection to server

    }else 
    {
        // If Arduino can't connect to the server (your computer or web page)
        Serial.println("--> connection failed\n");
    }

    // WARNING = you need to give a delay before posting it to the database
    // or else the server might not capture it.
}