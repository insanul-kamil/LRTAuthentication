#include "RFID.h"
#include "LCDI2C.h"

bool RFID::readCard()           // return true if there's card detected.
{
    memset(content_,0, sizeof(content_));
    content = "";

    // Look for new cards if there a card, pass this loop
    if ( !mfrc522.PICC_IsNewCardPresent()) 
    {
        return false;
    }

    // Select one of the cards, if the card are readable, pass this loop
    if ( !mfrc522.PICC_ReadCardSerial()) 
    {
        return false;
    }


    //Show UID on serial monitor
    Serial.print("UID tag :");
    
    for (byte i = 0; i < mfrc522.uid.size; i++) 
    {
        Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
        Serial.print(mfrc522.uid.uidByte[i], HEX);
        content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
        content.concat(String(mfrc522.uid.uidByte[i], HEX));
    }

    Serial.println();

    content.toUpperCase();
    content = content.substring(1);
    content.toCharArray(content_ ,content.length() + 1);

    return true;
}

char *RFID::getUID()
{
    return content_;
}
