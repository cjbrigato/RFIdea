//     @author    Colin J. Brigato < colin@brigato.fr>

#include "cube.h"

extern TS_StateTypeDef TS_State;

void DrawEndCube(LCD_DISCO_F469NI *lcd)
{

    float d = 3;
    float px[] = { -d,  d,  d, -d, -d,  d,  d, -d };
    float py[] = { -d, -d,  d,  d, -d, -d,  d,  d };
    float pz[] = { -d, -d, -d, -d,  d,  d,  d,  d };

    float p2x[] = {0,0,0,0,0,0,0,0};
    float p2y[] = {0,0,0,0,0,0,0,0};

    float r[] = {0,0,0};
    lcd->SetTextColor(LCD_COLOR_WHITE);

    int iterate = 0;
    while(1) {
        //while (!(LTDC->CDSR & LTDC_CDSR_VSYNCS)) {}
        if (iterate == 500) {
            HAL_NVIC_SystemReset();
        }
        lcd->SetTextColor(LCD_COLOR_WHITE);
        r[0] = r[0] + (SPEED * PI) / 180.0; // Add a degree
        r[1] = r[1] + (SPEED * PI) / 180.0; // Add a degree
        r[2] = r[2] + (SPEED * PI) / 180.0; // Add a degree
        if (r[0] >= 360.0 * PI / 180.0) r[0] = 0;
        if (r[1] >= 360.0 * PI / 180.0) r[1] = 0;
        if (r[2] >= 360.0 * PI / 180.0) r[2] = 0;

        for (int i = 0; i < 8; i++) {
            float px2 = px[i];
            float py2 = cos(r[0]) * py[i] - sin(r[0]) * pz[i];
            float pz2 = sin(r[0]) * py[i] + cos(r[0]) * pz[i];

            float px3 = cos(r[1]) * px2 + sin(r[1]) * pz2;
            float py3 = py2;
            float pz3 = -sin(r[1]) * px2 + cos(r[1]) * pz2;

            float ax = cos(r[2]) * px3 - sin(r[2]) * py3;
            float ay = sin(r[2]) * px3 + cos(r[2]) * py3;
            float az = pz3 - 150;

            p2x[i] = 180 + ax * SHAPE_SIZE / az;
            p2y[i] = 210 + ay * SHAPE_SIZE / az;

        }

        for (int i = 0; i < 3; i++) {
            lcd->DrawLine(p2x[i],   p2y[i],   p2x[i+1], p2y[i+1]);
            lcd->DrawLine(p2x[i+4], p2y[i+4], p2x[i+5], p2y[i+5]);
            lcd->DrawLine(p2x[i],   p2y[i],   p2x[i+4], p2y[i+4]);
        }

        lcd->DrawLine(p2x[3], p2y[3], p2x[0], p2y[0]);
        lcd->DrawLine(p2x[7], p2y[7], p2x[4], p2y[4]);
        lcd->DrawLine(p2x[3], p2y[3], p2x[7], p2y[7]);
        wait(ROTATE_SPEED);
        lcd->SetTextColor(LCD_COLOR_RED);
        lcd->FillRect(130,157,120,120);
        iterate++;
    }
}

void LogoCube(LCD_DISCO_F469NI *lcd, TS_DISCO_F469NI *ts)
{

    float d = 3;
    float px[] = { -d,  d,  d, -d, -d,  d,  d, -d };
    float py[] = { -d, -d,  d,  d, -d, -d,  d,  d };
    float pz[] = { -d, -d, -d, -d,  d,  d,  d,  d };

    float p2x[] = {0,0,0,0,0,0,0,0};
    float p2y[] = {0,0,0,0,0,0,0,0};

    float r[] = {0,0,0};

    lcd->Clear(LCD_COLOR_BLACK);
    lcd->SetTextColor(LCD_COLOR_WHITE);
    lcd->SetBackColor(LCD_COLOR_BLACK);
    BSP_LCD_SetFont(&Font24);
    lcd->DisplayStringAt(75, LINE(8), (uint8_t *)" RFIdea beta1", CENTER_MODE); // NEEDS VERSION VARIABLE
    BSP_LCD_SetFont(&Font16);
    lcd->DisplayStringAt(83, LINE(14), (uint8_t *)" (build.16061707)", CENTER_MODE);
    BSP_LCD_SetFont(&Font20);
    lcd->DisplayStringAt(0, LINE(18), (uint8_t *)"Touch the Screen to begin", CENTER_MODE); //Meh

    

    while(1) {

        ts->GetState(&TS_State);
        if (TS_State.touchDetected) {
            return ;
        }

        //while (!(LTDC->CDSR & LTDC_CDSR_VSYNCS)) {}
        lcd->SetTextColor(LCD_COLOR_BLUE);
        r[0] = r[0] + (SPEED * PI) / 180.0; // Add a degree
        r[1] = r[1] + (SPEED * PI) / 180.0; // Add a degree
        r[2] = r[2] + (SPEED * PI) / 180.0; // Add a degree
        if (r[0] >= 360.0 * PI / 180.0) r[0] = 0;
        if (r[1] >= 360.0 * PI / 180.0) r[1] = 0;
        if (r[2] >= 360.0 * PI / 180.0) r[2] = 0;

        for (int i = 0; i < 8; i++) {
            float px2 = px[i];
            float py2 = cos(r[0]) * py[i] - sin(r[0]) * pz[i];
            float pz2 = sin(r[0]) * py[i] + cos(r[0]) * pz[i];

            float px3 = cos(r[1]) * px2 + sin(r[1]) * pz2;
            float py3 = py2;
            float pz3 = -sin(r[1]) * px2 + cos(r[1]) * pz2;

            float ax = cos(r[2]) * px3 - sin(r[2]) * py3;
            float ay = sin(r[2]) * px3 + cos(r[2]) * py3;
            float az = pz3 - 150;

            p2x[i] = 180 + ax * SHAPE_SIZE*2 / az;
            p2y[i] = 210 + ay * SHAPE_SIZE*2 / az;

        }

        for (int i = 0; i < 3; i++) {
            switch(i) {
                case 0:
                    lcd->SetTextColor(LCD_COLOR_BLUE);
                    break;
                case 1:
                    lcd->SetTextColor(LCD_COLOR_RED);
                    break;
                case 2:
                    lcd->SetTextColor(LCD_COLOR_WHITE);
                    break;
            }
            lcd->DrawLine(p2x[i],   p2y[i],   p2x[i+1], p2y[i+1]);
            switch(i) {
                case 0:
                    lcd->SetTextColor(LCD_COLOR_CYAN);
                    break;
                case 1:
                    lcd->SetTextColor(LCD_COLOR_BROWN);
                    break;
                case 2:
                    lcd->SetTextColor(LCD_COLOR_YELLOW);
                    break;
            }
            lcd->DrawLine(p2x[i+4], p2y[i+4], p2x[i+5], p2y[i+5]);
            switch(i) {
                case 0:
                    lcd->SetTextColor(LCD_COLOR_WHITE);
                    break;
                case 1:
                    lcd->SetTextColor(LCD_COLOR_ORANGE);
                    break;
                case 2:
                    lcd->SetTextColor(LCD_COLOR_GREEN);
                    break;
            }
            lcd->DrawLine(p2x[i],   p2y[i],   p2x[i+4], p2y[i+4]);
        }

        lcd->SetTextColor(LCD_COLOR_WHITE);
        lcd->DrawLine(p2x[3], p2y[3], p2x[0], p2y[0]);
        lcd->SetTextColor(LCD_COLOR_GREEN);
        lcd->DrawLine(p2x[7], p2y[7], p2x[4], p2y[4]);
        lcd->SetTextColor(LCD_COLOR_ORANGE);
        lcd->DrawLine(p2x[3], p2y[3], p2x[7], p2y[7]);

        //wait(ROTATE_SPEED);
        while (!(LTDC->CDSR & LTDC_CDSR_VSYNCS)) {}
        lcd->SetTextColor(LCD_COLOR_BLACK);

        for (int i = 0; i < 3; i++) {
            lcd->DrawLine(p2x[i],   p2y[i],   p2x[i+1], p2y[i+1]);
            lcd->DrawLine(p2x[i+4], p2y[i+4], p2x[i+5], p2y[i+5]);
            lcd->DrawLine(p2x[i],   p2y[i],   p2x[i+4], p2y[i+4]);
        }

        lcd->DrawLine(p2x[3], p2y[3], p2x[0], p2y[0]);
        lcd->DrawLine(p2x[7], p2y[7], p2x[4], p2y[4]);
        lcd->DrawLine(p2x[3], p2y[3], p2x[7], p2y[7]);

        //lcd->SetTextColor(LCD_COLOR_BLUE);
        //lcd->FillRect(60,80,240,240);
    }
}
