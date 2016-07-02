#ifndef NdefMessage_h
#define NdefMessage_h

#include <Ndef.h>
#include <NdefRecord.h>

#define MAX_NDEF_RECORDS 4

class NdefMessage
{
    public:
        NdefMessage(void);
        NdefMessage(const uint8_t *data, const int numuint8_ts);
        NdefMessage(const NdefMessage& rhs);
        ~NdefMessage();
        NdefMessage& operator=(const NdefMessage& rhs);

        int getEncodedSize(); // need so we can pass array to encode
        void encode(uint8_t *data);

        bool addRecord(NdefRecord& record);
        void addMimeMediaRecord(string mimeType, string payload);
        void addMimeMediaRecord(string mimeType, uint8_t *payload, int payloadLength);
        void addTextRecord(string text);
        void addTextRecord(string text, string encoding);
        void addUriRecord(string uri);
        void addEmptyRecord();

        unsigned int getRecordCount();
        NdefRecord getRecord(int index);
        NdefRecord operator[](int index);

        void print();
    private:
        NdefRecord _records[MAX_NDEF_RECORDS];
        unsigned int _recordCount;
};

#endif
