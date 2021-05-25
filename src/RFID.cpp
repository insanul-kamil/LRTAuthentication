#include "RFID.h"
#include "main.cpp"

String content = "";
char content_[15] = "";


bool RFID::readCardUID()
{
    // Look for new cards if there a card, pass this loop
    if ( !mfrc522.PICC_IsNewCardPresent()) 
    {
        return;
    }
    // Select one of the cards, if the card are readable, pass this loop
    if ( !mfrc522.PICC_ReadCardSerial()) 
    {
        return;
    }

    //Show UID on serial monitor
    Serial.print("UID tag :");

    String content = "";
    
    for (byte i = 0; i < mfrc522.uid.size; i++) 
    {
        Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
        Serial.print(mfrc522.uid.uidByte[i], HEX);
        content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
        content.concat(String(mfrc522.uid.uidByte[i], HEX));
    }

    Serial.println();

    Serial.print("Message : ");
    content.toUpperCase();

    // TODO : check data if same as in the database

    if ( content.substring(1) == "BD 31 15 2B") //change here the UID of the card/cards that you want to give access
    {
        Serial.println("Authorized access");
        Serial.println();

        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Authorized");

        return true;

    }
 
    else   
    {
        Serial.println(" Access denied");

        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Denied");
        
        return false;
    }
}

char* RFID::getUID()
{
    content.toCharArray(content_ ,15);

    return content_;
}
