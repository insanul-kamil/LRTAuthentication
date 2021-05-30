#include "LCDI2C.h"

void LCDI2C::initLCD(int column, int row)
{
    lcd.begin(column,row);
}