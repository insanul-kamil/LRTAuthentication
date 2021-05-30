#ifndef BUZZER_H
#define BUZZER_H

#include <Arduino.h>

extern unsigned long startMillis;                              //some global variables available anywhere in the program
extern unsigned long currentMillis;
extern const unsigned long period;

extern const int buzzer;                                   // buzzer pin

class BUZZER
{
    public:
        void initBuzzer(const int buzzerPin);
        void trueSound();
        void falseSound();

    private:
};


#endif