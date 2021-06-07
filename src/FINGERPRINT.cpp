#include "FINGERPRINT.h"

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&Serial2);

void FINGERPRINT::InitFingerprint()
{
    finger.begin(57600);
    delay(5);
    if (finger.verifyPassword()) {
        Serial.println("Found fingerprint sensor!");
    } else {
        Serial.println("Did not find fingerprint sensor :(");
        while (1) { delay(1); }
    }

    finger.getTemplateCount();

    if (finger.templateCount == 0) {
        Serial.print("Sensor doesn't contain any fingerprint data. Please run the 'enroll' example.");
    }
    else {
        Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
    }
}
int FINGERPRINT::getFingerprintID() 
{
    int8_t p = finger.getImage();
    if (p != FINGERPRINT_OK)  return 20;

    p = finger.image2Tz();
    if (p != FINGERPRINT_OK)  return 20;

    p = finger.fingerFastSearch();
    if (p != FINGERPRINT_OK)  return 20;

    // found a match!
    Serial.print("Found ID #"); Serial.print(finger.fingerID);
    Serial.print(" with confidence of "); Serial.println(finger.confidence);
    return finger.fingerID;
}