#include <NfcAdapter.h>
#include <PN532_debug.h>

NfcAdapter::NfcAdapter(PN532Interface &interface)
{
    shield = new PN532(interface);
}

NfcAdapter::~NfcAdapter(void)
{
    delete shield;
}

void NfcAdapter::begin()
{
    shield->begin();

    uint32_t versiondata = shield->getFirmwareVersion();
    if (! versiondata) {
        DMSG("Didn't find PN53x board");
        while (1); // halt
    }
    
    DMSG("Found chip PN5%2X\r\n", versiondata >> 24);
    DMSG("Firmware V%d.%d\r\n", (versiondata >> 16) & 0xFF, (versiondata >> 8) & 0xFF);

    // configure board to read RFID tags
    shield->SAMConfig();
}

bool NfcAdapter::tagPresent()
{
    uint8_t success;
    uidLength = 0;

    // TODO is cast of uidLength OK?
    success = shield->readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, (uint8_t*)&uidLength,&ATQA,&SAK);

    // if (success)
    // {
    //   DMSG("Found an ISO14443A card");
    //   DMSG("  UID Length: ");Serial.print(uidLength, DEC);DMSG(" uint8_ts");
    //   DMSG("  UID Value: ");
    //   shield->PrintHex(uid, uidLength);
    //   DMSG("");
    // }

    return success;
}

NfcTag NfcAdapter::read()
{

    uint8_t type = guessTagType();

    // TODO need an abstraction of Driver
    if (type == TAG_TYPE_MIFARE_CLASSIC)
    {
        #ifdef NDEF_DEBUG
        DMSG("Reading Mifare Classic");
        #endif
        MifareClassic mifareClassic = MifareClassic(*shield);
        return mifareClassic.read(uid, uidLength);
    }
    else if (type == TAG_TYPE_2)
    {
        #ifdef NDEF_DEBUG
        DMSG("Reading Mifare Ultralight");
        #endif
        MifareUltralight ultralight = MifareUltralight(*shield);
        return ultralight.read(uid, uidLength);
    }
    else if (type == TAG_TYPE_UNKNOWN)
    {
        DMSG("Can not determine tag type");
        //DMSG("Can not determine tag type for ATQA 0x");
        //Serial.print(atqa, HEX);DMSG(" SAK 0x");DMSG(sak, HEX);
        return NfcTag(uid, uidLength);
    }
    else
    {
        DMSG("No driver for card type ");
        DMSG_INT(type);
        // TODO should set type here
        return NfcTag(uid, uidLength);
    }

}

bool NfcAdapter::write(NdefMessage& ndefMessage)
{
    bool success;
    uint8_t type = guessTagType();
    if (uidLength == 4)
    {
        MifareClassic mifareClassic = MifareClassic(*shield);
        success = mifareClassic.write(ndefMessage, uid, uidLength);
    }
    else
    {
        DMSG("Ultralight Tag");
        MifareUltralight mifareUltralight = MifareUltralight(*shield);
        success= mifareUltralight.write(ndefMessage, uid, uidLength);
        //success = false;
    }
    return success;
}

// TODO this should return a Driver MifareClassic, MifareUltralight, Type 4, Unknown
// Guess Tag Type by looking at the ATQA and SAK values
// Need to follow spec for Card Identification. Maybe AN1303, AN1305 and ???
unsigned int NfcAdapter::guessTagType()
{

    // 4 uint8_t id - Mifare Classic
    //  - ATQA 0x4 && SAK 0x8
    // 7 uint8_t id
    //  - ATQA 0x44 && SAK 0x8 - Mifare Classic
    //  - ATQA 0x44 && SAK 0x0 - Mifare Ultralight NFC Forum Type 2
    //  - ATQA 0x344 && SAK 0x20 - NFC Forum Type 4
    DMSG("Guess type");
    DMSG("ATQA: 0x");  DMSG_HEX(ATQA);
    DMSG("SAK: 0x");  DMSG_HEX(SAK);
    /*if (uidLength == 4)
    {
        return TAG_TYPE_MIFARE_CLASSIC;
    }
    else
    {
        return TAG_TYPE_2;
    }*/
    if(ATQA==0x4&&SAK==0x8){
        return TAG_TYPE_MIFARE_CLASSIC;
    }
    if(ATQA==0x44&&SAK==0x0){
        return TAG_TYPE_2;
    }
    return TAG_TYPE_UNKNOWN;
}
