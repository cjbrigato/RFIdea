#include "mbed.h"

static I2C_HandleTypeDef heval_I2c1;

Serial pc(USBTX, USBRX); // tx, rx
//Terminal term(USBTX, USBRX);
