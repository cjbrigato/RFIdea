#include "mbed.h"
#include <string>
#include <stdio.h>

#include "main.h"
#include "apps.h"
#include "dump.h"
#include "nfc.h"
#include "cube.h"

#include "LCD_DISCO_F469NI.h"
#include "TS_DISCO_F469NI.h"


LCD_DISCO_F469NI lcd;
uint8_t text[60];

TS_DISCO_F469NI ts;
TS_StateTypeDef TS_State;

int main()
{

    pc.baud(460800);
    printf("SystemCoreClock = %d Hz\n", SystemCoreClock);
    while (!(LTDC->CDSR & LTDC_CDSR_VSYNCS)) {}

//    BSP_LCD_DrawBitmap(0,0, (uint8_t*) &_EMGCORECOLOR4);

    while(1) {
        //TS_DISCO_F469NI ts;
        //TS_StateTypeDef TS_State;
        uint16_t tsx, tsy;
        uint8_t tsstatus;
        uint8_t idx;

        BSP_LCD_SetFont(&Font16);

        tsstatus = ts.Init(lcd.GetXSize(), lcd.GetYSize());
        if (tsstatus != TS_OK) {
            lcd.Clear(LCD_COLOR_RED);
            lcd.SetBackColor(LCD_COLOR_RED);
            lcd.SetTextColor(LCD_COLOR_WHITE);
            lcd.DisplayStringAt(0, LINE(5), (uint8_t *)"TOUCHSCREEN INIT FAIL", CENTER_MODE);
            wait(300);
            return 127;
        }

        LogoCube(&lcd, &ts);
        wait(0.25);
        BSP_LCD_SetFont(&Font16);

        int gofunc=0;
        int knowndump=0;
        int scrysize = lcd.GetYSize();
        int scrxsize = lcd.GetXSize();
        

        lcd.Clear(LCD_COLOR_BLACK);
        lcd.SetBackColor(LCD_COLOR_BLACK);
        lcd.SetTextColor(LCD_COLOR_WHITE);
        lcd.DrawHLine(0, scrysize/2, scrxsize);
        lcd.DisplayStringAt(0, 120, (uint8_t *)"AUTO-CLONE MODE", CENTER_MODE);
        lcd.DisplayStringAt(0, 360, (uint8_t *)"KNOWNWRITE MODE", CENTER_MODE);

        while(1) {

            ts.GetState(&TS_State);
            if (TS_State.touchDetected) {
            // screenn 800x480
                for (idx = 0; idx < TS_State.touchDetected; idx++) {
                    tsx = TS_State.touchX[idx];
                    tsy = TS_State.touchY[idx];
                    //AUTO-CLONE
                    if (tsy < 240) {
                        gofunc=1;
                        goto endts;
                    //KNOWN-WRITE
                    } else if (tsy > 240) {
                        gofunc=2;
                        lcd.Clear(LCD_COLOR_BLACK);
                        lcd.SetBackColor(LCD_COLOR_BLACK);
                        lcd.SetTextColor(LCD_COLOR_WHITE);

                        int stepl = scrysize / dumpnbr; //need GetSizeY
                        int spacesize = 20;
                        int spacenbr = dumpnbr + 1 ;
                        int dbuttonsize = (scrysize - (spacesize * (spacenbr)))/(dumpnbr);
                        int dhbuttonstep = spacesize + dbuttonsize;
                        int dbuttonl = 400;
                        int dbuttonx = 200;
                        for (int i = 1; i <= dumpnbr; i++) {
                            lcd.DrawHLine(dbuttonx, dhbuttonstep*i, dbuttonl);
                            lcd.DrawHLine(dbuttonx, (dhbuttonstep*i)-dbuttonsize, dbuttonl);
                            lcd.DrawVLine(dbuttonx, (dhbuttonstep*i)-dbuttonsize, dbuttonsize);
                            lcd.DrawVLine(dbuttonx+dbuttonl, (dhbuttonstep*i)-dbuttonsize, dbuttonsize);
                        }
                        int stept = spacesize+(dbuttonsize);
                        int stept1 = stept - dbuttonsize/2;
                        // We got 30 (0->29) LINES
                        lcd.SetTextColor(LCD_COLOR_YELLOW);
                        lcd.DisplayStringAt(0, stept1, (uint8_t *)"FORMAT BLANK", CENTER_MODE);
                        lcd.SetTextColor(LCD_COLOR_WHITE);
                        lcd.DisplayStringAt(0, stept1+stept, (uint8_t *)"DUMP: Bureau (Colin)", CENTER_MODE);
                        lcd.DisplayStringAt(0, stept1+(2*stept), (uint8_t *)"DUMP: Maison (Colin)", CENTER_MODE);
                        lcd.DisplayStringAt(0, stept1+(3*stept), (uint8_t *)"Dump: Maman Anais (Colin)", CENTER_MODE);
                        lcd.DisplayStringAt(0, stept1+(4*stept), (uint8_t *)"DUMP: Ornano", CENTER_MODE);
                        wait(1);
                        while(1) {
                            ts.GetState(&TS_State);
                            if (TS_State.touchDetected) {
                                for (idx = 0; idx < TS_State.touchDetected; idx++) {
                                    tsx = TS_State.touchX[idx];
                                    tsy = TS_State.touchY[idx];
                                    for (int i = 0; i < dumpnbr; i++) {
                                        if (tsy > i*stepl && tsy < (i+1)*stepl) {
                                            knowndump=i;
                                            goto endts;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

    //APPLICATION SWITCH
    endts:
        delete &ts;
        HAL_I2C_DeInit(&heval_I2c1);
        wait(0.25);

        SPI spi(D11, D12, D13);  // SPI(mosi, miso, clk)
        PN532_SPI pn532spi(spi, D14);
        PN532 nfc(pn532spi);
        nfcinit(&nfc);
        wait(0.25);
        
        switch (gofunc) {
            case 1:
                AutoClone(&nfc);
                break;
            case 2:
                KnownWrite(&nfc,knowndump);
                break;  
        }
    }
}
