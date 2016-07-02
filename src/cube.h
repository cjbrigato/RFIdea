#include "LCD_DISCO_F469NI.h"
#include "TS_DISCO_F469NI.h"
#include "mbed.h"

#define SHAPE_SIZE 1500
#define ROTATE_SPEED 0.006 // Delay in ms between cube redraws
#define SPEED 0.5
#define PI 3.1416//265358979323846264338327950288419716939937510
void DrawEndCube(LCD_DISCO_F469NI *lcd);
void LogoCube(LCD_DISCO_F469NI *lcd, TS_DISCO_F469NI *ts);
