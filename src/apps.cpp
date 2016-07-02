#include "mbed.h"
#include "LCD_DISCO_F469NI.h"
#include "PN532.h"
#include "apps.h"
#include "dump.h"
#include "keys.h"
#include "utils.h"
#include "nfc.h"

extern LCD_DISCO_F469NI lcd;
extern uint8_t text[60];

// Intern Dump to Tag App
int KnownWrite(PN532 *nfc, int dump)
{
    start:
    RefreshLCD(&lcd);
    lcd.SetTextColor(LCD_COLOR_WHITE);
    lcd.Clear(LCD_COLOR_BLACK);
    lcd.SetBackColor(LCD_COLOR_BLACK);
    BSP_LCD_SetFont(&Font20);
    uint8_t success;
    uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
    uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
    uint16_t atqa;
    uint8_t sak;
    uint8_t currentblock;
    uint8_t datac[64][16];

    // BCC Reminder
    // abtData[4] = abtData[0] ^ abtData[1] ^ abtData[2] ^ abtData[3];
    // dump_blank[0][4] = dump_blank[0][0] ^ dump_blank[0][1] ^ dump_blank[0][2] ^ dump_blank[0][3];

    nfc->setPassiveActivationRetries(0xFF);

    lcd.DisplayStringAt(0, LINE(10), (uint8_t *)"WAITING FOR TAG", CENTER_MODE);

    success = nfc->readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, &atqa, &sak,0);
    if (!success) {
        goto start;
    }

    if (success) {
        lcd.Clear(LCD_COLOR_BLACK);
        lcd.SetBackColor(LCD_COLOR_BLUE);
        lcd.SetTextColor(LCD_COLOR_YELLOW);
        lcd.DisplayStringAt(0, LINE(1), (uint8_t *)"ISO14443A (MIFARE Classic?) found.", CENTER_MODE);
        lcd.SetBackColor(LCD_COLOR_BLACK);
        uint8_t natqa[]= { (uint8_t)(atqa >> 8), (uint8_t)atqa };
        char satqa[60];
        uint8_t suid[30];
        char ssak[60];
        sprintf((char*)suid,"* UID : ");
        sprintf(satqa,"* ATQA : ");
        sprintf(ssak,"* SAK : ");
        sprintf(satqa + strlen(satqa),"%02X %02X", natqa[0] & 0xFF, natqa[1] & 0xFF);
        sprintf(ssak + strlen(ssak),"%02X", sak & 0xFF);
        for (uint8_t i=0; i < uidLength ; i++) {
            sprintf((char*)suid + strlen((char*)suid),"%02X ",uid[i] & 0xff);
        }
        lcd.SetTextColor(LCD_COLOR_GREEN);
        lcd.DisplayStringAt(0, LINE(5), (uint8_t *)suid, LEFT_MODE);
        lcd.SetTextColor(LCD_COLOR_WHITE);
        lcd.DisplayStringAt(0, LINE(7), (uint8_t *)satqa, LEFT_MODE);
        lcd.DisplayStringAt(0, LINE(9), (uint8_t *)ssak, LEFT_MODE);
        wait (0.1);
        int dmptime = TBDUMP;
        lcd.SetTextColor(LCD_COLOR_YELLOW);
        lcd.DisplayStringAt(0, LINE(15), (uint8_t *)"Get the Key Back NOW to CANCEL !", CENTER_MODE);
        for (int i = 0 ; i <= dmptime; i++) {
            int rest = dmptime - i;
            char counter[60];
            sprintf(counter, "Will Start copy in : ");
            sprintf(counter + strlen(counter), "%u", rest);
            lcd.ClearStringLine(LINE(13));
            lcd.SetTextColor(LCD_COLOR_RED);
            lcd.DisplayStringAt(0, LINE(13), (uint8_t *)counter, CENTER_MODE);
            lcd.SetTextColor(LCD_COLOR_WHITE);
            wait (1);
        }
    }
    success = nfc->readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, &atqa, &sak);
    if (!success) {
        lcd.Clear(LCD_COLOR_RED);
        wait(0.05);
        lcd.DisplayStringAt(0, LINE(10), (uint8_t *)"TAG HAS GONE AWAY", CENTER_MODE);
        wait(1);
        goto start;
    }

    // IT NEED TYPEDEF STRUCT WITH NAMES AND ALL
    switch(dump) {
        case 0:
            memcpy(datac,dump_blank,sizeof(dump_blank));
            break;
        case 1:
            memcpy(datac,dump_buro_colin,sizeof(dump_buro_colin));
            break;
        case 2:
            memcpy(datac,dump_maison_colin,sizeof(dump_maison_colin));
            break;
        case 3:
            memcpy(datac,dump_maman_anais,sizeof(dump_maman_anais));
            break;
        case 4:
            memcpy(datac,dump_ornano,sizeof(dump_ornano));
            break;
    }

    if (success) {
        RefreshLCD(&lcd);
        lcd.Clear(LCD_COLOR_BLACK);
        BSP_LCD_SetFont(&Font16);
        wait(0.05);
        lcd.SetTextColor(LCD_COLOR_WHITE);
        lcd.DisplayStringAt(0, LINE(10), (uint8_t *)"COPYING...", CENTER_MODE);
        nfc->unlockChina();
        for (currentblock = 0; currentblock < 64; currentblock++) {
            printf("[%u]",(unsigned int)currentblock);
            success = nfc->mifareclassic_WriteDataBlock(currentblock, datac[currentblock]);
        }
        lcd.Clear(LCD_COLOR_BLACK);
        wait(0.05);
        lcd.SetTextColor(LCD_COLOR_YELLOW);
        lcd.DisplayStringAt(0, LINE(10), (uint8_t *)"DONE.", CENTER_MODE);
        lcd.SetTextColor(LCD_COLOR_YELLOW);
        lcd.DisplayStringAt(0,LINE(12), (uint8_t *)"Have a nice day.", CENTER_MODE);
        lcd.SetTextColor(LCD_COLOR_WHITE);
        wait(3);
        HAL_NVIC_SystemReset();
    }

    return 0;
}

int AutoClone(PN532 *nfc)
{
    int lineselect=0;
    int colselect=0;
    lcd.Clear(LCD_COLOR_BLACK);
    lcd.SetBackColor(LCD_COLOR_BLACK);

    start:
    RefreshLCD(&lcd);
    // We ARE IN
    lcd.SetTextColor(LCD_COLOR_WHITE);
    lcd.Clear(LCD_COLOR_BLACK);
    lcd.SetBackColor(LCD_COLOR_BLACK);
    BSP_LCD_SetFont(&Font20);
    uint8_t success;
    uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
    uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
    uint16_t atqa;
    uint8_t sak;
    uint8_t currentblock;                     // Counter to keep track of which block we're on
    bool authenticated = false;               // Flag to indicate if the sector is authenticated
    uint8_t datac[64][16];

    uint8_t foundKeysA[64][6];
    uint8_t foundKeysB[64][6];
    bool foundFor[64][2];
    for (int i=0; i<64; i++) {
        foundFor[i][0]=false;
        foundFor[i][1]=false;
    }

    int nkeys = sizeof(defaultKeys) / sizeof(defaultKeys[0]); //number of keys
    int key = 0; // Current key
    char snkeys[60];
    sprintf(snkeys,"Registered keys : ");
    sprintf(snkeys + strlen(snkeys), "%u", nkeys);

    nfc->setPassiveActivationRetries(0xFF);

    lcd.DisplayStringAt(0, LINE(10), (uint8_t *)"Ready.", CENTER_MODE);
    lcd.DisplayStringAt(0, LINE(22), (uint8_t *)snkeys, RIGHT_MODE);

    success = nfc->readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, &atqa, &sak,0);
    if (!success) {
        goto start;
    }
    if (success) {
        lcd.Clear(LCD_COLOR_BLACK);
        lcd.SetBackColor(LCD_COLOR_BLUE);
        lcd.SetTextColor(LCD_COLOR_YELLOW);
        lcd.DisplayStringAt(0, LINE(1), (uint8_t *)"ISO14443A (MIFARE Classic?) found.", CENTER_MODE);
        lcd.SetBackColor(LCD_COLOR_BLACK);
        uint8_t natqa[]= { (uint8_t)(atqa >> 8), (uint8_t)atqa };
        char satqa[60];
        uint8_t suid[30];
        char ssak[60];
        sprintf((char*)suid,"* UID : ");
        sprintf(satqa,"* ATQA : ");
        sprintf(ssak,"* SAK : ");
        sprintf(satqa + strlen(satqa),"%02X %02X", natqa[0] & 0xFF, natqa[1] & 0xFF);
        sprintf(ssak + strlen(ssak),"%02X", sak & 0xFF);
        for (uint8_t i=0; i < uidLength ; i++) {
            sprintf((char*)suid + strlen((char*)suid),"%02X ",uid[i] & 0xff);
        }
        lcd.SetTextColor(LCD_COLOR_GREEN);
        lcd.DisplayStringAt(0, LINE(5), (uint8_t *)suid, LEFT_MODE);
        lcd.SetTextColor(LCD_COLOR_WHITE);
        lcd.DisplayStringAt(0, LINE(7), (uint8_t *)satqa, LEFT_MODE);
        lcd.DisplayStringAt(0, LINE(9), (uint8_t *)ssak, LEFT_MODE);
        wait (0.1);
        int dmptime = TBDUMP;
        lcd.SetTextColor(LCD_COLOR_YELLOW);
        lcd.DisplayStringAt(0, LINE(15), (uint8_t *)"Get the Key Back NOW to CANCEL !", CENTER_MODE);
        for (int i = 0 ; i <= dmptime; i++) {
            int rest = dmptime - i;
            char counter[60];
            sprintf(counter, "Will Start dump in : ");
            sprintf(counter + strlen(counter), "%u", rest);
            lcd.ClearStringLine(LINE(13));
            lcd.SetTextColor(LCD_COLOR_RED);
            lcd.DisplayStringAt(0, LINE(13), (uint8_t *)counter, CENTER_MODE);
            lcd.SetTextColor(LCD_COLOR_WHITE);
            wait (1);
        }
    }

    success = nfc->readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, &atqa, &sak);
    if (!success) {
        lcd.Clear(LCD_COLOR_RED);
        wait(0.05);
        lcd.DisplayStringAt(0, LINE(10), (uint8_t *)"TAG HAS GONE AWAY", CENTER_MODE);
        wait(1);
        goto start;
    }

    if (success) {
        RefreshLCD(&lcd);
        lcd.Clear(LCD_COLOR_BLACK);
        BSP_LCD_SetFont(&Font16);
        lcd.DisplayStringAt(0, LINE(0), (uint8_t *)"Breaking Keys Over.", LEFT_MODE);

        int cipherleft=16*2; //16 sectors * 2 Keys (A/B)
        for (key = 0; key <= nkeys; key++) {
            colselect=0;
            lineselect=key-(key/2)+2;
            if (key % 2) {
                colselect=lcd.GetXSize()/2;
                lineselect=key-(key/2)+1;
            }
            int keyspace=18;
            sprintf((char*)text,"%02X %02X %02X %02X %02X %02X", defaultKeys[key][0] & 0xff,defaultKeys[key][1] & 0xff,defaultKeys[key][2] & 0xff,defaultKeys[key][3] & 0xff,defaultKeys[key][4] & 0xff,defaultKeys[key][5] & 0xff);
            lcd.DisplayStringAt(colselect,LINE(lineselect), (uint8_t *)text, LEFT_MODE);
            for (currentblock = 0; currentblock < 64; currentblock++) {
                if (nfc->mifareclassic_IsFirstBlock(currentblock)) {
                    bool gotkey[2]= { false, false };
                    //may test A
                    if (foundFor[currentblock][0] == false) {
                        nfc->quickMifareResetN();
                        success = nfc->mifareclassic_AuthenticateBlock (uid, uidLength, currentblock, 0, defaultKeys[key]);
                        if (success) {
                            for (int k = 0; k <4; k++) {
                                memcpy(foundKeysA[currentblock+k], defaultKeys[key], sizeof(defaultKeys[key]));
                                foundFor[currentblock+k][0]=true;
                            }
                            gotkey[0]=true;
                            cipherleft--;
                        }
                    }
                    // may test B
                    // WE SHOULD REVEAL B KEY BY READING A KEY
                    //if(false){
                    if (foundFor[currentblock][1] == false) {
                        nfc->quickMifareResetN();
                        uint8_t success2 = nfc->mifareclassic_AuthenticateBlock (uid, uidLength, currentblock, 1, defaultKeys[key]);
                        if (success2) {
                            for (int k = 0; k <4; k++) {
                                memcpy(foundKeysB[currentblock+k], defaultKeys[key], sizeof(defaultKeys[key]));
                                foundFor[currentblock+k][1]=true;
                            }
                            gotkey[1]=true;
                            cipherleft--;
                        }
                    }
                    if (gotkey[0] == true) {
                        if (gotkey[1] == true) {
                            sprintf((char*)text,"X");
                        } else {
                            sprintf((char*)text,"A");
                        }
                    } else {
                        if (gotkey[1] == true) {
                            sprintf((char*)text,"B");
                        } else {
                            sprintf((char*)text,".");
                        }
                    }
                    lcd.DisplayStringAt(colselect+LCOL(keyspace+currentblock/4),LINE(lineselect), (uint8_t *)text, LEFT_MODE);
                }
            }
        }

        wait(0.1);
        lcd.Clear(LCD_COLOR_BLACK);
        if(cipherleft !=0) {

            lcd.SetTextColor(LCD_COLOR_RED);
            lcd.DisplayStringAt(0, LINE(10), (uint8_t *)"FAILED TO GET ALL CIPHERS", CENTER_MODE);
            wait(3);
            goto start;
        }

        //END OF ALL TESTKEYS
        sprintf((char*)text,"Resume for Tag : ");
        for (uint8_t i=0; i < uidLength ; i++) {
            sprintf((char*)text + strlen((char*)text),"%02X ",uid[i] & 0xff);
        }
        lcd.DisplayStringAt(0, LINE(2), (uint8_t *)text, CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(3), (uint8_t *)"+---------------------+---------------+-------------------+", CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(4), (uint8_t *)"|       KEY A         |    SECTOR     |       KEY B       |", CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(5), (uint8_t *)"+---------------------+---------------+-------------------+", CENTER_MODE);
        for (int b=0; b<64; b++) {
            if (nfc->mifareclassic_IsFirstBlock(b)) {
                int s = b/4;
                sprintf((char*)text,"|    %02X%02X%02X%02X%02X%02X     |       %X       |   %02X%02X%02X%02X%02X%02X    |", foundKeysA[b][0] & 0xff,foundKeysA[b][1] & 0xff,foundKeysA[b][2] & 0xff,foundKeysA[b][3] & 0xff,foundKeysA[b][4] & 0xff, foundKeysA[b][5] & 0xff, s, foundKeysB[b][0] & 0xff,foundKeysB[b][1] & 0xff,foundKeysB[b][2] & 0xff,foundKeysB[b][3] & 0xff,foundKeysB[b][4] & 0xff,foundKeysB[b][5] & 0xff);
                lcd.DisplayStringAt(0, LINE(6+s), (uint8_t *)text,CENTER_MODE);
            }
        }
        lcd.DisplayStringAt(0, LINE(22), (uint8_t *)"+---------------------+---------------+-------------------+", CENTER_MODE);
        lcd.SetTextColor(LCD_COLOR_RED);
        lcd.DisplayStringAt(0, LINE(22), (uint8_t *)"WE ARE DUMPING RIGHT NOW", CENTER_MODE);
        wait(0.5);
        nfc->quickMifareReset();
        printf("\n uint8_t dump_name_me[64][16] = { \n");
        for (currentblock = 0; currentblock < 64; currentblock++) {
            if (nfc->mifareclassic_IsFirstBlock(currentblock)) authenticated = false;
            if (!authenticated) {
                success = nfc->mifareclassic_AuthenticateBlock (uid, uidLength, currentblock, 0, foundKeysA[currentblock]);
            }
            if (success) {
                authenticated = true;
            } else {
                printf("Authentication error, Trying B key\n");
                nfc->quickMifareReset();
                //currentblock--;
                success = nfc->mifareclassic_AuthenticateBlock (uid, uidLength, currentblock, 1, foundKeysB[currentblock]);
                if (success) {
                    authenticated = true;
                } else {
                    printf("Authentication error, DEADLINE\n");
                    goto start;
                }
            }
            if (!authenticated) {
                printf("Block ");
                printf("%u",currentblock);
                printf(" unable to authenticate\n");
            } else {
                success = nfc->mifareclassic_ReadDataBlock(currentblock, datac[currentblock]);
                if (success) {
                    if (nfc->mifareclassic_IsTrailerBlock(currentblock)) {
                        memcpy(datac[currentblock],foundKeysA[currentblock],sizeof(foundKeysA[currentblock]));
                        memcpy(datac[currentblock]+(10*sizeof(foundKeysA[currentblock][0])),foundKeysB[currentblock],sizeof(foundKeysB[currentblock]));
                    }
                    //HORRIBL
                    printf("{ ");
                    for (int i=0; i< 16; i++) {
                        if (i == 15) {
                            printf("0x%02X ", datac[currentblock][i]);
                        } else {
                            printf("0x%02X, ", datac[currentblock][i]);
                       }
                    }
                    if (currentblock==63) {
                        printf("}\n};\n");
                    } else {
                        printf("},\n");
                    }
                } else {
                    printf("Block ");
                    printf("%u",currentblock);
                    printf(" unable to read this block\n");
                }
            }
        }
        lcd.Clear(LCD_COLOR_BLACK);
        wait(0.05);
        lcd.DisplayStringAt(0, LINE(10), (uint8_t *)"END OF DUMP !", CENTER_MODE);
        success = nfc->readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, &atqa, &sak,0);
        while(success) {
            lcd.SetTextColor(LCD_COLOR_YELLOW);
            lcd.DisplayStringAt(0, LINE(12), (uint8_t *)"GET ORIGINAL TAG BACK PLEASE", CENTER_MODE);
            wait(0.5),
                 lcd.SetTextColor(LCD_COLOR_BLACK);
            lcd.DisplayStringAt(0, LINE(12), (uint8_t *)"GET ORIGINAL TAG BACK PLEASE", CENTER_MODE);
            wait(0.5);
            success = nfc->readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, &atqa, &sak);
        }
        lcd.SetTextColor(LCD_COLOR_GREEN);
        lcd.DisplayStringAt(0, LINE(12), (uint8_t *)"INSERT BLANK MIFARE FOR COPY", CENTER_MODE);
        success = nfc->readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, &atqa, &sak,0);
        lcd.Clear(LCD_COLOR_BLACK);
        wait(0.05);
        lcd.SetTextColor(LCD_COLOR_WHITE);
        lcd.DisplayStringAt(0, LINE(10), (uint8_t *)"COPYING...", CENTER_MODE);
        nfc->unlockChina();
        for (currentblock = 0; currentblock < 64; currentblock++) {
            success = nfc->mifareclassic_WriteDataBlock(currentblock, datac[currentblock]);
	    printf("[%u] - MightBeenDone",(unsigned int)currentblock);
        }

        lcd.Clear(LCD_COLOR_BLACK);
        wait(0.05);
        lcd.SetTextColor(LCD_COLOR_YELLOW);
        lcd.DisplayStringAt(0, LINE(10), (uint8_t *)"DONE.", CENTER_MODE);
        lcd.SetTextColor(LCD_COLOR_YELLOW);
        lcd.DisplayStringAt(0,LINE(12), (uint8_t *)"Have a nice day.", CENTER_MODE);
        lcd.SetTextColor(LCD_COLOR_WHITE);
        wait(3);
        HAL_NVIC_SystemReset();
    }

    return 0;
}
