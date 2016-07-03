//     @author    Colin J. Brigato < colin@brigato.fr>

#include "mbed.h"
#include "nfc.h"

#define TBDUMP 1

const int appsnbr=2;

int KnownWrite(PN532 *nfc, int dump);
int AutoClone(PN532 *nfc);
