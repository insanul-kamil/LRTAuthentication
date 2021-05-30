#ifndef LCDI2C_H
#define LCDI2C_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>          // library for lcdi2c

extern LiquidCrystal_I2C lcd;
class LCDI2C
{
    public:
        void initLCD(int column, int row);

    private:
};

#endif