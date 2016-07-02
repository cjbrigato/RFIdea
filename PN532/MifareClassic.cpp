#include "mbed.h"
#include "MifareClassic.h"
#include "PN532_debug.h"

#define BLOCK_SIZE 16
#define LONG_TLV_SIZE 4
#define SHORT_TLV_SIZE 2

#define MIFARE_CLASSIC ("Mifare Classic")

MifareClassic::MifareClassic(PN532& nfcShield)
{
  _nfcShield = &nfcShield;
}

MifareClassic::~MifareClassic()
{
}

NfcTag MifareClassic::read(uint8_t *uid, unsigned int uidLength)
{
    uint8_t key[6] = { 0xD3, 0xF7, 0xD3, 0xF7, 0xD3, 0xF7 };
    int currentBlock = 4;
    int messageStartIndex = 0;
    int messageLength = 0;
    uint8_t data[BLOCK_SIZE];

    // read first block to get message length
    int success = _nfcShield->mifareclassic_AuthenticateBlock(uid, uidLength, currentBlock, 0, key);
    if (success)
    {
        success = _nfcShield->mifareclassic_ReadDataBlock(currentBlock, data);
        if (success)
        {
            if (!decodeTlv(data, messageLength, messageStartIndex)) {
                return NfcTag(uid, uidLength, "ERROR"); // TODO should the error message go in NfcTag?
            }
        }
        else
        {
            DMSG("Error. Failed read block ");
			DMSG_INT(currentBlock);
            return NfcTag(uid, uidLength, MIFARE_CLASSIC);
        }
    }
    else
    {
        DMSG("Tag is not NDEF formatted.");
        // TODO set tag.isFormatted = false
        return NfcTag(uid, uidLength, MIFARE_CLASSIC);
    }

    // this should be nested in the message length loop
    int index = 0;
    int bufferSize = getBufferSize(messageLength);
    uint8_t buffer[bufferSize];

    #ifdef MIFARE_CLASSIC_DEBUG
    DMSG("Message Length ");
    DMSG_INT(messageLength);
    DMSG("Buffer Size ");
    DMSG_INT(bufferSize);
    #endif

    while (index < bufferSize)
    {

        // authenticate on every sector
        if (_nfcShield->mifareclassic_IsFirstBlock(currentBlock))
        {
            success = _nfcShield->mifareclassic_AuthenticateBlock(uid, uidLength, currentBlock, 0, key);
            if (!success)
            {
                DMSG("Error. Block Authentication failed for ");
                DMSG_INT(currentBlock);
                // TODO error handling
            }
        }

        // read the data
        success = _nfcShield->mifareclassic_ReadDataBlock(currentBlock, &buffer[index]);
        if (success)
        {
            #ifdef MIFARE_CLASSIC_DEBUG
            DMSG("Block ");
            DMSG_INT(currentBlock);
            _nfcShield->PrintHexChar(&buffer[index], BLOCK_SIZE);
            #endif
        }
        else
        {
            DMSG("Read failed ");
            DMSG_INT(currentBlock);
            // TODO handle errors here
        }

        index += BLOCK_SIZE;
        currentBlock++;

        // skip the trailer block
        if (_nfcShield->mifareclassic_IsTrailerBlock(currentBlock))
        {
            #ifdef MIFARE_CLASSIC_DEBUG
            DMSG("Skipping block ");
            DMSG_INT(currentBlock);
            #endif
            currentBlock++;
        }
    }

    return NfcTag(uid, uidLength, MIFARE_CLASSIC, &buffer[messageStartIndex], messageLength);
}

int MifareClassic::getBufferSize(int messageLength)
{

    int bufferSize = messageLength;

    // TLV header is 2 or 4 uint8_ts, TLV terminator is 1 uint8_t.
    if (messageLength < 0xFF)
    {
        bufferSize += SHORT_TLV_SIZE + 1;
    }
    else
    {
        bufferSize += LONG_TLV_SIZE + 1;
    }

    // bufferSize needs to be a multiple of BLOCK_SIZE
    if (bufferSize % BLOCK_SIZE != 0)
    {
        bufferSize = ((bufferSize / BLOCK_SIZE) + 1) * BLOCK_SIZE;
    }

    return bufferSize;
}

// skip null tlvs (0x0) before the real message
// technically unlimited null tlvs, but we assume
// T & L of TLV in the first block we read
int MifareClassic::getNdefStartIndex(uint8_t *data)
{

    for (int i = 0; i < BLOCK_SIZE; i++)
    {
        if (data[i] == 0x0)
        {
            // do nothing, skip
        }
        else if (data[i] == 0x3)
        {
            return i;
        }
        else
        {
            DMSG("Unknown TLV ");
            DMSG_HEX(data[i]);
            return -2;
        }
    }

    return -1;
}

// Decode the NDEF data length from the Mifare TLV
// Leading null TLVs (0x0) are skipped
// Assuming T & L of TLV will be in the first block
// messageLength and messageStartIndex written to the parameters
// success or failure status is returned
//
// { 0x3, LENGTH }
// { 0x3, 0xFF, LENGTH, LENGTH }
bool MifareClassic::decodeTlv(uint8_t *data, int &messageLength, int &messageStartIndex)
{
    int i = getNdefStartIndex(data);

    if (i < 0 || data[i] != 0x3)
    {
        DMSG("Error. Can't decode message length.");
        return false;
    }
    else
    {
        if (data[i+1] == 0xFF)
        {
            messageLength = ((0xFF & data[i+2]) << 8) | (0xFF & data[i+3]);
            messageStartIndex = i + LONG_TLV_SIZE;
        }
        else
        {
            messageLength = data[i+1];
            messageStartIndex = i + SHORT_TLV_SIZE;
        }
    }

    return true;
}

bool MifareClassic::write(NdefMessage& m, uint8_t * uid, unsigned int uidLength)
{

    uint8_t encoded[m.getEncodedSize()];
    m.encode(encoded);

    uint8_t buffer[getBufferSize(sizeof(encoded))];
    memset(buffer, 0, sizeof(buffer));

    #ifdef MIFARE_CLASSIC_DEBUG
    DMSG("sizeof(encoded) "));DMSG(sizeof(encoded);
    DMSG("sizeof(buffer) "));DMSG(sizeof(buffer);
    #endif

    if (sizeof(encoded) < 0xFF)
    {
        buffer[0] = 0x3;
        buffer[1] = sizeof(encoded);
        memcpy(&buffer[2], encoded, sizeof(encoded));
        buffer[2+sizeof(encoded)] = 0xFE; // terminator
    }
    else
    {
        buffer[0] = 0x3;
        buffer[1] = 0xFF;
        buffer[2] = ((sizeof(encoded) >> 8) & 0xFF);
        buffer[3] = (sizeof(encoded) & 0xFF);
        memcpy(&buffer[4], encoded, sizeof(encoded));
        buffer[4+sizeof(encoded)] = 0xFE; // terminator
    }

    // Write to tag
    int index = 0;
    int currentBlock = 4;
    uint8_t key[6] = { 0xD3, 0xF7, 0xD3, 0xF7, 0xD3, 0xF7 }; // this is Sector 1 - 15 key

    while (index < sizeof(buffer))
    {

        if (_nfcShield->mifareclassic_IsFirstBlock(currentBlock))
        {
            int success = _nfcShield->mifareclassic_AuthenticateBlock(uid, uidLength, currentBlock, 0, key);
            if (!success)
            {
                DMSG("Error. Block Authentication failed for ");DMSG_INT(currentBlock);
                return false;
            }
        }
        int write_success = _nfcShield->mifareclassic_WriteDataBlock (currentBlock, &buffer[index]);
        if (write_success)
        {
            #ifdef MIFARE_CLASSIC_DEBUG
            DMSG("Wrote block ");Serial.print(currentBlock);DMSG(" - ");
            _nfcShield->PrintHexChar(&buffer[index], BLOCK_SIZE);
            #endif
        }
        else
        {
            DMSG("Write failed ");DMSG_INT(currentBlock);
            return false;
        }
        index += BLOCK_SIZE;
        currentBlock++;

        if (_nfcShield->mifareclassic_IsTrailerBlock(currentBlock))
        {
            // can't write to trailer block
            #ifdef MIFARE_CLASSIC_DEBUG
            DMSG("Skipping block ");DMSG(currentBlock);
            #endif
            currentBlock++;
        }

    }

    return true;
}
