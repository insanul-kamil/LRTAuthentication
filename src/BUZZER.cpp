#include "BUZZZER.h"

void BUZZER::initBuzzer(int buzzerPin)
{
    pinMode(buzzer,buzzerPin);
}

void BUZZER::trueSound()
{
    tone(buzzer, 1000);
    delay(500);
    noTone(buzzer);
}

void BUZZER::falseSound()
{
    tone(buzzer, 4000);
    delay(500);
    noTone(buzzer);
}