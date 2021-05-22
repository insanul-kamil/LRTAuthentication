#include "RFID.h"
#include "main.cpp"

void RFID::detectCard()
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

}

// TODO :  i need to find a way to get UID as a char, and then get the MFRC522 UID 
// by using maybe 
/*
  Serial.print(F("Card UID:"));
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  } 
  Serial.println();

*/

char RFID::getUIDTag()
{
    //Show UID on serial monitor
    Serial.print("UID tag :");

    for (byte i = 0; i < mfrc522.uid.size; i++) 
    {
        // u need to fix here

    }
    Serial.println();

    return UID;
}

bool RFID::isAuthorized()
{
    Serial.print("Message : ");
    UID.toUpperCase();

    if ( UID.substring(1) == "BD 31 15 2B") //change here the UID of the card/cards that you want to give access
    {
        Serial.println("Authorized access");
        Serial.println();
        
        stat = true;

        delay(3000);
    }
 
    else   
    {
        Serial.println(" Access denied");
        delay(3000);

        stat = false;
    
    }
    return stat;
}