//    @author   Speed, Mifare backdoor and more by Colin J. Brigato

#include "PN532_SPI.h"
#include "PN532_debug.h"

#define STATUS_READ     2
#define DATA_WRITE      1
#define DATA_READ       3

PN532_SPI::PN532_SPI(SPI &spi, PinName ss) : _ss(ss)
{
    command = 0;
    _spi = &spi;
    _spi->format(8, 0);
    _spi->frequency(4000000);

    _ss  = 1;
}

PN532_SPI::PN532_SPI(SPI *spi, PinName ss) : _ss(ss)
{
    command = 0;
    _spi = spi;
    _spi->format(8, 0);
    _spi->frequency(4000000);

    _ss  = 1;
}

void PN532_SPI::begin()
{

}

void PN532_SPI::wakeup()
{
    _ss = 0;
    //wait_ms(2);
    wait_us(1000);
    _ss = 1;
}

int8_t PN532_SPI::writeCommand(const uint8_t *header, uint8_t hlen, const uint8_t *body, uint8_t blen)
{
    command = header[0];
    writeFrame(header, hlen, body, blen);

    uint8_t timeout = PN532_ACK_WAIT_TIME;// * 1000;
    timeout=timeout*5000;
    while (!isReady()) {
        //wait_ms(1);
	wait_us(1);
        timeout--;
        if (0 == timeout) {
            DMSG("Time out when waiting for ACK\n");
            return -2;
        }
    }
    if (readAckFrame()) {
        DMSG("Invalid ACK\n");
        return PN532_INVALID_ACK;
    }
    return 0;
}

int16_t PN532_SPI::readResponse(uint8_t buf[], uint8_t len, uint16_t timeout)
{
    uint16_t time = 0;
    timeout = timeout * 1000;
    while (!isReady()) {
        //wait_ms(1);
	wait_us(1);
        time++;
        if (timeout > 0 && time > timeout) {
            return PN532_TIMEOUT;
        }
    }

    _ss = 0;
    wait_ms(1);
    //wait_us(500);

    int16_t result;
    do {
        write(DATA_READ);

        if (0x00 != read()      ||       // PREAMBLE
                0x00 != read()  ||       // STARTCODE1
                0xFF != read()           // STARTCODE2
           ) {

            result = PN532_INVALID_FRAME;
            break;
        }

        uint8_t length = read();
        if (0 != (uint8_t)(length + read())) {   // checksum of length
            result = PN532_INVALID_FRAME;
            break;
        }

        uint8_t cmd = command + 1;               // response command
        if (PN532_PN532TOHOST != read() || (cmd) != read()) {
            result = PN532_INVALID_FRAME;
            break;
        }

        DMSG("read:  ");
        DMSG_HEX(cmd);

        length -= 2;
        if (length > len) {
            for (uint8_t i = 0; i < length; i++) {
                DMSG_HEX(read());                 // dump message
            }
            DMSG("\nNot enough space\n");
            read();
            read();
            result = PN532_NO_SPACE;  // not enough space
            break;
        }

        uint8_t sum = PN532_PN532TOHOST + cmd;
        for (uint8_t i = 0; i < length; i++) {
            buf[i] = read();
            sum += buf[i];

            DMSG_HEX(buf[i]);
        }
        DMSG("\n");

        uint8_t checksum = read();
        if (0 != (uint8_t)(sum + checksum)) {
            DMSG("checksum is not ok\n");
            result = PN532_INVALID_FRAME;
            break;
        }
        read();         // POSTAMBLE

        result = length;
    } while (0);
    _ss = 1;

    return result;
}

bool PN532_SPI::isReady()
{
    _ss = 0;
    wait_us(1);
    write(STATUS_READ);
    uint8_t status = read() & 1;
    _ss = 1;
    return status;
}

void PN532_SPI::writeFrame(const uint8_t *header, uint8_t hlen, const uint8_t *body, uint8_t blen)
{
    _ss = 0;
    //wait_ms(2);               // wake up PN532
    wait_us(500);
    write(DATA_WRITE);
    write(PN532_PREAMBLE);
    write(PN532_STARTCODE1);
    write(PN532_STARTCODE2);

    uint8_t length = hlen + blen + 1;   // length of data field: TFI + DATA
    write(length);
    write(~length + 1);         // checksum of length

    write(PN532_HOSTTOPN532);
    uint8_t sum = PN532_HOSTTOPN532;    // sum of TFI + DATA

    DMSG("write: ");

    for (uint8_t i = 0; i < hlen; i++) {
        write(header[i]);
        sum += header[i];

        DMSG_HEX(header[i]);
    }
    for (uint8_t i = 0; i < blen; i++) {
        write(body[i]);
        sum += body[i];
        
        DMSG_HEX(header[i]);
    }

    uint8_t checksum = ~sum + 1;        // checksum of TFI + DATA
    write(checksum);
    write(PN532_POSTAMBLE);

    _ss = 1;

    DMSG("\n");
}

int8_t PN532_SPI::readAckFrame()
{
    const uint8_t PN532_ACK[] = {0, 0, 0xFF, 0, 0xFF, 0};

    uint8_t ackBuf[sizeof(PN532_ACK)];

    _ss = 0;
    //wait_ms(1);
    //wait_us(500);
    write(DATA_READ);

    for (uint8_t i = 0; i < sizeof(PN532_ACK); i++) {
        ackBuf[i] = read();
    }

    _ss = 1;

    return memcmp(ackBuf, PN532_ACK, sizeof(PN532_ACK));
}


int16_t PN532_SPI::quickreadResponse(uint8_t buf[], uint8_t len, uint16_t timeout)
{
    uint16_t time = 0;
    timeout = timeout * 100;
    while (!isReady()) {
        //wait_ms(1);
        wait_us(1);
        time++;
        if (timeout > 0 && time > timeout) {
            return PN532_TIMEOUT;
    }
 }
    _ss = 0;
    //_ss = 0;
    //wait_ms(1);
//    wait_us(500);

    int16_t result;

    do {
        write(DATA_READ);
     if (0x00 != qread()      ||       // PREAMBLE
                0x00 != qread()  ||       // STARTCODE1
                0xFF != qread()           // STARTCODE2
           ) {

        }

        uint8_t length = read();
	qread();

        uint8_t cmd = command + 1;               // response command
	qread();
	qread();

        DMSG("read:  ");
        DMSG_HEX(cmd);

        length -= 2;
        if (length > len) {
            qread();
            qread();
        }

        uint8_t sum = PN532_PN532TOHOST + cmd;
        for (uint8_t i = 0; i < length; i++) {
            buf[i] = qread();
            sum += buf[i];
        }
        DMSG("\n");

        //uint8_t checksum = qread();
        //if (0 != (uint8_t)(sum + checksum)) {
        //    DMSG("checksum is not ok\n");
        //}
        qread();         // POSTAMBLE

        result = length;
    } while (0);
    _ss = 1;

    return result;
}

