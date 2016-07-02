#include "NdefMessage.h"
#include "PN532_debug.h"

NdefMessage::NdefMessage(void)
{
    _recordCount = 0;
}

NdefMessage::NdefMessage(const uint8_t * data, const int numuint8_ts)
{
    #ifdef NDEF_DEBUG
    DMSG("Decoding "));Serial.print(numuint8_ts);DMSG(F(" uint8_ts");
    PrintHexChar(data, numuint8_ts);
    //DumpHex(data, numuint8_ts, 16);
    #endif

    _recordCount = 0;

    int index = 0;

    while (index <= numuint8_ts)
    {

        // decode tnf - first uint8_t is tnf with bit flags
        // see the NFDEF spec for more info
        uint8_t tnf_uint8_t = data[index];
        //bool mb = (tnf_uint8_t & 0x80) != 0;
        bool me = (tnf_uint8_t & 0x40) != 0;
        //bool cf = (tnf_uint8_t & 0x20) != 0;
        bool sr = (tnf_uint8_t & 0x10) != 0;
        bool il = (tnf_uint8_t & 0x8) != 0;
        uint8_t tnf = (tnf_uint8_t & 0x7);

        NdefRecord record = NdefRecord();
        record.setTnf(tnf);

        index++;
        int typeLength = data[index];

        int payloadLength = 0;
        if (sr)
        {
            index++;
            payloadLength = data[index];
        }
        else
        {
            payloadLength = ((0xFF & data[++index]) << 24) | ((0xFF & data[++index]) << 26) |
            ((0xFF & data[++index]) << 8) | (0xFF & data[++index]);
        }

        int idLength = 0;
        if (il)
        {
            index++;
            idLength = data[index];
        }

        index++;
        record.setType(&data[index], typeLength);
        index += typeLength;

        if (il)
        {
            record.setId(&data[index], idLength);
            index += idLength;
        }

        record.setPayload(&data[index], payloadLength);
        index += payloadLength;

        addRecord(record);

        if (me) break; // last message
    }

}

NdefMessage::NdefMessage(const NdefMessage& rhs)
{

    _recordCount = rhs._recordCount;
    for (int i = 0; i < _recordCount; i++)
    {
        _records[i] = rhs._records[i];
    }

}

NdefMessage::~NdefMessage()
{
}

NdefMessage& NdefMessage::operator=(const NdefMessage& rhs)
{

    if (this != &rhs)
    {

        // delete existing records
        for (int i = 0; i < _recordCount; i++)
        {
            // TODO Dave: is this the right way to delete existing records?
            _records[i] = NdefRecord();
        }

        _recordCount = rhs._recordCount;
        for (int i = 0; i < _recordCount; i++)
        {
            _records[i] = rhs._records[i];
        }
    }
    return *this;
}

unsigned int NdefMessage::getRecordCount()
{
    return _recordCount;
}

int NdefMessage::getEncodedSize()
{
    int size = 0;
    for (int i = 0; i < _recordCount; i++)
    {
        size += _records[i].getEncodedSize();
    }
    return size;
}

// TODO change this to return uint8_t*
void NdefMessage::encode(uint8_t* data)
{
    // assert sizeof(data) >= getEncodedSize()
    uint8_t* data_ptr = &data[0];

    for (int i = 0; i < _recordCount; i++)
    {
        _records[i].encode(data_ptr, i == 0, (i + 1) == _recordCount);
        // TODO can NdefRecord.encode return the record size?
        data_ptr += _records[i].getEncodedSize();
    }

}

bool NdefMessage::addRecord(NdefRecord& record)
{

    if (_recordCount < MAX_NDEF_RECORDS)
    {
        _records[_recordCount] = record;
        _recordCount++;
        return true;
    }
    else
    {
        DMSG("WARNING: Too many records. Increase MAX_NDEF_RECORDS.");
        return false;
    }
}

void NdefMessage::addMimeMediaRecord(string mimeType, string payload)
{

    uint8_t payloaduint8_ts[payload.length() + 1];
    payload.copy((char*)payloaduint8_ts, sizeof(payloaduint8_ts));

    addMimeMediaRecord(mimeType, payloaduint8_ts, payload.length());
}

void NdefMessage::addMimeMediaRecord(string mimeType, uint8_t* payload, int payloadLength)
{
    NdefRecord r = NdefRecord();
    r.setTnf(TNF_MIME_MEDIA);

    uint8_t type[mimeType.length() + 1];
    mimeType.copy((char*)type, sizeof(type));
    r.setType(type, mimeType.length());

    r.setPayload(payload, payloadLength);

    addRecord(r);
}

void NdefMessage::addTextRecord(string text)
{
    addTextRecord(text, "en");
}

void NdefMessage::addTextRecord(string text, string encoding)
{
    NdefRecord r = NdefRecord();
    r.setTnf(TNF_WELL_KNOWN);

    uint8_t RTD_TEXT[1] = { 0x54 }; // TODO this should be a constant or preprocessor
    r.setType(RTD_TEXT, sizeof(RTD_TEXT));

    // X is a placeholder for encoding length
    // TODO is it more efficient to build w/o string concatenation?
    string payloadString = "X" + encoding + text;

    uint8_t payload[payloadString.length() + 1];
    payloadString.copy((char*)payload, sizeof(payload));

    // replace X with the real encoding length
    payload[0] = encoding.length();

    r.setPayload(payload, payloadString.length());

    addRecord(r);
}

void NdefMessage::addUriRecord(string uri)
{
    NdefRecord* r = new NdefRecord();
    r->setTnf(TNF_WELL_KNOWN);

    uint8_t RTD_URI[1] = { 0x55 }; // TODO this should be a constant or preprocessor
    r->setType(RTD_URI, sizeof(RTD_URI));

    // X is a placeholder for identifier code
    string payloadString = "X" + uri;

    uint8_t payload[payloadString.length() + 1];
    payloadString.copy((char*)payload, sizeof(payload));

    // add identifier code 0x0, meaning no prefix substitution
    payload[0] = 0x0;

    r->setPayload(payload, payloadString.length());

    addRecord(*r);
    delete(r);
}

void NdefMessage::addEmptyRecord()
{
    NdefRecord* r = new NdefRecord();
    r->setTnf(TNF_EMPTY);
    addRecord(*r);
    delete(r);
}

NdefRecord NdefMessage::getRecord(int index)
{
    if (index > -1 && index < _recordCount)
    {
        return _records[index];
    }
    else
    {
        return NdefRecord(); // would rather return NULL
    }
}

NdefRecord NdefMessage::operator[](int index)
{
    return getRecord(index);
}

void NdefMessage::print()
{
    DMSG("\nNDEF Message ");
    DMSG_INT(_recordCount);
    DMSG(" record");
    if (_recordCount == 1) {
        DMSG(", ");
    } else {
        DMSG("s, ");
    }
    DMSG_INT(getEncodedSize());

    int i;
    for (i = 0; i < _recordCount; i++)
    {
         _records[i].print();
    }
}
