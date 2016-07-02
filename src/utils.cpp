#include "utils.h"

void RefreshLCD(LCD_DISCO_F469NI *lcd)
{
    // LCD REFRESH IN CASE OF GLITCHES
    lcd->DisplayOff();
    lcd->DisplayOn();
    lcd->Clear(LCD_COLOR_BLACK);
}
