#include "Ndef.h"
#include "PN532_debug.h"

// Borrowed from Adafruit_NFCShield_I2C
void PrintHex(const uint8_t * data, const long numuint8_ts)
{
  uint32_t szPos;
  for (szPos=0; szPos < numuint8_ts; szPos++)
  {
    DMSG("0x");
    // Append leading 0 for small values
    if (data[szPos] <= 0xF)
      DMSG("0");
    DMSG_HEX(data[szPos]&0xff);
    if ((numuint8_ts > 1) && (szPos != numuint8_ts - 1))
    {
      DMSG(" ");
    }
  }
  DMSG("");
}

// Borrowed from Adafruit_NFCShield_I2C
void PrintHexChar(const uint8_t * data, const long numuint8_ts)
{
  uint32_t szPos;
  for (szPos=0; szPos < numuint8_ts; szPos++)
  {
    // Append leading 0 for small values
    DMSG_HEX(data[szPos]);
  }
  DMSG("   ");
  for (szPos=0; szPos < numuint8_ts; szPos++)
  {
    if (data[szPos] <= 0x1F)
      DMSG(".");
    else
      DMSG("%c", (char)data[szPos]);
  }
  DMSG("\n");
}

// Note if buffer % blockSize != 0, last block will not be written
void DumpHex(const uint8_t * data, const long numuint8_ts, const unsigned int blockSize)
{
    int i;
    for (i = 0; i < (numuint8_ts / blockSize); i++)
    {
        PrintHexChar(data, blockSize);
        data += blockSize;
    }
}
