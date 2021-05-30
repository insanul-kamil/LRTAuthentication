#include "BUZZZER.h"

void BUZZER::initBuzzer(int buzzerPin)
{
    pinMode(buzzer,buzzerPin);
}

void BUZZER::trueSound()
{
    tone(buzzer, 1000);

    currentMillis = millis();  //get the current "time" (actually the number of milliseconds since the program started)
    
    if (currentMillis - startMillis >= period)  //test whether the period has elapsed
    {
        noTone(buzzer);
        
        startMillis = currentMillis;  //IMPORTANT to save the start time of the current LED state.
    
    }
}

void BUZZER::falseSound()
{
    tone(buzzer, 4000);

    currentMillis = millis();  //get the current "time" (actually the number of milliseconds since the program started)
    
    if (currentMillis - startMillis >= period)  //test whether the period has elapsed
    {
        noTone(buzzer);
        
        startMillis = currentMillis;  //IMPORTANT to save the start time of the current LED state.
    
    }
}