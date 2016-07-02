#ifndef NfcAdapter_h
#define NfcAdapter_h

#include <PN532Interface.h>
#include <PN532.h>
#include <NfcTag.h>
#include <Ndef.h>

// Drivers
#include <MifareClassic.h>
#include <MifareUltralight.h>

#define TAG_TYPE_MIFARE_CLASSIC (0)
#define TAG_TYPE_1 (1)
#define TAG_TYPE_2 (2)
#define TAG_TYPE_3 (3)
#define TAG_TYPE_4 (4)
#define TAG_TYPE_UNKNOWN (99)

#define IRQ   (2)
//#define RESET (3)  // Not connected by default on the NFC Shield

class NfcAdapter {
    public:
        NfcAdapter(PN532Interface &interface);

        ~NfcAdapter(void);
        void begin(void);
        bool tagPresent(); // tagAvailable
        NfcTag read();
        bool write(NdefMessage& ndefMessage);
        // FUTURE bool share(NdefMessage& ndefMessage);
        // FUTURE bool unshare();
        // FUTURE bool erase();
        // FUTURE bool format();
    private:
        PN532* shield;
        uint8_t uid[7];    // Buffer to store the returned UID
        unsigned int uidLength; // Length of the UID (4 or 7 uint8_ts depending on ISO14443A card type)
        unsigned int guessTagType();
        uint16_t ATQA;
        uint8_t SAK;
};

#endif
