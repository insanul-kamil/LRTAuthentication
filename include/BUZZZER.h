#ifndef BUZZER_H
#define BUZZER_H

#include <Arduino.h>


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