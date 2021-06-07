#ifndef FINGERPRINT_H
#define FINGERPRINT_H

#include <Arduino.h>
#include <Adafruit_Fingerprint.h>

extern bool detectedFinger;

class FINGERPRINT
{
    public:
        void InitFingerprint();
        // returns -1 if failed, otherwise returns ID #
        int getFingerprintID();
        
    private:

};
#endif