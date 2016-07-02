# RFIdea

For STM32F4X9 (Cortex M4) with DSI : quick RFID Pwner for French VIGIK system and Common centrals.
Work Straight on a STM32F469I-Discovery (With MIPI-DSI Screen) driving hardware SPI2.

## PN532
- Hardly modified for speed
- Contains unlockChina(); homemade function
- Needs Hardware Spi.
- Contradicts Touchscreen I2C1 via SS/SDA/NSS pin, making Clock Frenzy. Radical `delete ts;` upon PN532 needs.

## Keys and dumps
- dump.h not given here for obvious security reasons
- keys.h not given here for obvious security reasons
- dump.h format :
```C++
const int dumpnbr=5;
const uint8_t dump_blank[64][16]={
{  0x01, 0x23, 0x45, 0x67, 0x00, 0x08, 0x04, 0x00, 0x46, 0x59, 0x25, 0x58, 0x49, 0x10, 0x23, 0x02},
{  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x07, 0x80, 0x69, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
[...]
{  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x07, 0x80, 0x69, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}
};

//DUMP==1
const uint8_t dump_buro_colin[64][16] = {
 { 0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0x43, 0x26, 0x76, 0x6C, 0x00, 0x08, 0x06, 0x06},
{ 0x01, 0x11, 0x21, 0x31, 0x41, 0x51, 0x61, 0x71, 0x81, 0x91, 0xA1, 0xB1, 0xC1, 0xD1, 0xE1, 0xF1},
[...]
```
- keys.h format :
```C++
uint8_t defaultKeys[][6] = {
        {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // Default key
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // Blank key
        {0xXX, 0xXX, 0xXX ,0xXX, 0xXX, 0xXX}, // NORALSY DEFAULT ALARONism
        {0xXX, 0xXX, 0xXX ,0xXX, 0xXX, 0xXX}, // NORALSY DEFAULT BLARONism
        {0xxX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX}, // COGELEC / URMET-CAPTIV A/B DEFAULT
[...]
        {0x8f, 0xd0, 0xa4, 0xf2, 0x56, 0xe9}  // Defaults Transport Key 8
};
```


## UnlockChina();
- Unlock Chinese Mifare Cards by triggering Backdoor "Magic". 
- quick WriteRegister -> InCommunicateThru -> OpenBackdoor of Mifare card
- Backdoor opening is implemented by desactivating PN532 CRC control, Change in bit Framing,then InCommunicateThru : HALT+CRC, Unlock Command1, Unlock Command2.
- Write registers back to normal for the rest of communication
```C++

// Not actual, stripped example for those who are just here for this very sequence ;)
uint8_t PN532::unlockChina(void)
{
    uint8_t response;

    // We start from "Normal" Configuration :
    // Tx&Rx CRC is handled by PN532, Bit Framing and all registers are normal
    // Tag should already be selected and active (WUPA,REQed, Anticolled, INLISTPASSIVETARGETed, whatever)

    // Deactivate Tx/Rx CRC handling (we have to compute it ourselves, but still ONLY for commands to CARD
    // not PN532, and only when need.
    // Here in Fact will be only for first IncommunicateThrue "HALT" command

    pn532_packetbuffer[0] = 0x08; // WriteRegisters
    pn532_packetbuffer[1] = 0x63; // REG1 HIGH BYTE
    pn532_packetbuffer[2] = 0x03; // REG1 LOW BYTE
    pn532_packetbuffer[3] = 0x00; // FALSE
    pn532_packetbuffer[4] = 0x63; // REG2 HIGH BYTE
    pn532_packetbuffer[5] = 0x02; // REG2 LOW BYTE
    pn532_packetbuffer[6] = 0x00;  // FALSE

    HAL(writeCommand)(pn532_packetbuffer, 7); // Sends the command thrue Chose interface (SPI I guess, see PN532_Interface.h)
    HAL(readResponse)(pn532_packetbuffer, sizeof(pn532_packetbuffer)); //Response SHOULD be read, as when there is still data in FIFO, nothing new will work.

    pn532_packetbuffer[0] = 0x42; // InCommunicateThrue
    pn532_packetbuffer[1] = 0x50; // HALT
    pn532_packetbuffer[2] = 0x00; // +
    pn532_packetbuffer[3] = 0x57; // CRC
    pn532_packetbuffer[4] = 0xCD; // CRC 

    HAL(writeCommand)(pn532_packetbuffer, 5);
    HAL(readResponse)(pn532_packetbuffer, sizeof(pn532_packetbuffer));

    pn532_packetbuffer[0] = 0x08; // WriteRegister Set Bit Framing opt for step1
    pn532_packetbuffer[1] = 0x63;
    pn532_packetbuffer[2] = 0x3D; 
    pn532_packetbuffer[3] = 0x07; 

    HAL(writeCommand)(pn532_packetbuffer, 4);
    HAL(readResponse)(pn532_packetbuffer, sizeof(pn532_packetbuffer));

    pn532_packetbuffer[0] = 0x42; 
    pn532_packetbuffer[1] = 0x40; // OpenBackdoor, step1.

    HAL(writeCommand)(pn532_packetbuffer, 2);
    HAL(readResponse)(pn532_packetbuffer, sizeof(pn532_packetbuffer));

    pn532_packetbuffer[0] = 0x08; // WriteRegister Set Bit Framing opt for step2
    pn532_packetbuffer[1] = 0x63; 
    pn532_packetbuffer[2] = 0x3D; 
    pn532_packetbuffer[3] = 0x00; 

    HAL(writeCommand)(pn532_packetbuffer, 4);
    HAL(readResponse)(pn532_packetbuffer, sizeof(pn532_packetbuffer));

    pn532_packetbuffer[0] = 0x42; 
    pn532_packetbuffer[1] = 0x43; // Openbackdoor, Sstep2

    HAL(writeCommand)(pn532_packetbuffer, 2);
    HAL(readResponse)(pn532_packetbuffer, sizeof(pn532_packetbuffer));

    pn532_packetbuffer[0] = 0x08; // Tx/Rx CRC Handling back please
    pn532_packetbuffer[1] = 0x63; 
    pn532_packetbuffer[2] = 0x02; 
    pn532_packetbuffer[3] = 0x80; 
    pn532_packetbuffer[4] = 0x63;
    pn532_packetbuffer[5] = 0x03;
    pn532_packetbuffer[6] = 0x80; 

    HAL(writeCommand)(pn532_packetbuffer, 7);
    HAL(readResponse)(pn532_packetbuffer, sizeof(pn532_packetbuffer));

    //Optional
    pn532_packetbuffer[0] = 0x40; // Normal INDATAExchange
    pn532_packetbuffer[1] = 0x01; // TAG
    pn532_packetbuffer[2] = 0x30; // MIFARE_READ
    pn532_packetbuffer[3] = 0x00; // BLOCK 0

    HAL(writeCommand)(pn532_packetbuffer, 4);
    HAL(readResponse)(pn532_packetbuffer, sizeof(pn532_packetbuffer));

    return 0;
}

```
- Still this needs an activated target, so it cannot unbrick UID (unless you do some very-quick tag-replace ninja movement) Block 0 fail, for which you have to do the whole anticollision(with NUID Cascading up to level3).
