
#ifndef __PN532_SPI_H__
#define __PN532_SPI_H__

#include "mbed.h"
#include "PN532Interface.h"

class PN532_SPI : public PN532Interface
{
public:
    PN532_SPI(SPI &spi, PinName ss);
    PN532_SPI(SPI *spi, PinName ss);

    virtual void begin();
    virtual void wakeup();
    virtual int8_t writeCommand(const uint8_t *header, uint8_t hlen, const uint8_t *body, uint8_t blen);
    virtual int16_t readResponse(uint8_t buf[], uint8_t len, uint16_t timeout);
    virtual int16_t quickreadResponse(uint8_t buf[], uint8_t len, uint16_t timeout);

private:
    SPI        *_spi;
    DigitalOut  _ss;
    uint8_t     command;

    bool isReady();
    void writeFrame(const uint8_t *header, uint8_t hlen, const uint8_t *body, uint8_t blen);
    int8_t readAckFrame();

    inline void write(uint8_t data) {
        REVERSE_BITS_ORDER(data);
        _spi->write(data);
    }
    inline uint8_t read() {
        uint8_t data =  _spi->write(0);
        REVERSE_BITS_ORDER(data);
        return data;
    }
    inline uint8_t qread() {
        uint8_t data =  _spi->write(0);
        return data;
    }
};

#endif
