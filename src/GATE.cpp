#include "GATE.h"

extern GATE gate;                          // gate instance(servo header file)

void GATE::gateOpen()
{
    for (pos = 0; pos <= 180; pos += 1) {   // goes from 0 degrees to 180 degrees
        
        // in steps of 1 degree
        servo.write(pos);                   // tell servo to go to position in variable 'pos'
        delay(15);                          // waits 15ms for the servo to reach the position
    }

    delay(3000);

    for (pos = 180; pos >= 0; pos -= 1) {   // goes from 180 degrees to 0 degrees
        
        servo.write(pos);                   // tell servo to go to position in variable 'pos'
        delay(15);                          // waits 15ms for the servo to reach the position
    }
}

void GATE::gateClose()
{
    servo.write(0);
}
