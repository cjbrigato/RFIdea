#include "mbed.h"
#include "LCD_DISCO_F469NI.h"
#include "PN532.h"
#include "cube.h"
#include "nfc.h"

extern LCD_DISCO_F469NI lcd;
extern uint8_t text[60];

int nfcinit(PN532 *nfc)
{
    lcd.Clear(LCD_COLOR_BLACK);
    lcd.SetBackColor(LCD_COLOR_BLACK);
    lcd.SetTextColor(LCD_COLOR_WHITE);

    nfc->begin();
    wait(0.05);

    uint32_t versiondata = nfc->getFirmwareVersion();
    //NOLUCK
    if (! versiondata) {
        BSP_LCD_SetFont(&Font20);
        lcd.Clear(LCD_COLOR_RED);
        lcd.SetBackColor(LCD_COLOR_RED);
        lcd.DisplayStringAt(0, LINE(9), (uint8_t *)"Didn't find PN532 !", CENTER_MODE);
        while (1) {
            lcd.SetTextColor(LCD_COLOR_WHITE);
            lcd.DisplayStringAt(0, LINE(11), (uint8_t *)"PROGRAM HALTED", CENTER_MODE);
            wait(0.05);
            lcd.SetBackColor(LCD_COLOR_BLACK);
            lcd.SetTextColor(LCD_COLOR_WHITE);
            lcd.DisplayStringAt(0, LINE(11), (uint8_t *)"   PROGRAM HALTED   ", CENTER_MODE);
            DrawEndCube(&lcd);
        }
    }
    lcd.SetTextColor(LCD_COLOR_WHITE);
    lcd.DisplayStringAt(0, LINE(4), (uint8_t *)"SAMConfig();", LEFT_MODE);
    nfc->SAMConfig();
    wait(0.05);
    return 1;
}
