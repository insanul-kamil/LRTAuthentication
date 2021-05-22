#include "LCDI2C.h"
#include "main.cpp"

void LCDI2C::initLCD(int column, int row)
{
    lcd.begin(column,row);
}