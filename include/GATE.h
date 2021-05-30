#ifndef GATE_H
#define GATE_H

#include <Arduino.h>
#include <Servo.h>                      // library for controlling servo

extern Servo servo;                     // servo instances

class GATE
{
    public:
        void gateOpen();            // to open the gate for passenget to enter
        void gateClose();           // closed position
        
    private:
        int pos = 0;
};


#endif