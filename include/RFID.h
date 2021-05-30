#ifndef RFID_H
#define RFID_H

#include <Arduino.h>
#include <MFRC522.h>                        // library for RFID card reader

extern const int SS_PIN;               // mfrc522 ss pin
extern const int RST_PIN;               // mfrc522 rst pin

extern MFRC522 mfrc522;    // Create MFRC522 instance.

class RFID
{
    public:
        bool readCardUID();                 // return bool if theres card detected and can be readed
        char* getUID();

    private:
        char content_[15] = "";

};


#endif