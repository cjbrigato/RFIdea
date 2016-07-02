#ifndef Ndef_h
#define Ndef_h

/* NOTE: To use the Ndef library in your code, don't include Ndef.h
   See README.md for details on which files to include in your sketch.
*/
#include <stdint.h>

void PrintHex(const uint8_t *data, const long numuint8_ts);
void PrintHexChar(const uint8_t *data, const long numuint8_ts);
void DumpHex(const uint8_t *data, const long numuint8_ts, const int blockSize);

#endif
