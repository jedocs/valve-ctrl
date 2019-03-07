#include "../include/define.h"

#define FREQ_Band       0x0020                                 //868MHz
#define FREQ_start      710
#define FREQ_step       90

// rf input/output buffers:
unsigned char RxPacket[20]; // Receive data puffer (payload only)
unsigned char TxPacket[20]; // Transmit data puffer
unsigned char TxPacketPtr; // Next byte to send
unsigned char RxPacketPtr; // Next byte to send
unsigned char RxPacketLen; // Tx packet size

unsigned char daa[5];

BOOL hasPacket;

// IA4420 SPI commands:
const rom unsigned int Power_management_cmd = 0x8201; // Everything off, uC clk disabled   Power_management_cmd;
const rom unsigned int Receiver_control_cmd = 0x9481; // BW=135kHz, DRSSI=-97dBm, pin8=VDI, fast VDI   81      Receiver_control_cmd;
const rom unsigned int Txreg_write_cmd = 0xB800; //Txreg_write_cmd;
const rom unsigned int FIFO_cmd = 0xCA81; // Sync. latch cleared, limit=8bits, disable sensitive reset    FIFO_cmd;
const rom unsigned int Data_filter_cmd = 0xC22C; // Digital LPF (default)    Data_filter_cmd;
const rom unsigned int AFC_cmd = 0xC4F7; // Auto AFC (default)   AFC_cmd;
unsigned int Frequency_cmd = 0xA7D0; // (Fo=915.000MHz (default))?????   870.000    Frequency_cmd;
const rom unsigned int Data_rate_cmd = 0xC623; // 9579Baud (default)   Data_rate_cmd;
const rom unsigned int Transmitter_control_cmd = 0x9850; //9850                           // df=75kHz, Pmax, normal modulation polarity  Transmitter_control_cmd;
const rom unsigned int Configuration_cmd = 0x8007 | FREQ_Band; // Cload=12pF; TX registers, FIFO are disabled  Configuration_cmd;

unsigned char ii;

extern struct CANMessage RX_Message, TX_Message;

/*********************************************************************
 * WORD CRC16(  INPUT unsigned char * data,
 *              INPUT signed char dataLength,
 *              INPUT WORD initCRC)
 *
 * Overview:        This function generates 16-bit CRC code for the input
 *                  data, with initial CRC value
 * Output:
 *          WORD                    Generated CRC code
 ********************************************************************/
unsigned short int CRC16(unsigned char *ptr, signed char count) {
    unsigned short int crc;
    unsigned char i;

    crc = 0;
    while (--count >= 0) {
        crc = crc ^ ((unsigned short int) *ptr++ << 8);
        for (i = 0; i < 8; i++) {
            if (crc & 0x8000) {
                crc = (crc << 1) ^ 0x1021;
            } else {
                crc = crc << 1;
            }
        }
    }

    return crc;
}

//================================================================7
// HW SPI write

void SPIPut(unsigned char v) {
    unsigned char i;

    PIR1bits.SSPIF = 0;
    i = SSPBUF;
    do {
        SSPCON1bits.WCOL = 0;
        SSPBUF = v;
    } while (SSPCON1bits.WCOL);

    while (PIR1bits.SSPIF == 0);
}

//*********************************************************************

unsigned char SPIGet(void) {
    SPIPut(0x00);
    return SSPBUF;
}

//==============================================================
// SPI interface read                                           // read spidata from SPI
//--------------------------------------------------------------

unsigned char SPIGet_sw(void) {
    unsigned char i, spidata;
    SPI_SDO = 0;
    SPI_SCK = 0;
    for (i = 0; i < 8; i = i + 1) {
        spidata = spidata << 1;

        if (SPI_SDI) spidata |= 1; // to the Din pin

        SPI_SCK = 1;
        Delay10TCYx(3);
        SPI_SCK = 0;
    }
    return (spidata);
}

// SPI interface write                                          // send spidata via SPI
//--------------------------------------------------------------

void SPIPut_sw(unsigned int spidata) {
    unsigned char i;
    SPI_SDO = 0;
    SPI_SCK = 0;
    for (i = 0; i < 8; i = i + 1) {
        if (spidata & 0x80) SPI_SDO = 1;
        else SPI_SDO = 0;

        spidata = spidata << 1;

        SPI_SCK = 1;
        Delay10TCYx(3);
        SPI_SCK = 0;

    }
    SPI_SDO = 0;
}

// Send SPI command to the rf chip                              // send 16-bit SPI command with nSEL control
//--------------------------------------------------------------

void send_cmd(unsigned int spicmd) {
    nSEL = 0;
    SPIPut_sw(spicmd >> 8);
    SPIPut_sw(spicmd);
    nSEL = 1;
}

//==============================================================
// initialization
//==============================================================

void RF_init(void) {
    unsigned int ch;

    nFFS = 1;
    nSEL = 1;

    SDIdir = 1;
    SDOdir = 0;
    SCKdir = 0;
    SPI_SDO = 0;
    SPI_SCK = 0;
    SPI_SDI = 1;

    RxPacketLen = 0;
    RxPacketPtr = 0;
    hasPacket = FALSE;

    //----  configuring the RF link --------------------------------
    ch = 7; //1340
    Frequency_cmd = (0xA000 + FREQ_start + ((unsigned int) ch * FREQ_step));
    send_cmd(0xCC77); // PLL setting cmd. HOPE RF datasheet!!!!!!!!!!!!!!!!!!!!!!!!
    send_cmd(FIFO_cmd);
    send_cmd(FIFO_cmd | 0x0002); // enable synchron latch
    send_cmd(Configuration_cmd);
    send_cmd(Frequency_cmd);
    send_cmd(Power_management_cmd);
    send_cmd(Receiver_control_cmd);
    send_cmd(Transmitter_control_cmd);

    // antenna tuning on startup
    send_cmd(Power_management_cmd | 0x0020); // turn on the transmitter
    DelayMs(5); // ant.tuning time (~100us) + Xosc starup time (5ms)!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // end of antenna tuning

    send_cmd(Power_management_cmd | 0x0080); // turn off transmitter, turn on receiver
    send_cmd(Configuration_cmd | 0x0040); // enable the FIFO
    send_cmd(FIFO_cmd);
    send_cmd(FIFO_cmd | 0x0002); // enable synchron latch
    send_cmd(0x0000); // read status byte (read ITs)
}

/*********************************************************************
 * Function:        void HighISR(void)
 ********************************************************************/
void RFInt(void) {

    INTCON3bits.INT1IE = 0;
    INTCON3bits.INT1IF = 0;
    nSEL = 0; // Sorrend fontos!!!!!!! (nsel, sdidir, spi_sdi) (késleltetés miatt?)
    Nop(); //kell !!!!
    SDIdir = 1; //????????????????? nem kell?
    SPI_SDI = 1; //????????????????? nem kell?

    if (SPI_SDI) // There is data in RX FIFO
    {
        unsigned char RxPacketPtr, tmpPacketLen;
        unsigned short int counter, received_crc, calculated_crc;

        nSEL = 1;
        nFFS = 0; // FIFO selected
        tmpPacketLen = SPIGet_sw();

        if (tmpPacketLen >= 20 || tmpPacketLen == 0 || hasPacket) {
IGNORE_HERE:
            nFFS = 1; // bad packet len received
            send_cmd(Power_management_cmd); // turn off the transmitter and receiver
            send_cmd(FIFO_cmd); // reset FIFO
            send_cmd(Configuration_cmd); // disable FIFO, TX_latch
            send_cmd(Configuration_cmd | 0x0040); // enable FIFO
            send_cmd(Power_management_cmd | 0x0080); // turn on receiver
            send_cmd(FIFO_cmd | 0x0002); // FIFO synchron latch re-enabled
            goto RETURN_HERE;
        }

        RxPacketLen = tmpPacketLen;
        nFFS = 1;
        RxPacketPtr = 0;
        counter = 0;

        while (1) {
            if (counter++ == 0xFFFF) {
                goto IGNORE_HERE;
            } else if (FFIT == 1) {
                nFFS = 0;
                counter = 0;
                RxPacket[RxPacketPtr++] = SPIGet_sw();

                if (RxPacketPtr >= RxPacketLen) {
                    RxPacket[RxPacketPtr++] = 0;
                    RxPacket[RxPacketPtr++] = 0;
                    nFFS = 1;
                    send_cmd(FIFO_cmd);

                    RxPacketLen -= 2; // do not count CRC
                    received_crc = ((unsigned short int) RxPacket[RxPacketLen + 1])+(((unsigned short int) RxPacket[RxPacketLen] << 8));
                    calculated_crc = CRC16((unsigned char *) RxPacket, RxPacketLen);

                    if (((char) (calculated_crc >> 8) != RxPacket[RxPacketLen]) | ((char) (calculated_crc) != RxPacket[RxPacketLen + 1]))
                        //if( received_crc != calculated_crc )
                    {
                        RxPacketPtr = 0;
                        RxPacketLen = 0;
                        send_cmd(FIFO_cmd | 0x0002); // FIFO synchron latch re-enable
                        goto IGNORE_HERE;
                    }
                    send_cmd(FIFO_cmd | 0x0002);
                    hasPacket = TRUE;
                    goto RETURN_HERE;
                }
                nFFS = 1;
            }
        }
    } else // read the rest of the interrupts
    {
        SPIGet_sw();
        nSEL = 1;
    }
RETURN_HERE:
            INTCON3bits.INT1IE = 1;
}

/*********************************************************************
 * BOOL TxPacket(unsigned char TxPacketLen)
 *
 * Overview:
 *              This function send the packet in the buffer TxPacket
 * Input:
 *              unsigned char    TxPacketLen     The length of the packet to be
 ********************************************************************/
void Tx(unsigned char TxPacketLen) {
    unsigned short int crc;
    unsigned char TxPacketPtr, synCount;

    INTCON3bits.INT1IE = 0;

    if (TxPacketLen + 2 > 20) {
        TxPacketLen = 18; //??????????? PACKET_LEN-2 ???????
    }

    crc = CRC16(TxPacket, TxPacketLen);
    TxPacket[TxPacketLen++] = (crc >> 8);
    TxPacket[TxPacketLen++] = crc;
       
    send_cmd(Power_management_cmd);// Turn off receiver, enable the TX register
    send_cmd(Configuration_cmd | 0x0080);
    send_cmd(Power_management_cmd | 0x0020);// enable transmitter

    DelayMs(1); //?????????????????????????????????????????????

    TxPacketPtr = 0;
    synCount = 0;

    nSEL = 0;

    SPIPut_sw(0xB8); // FIFO write
    SPIPut_sw(0xAA); // 3rd preamble
    //nSEL = 0;
    while (TxPacketPtr < TxPacketLen + 2) // 2 dummy byte
    {
        if (SPI_SDI == 1) {

            if (TxPacketPtr == 0 && synCount < 3) {
                switch (synCount) {
                    case 0:
                        SPIPut_sw(0x2D); // first syn byte
                        break;
                    case 1:
                        SPIPut_sw(0xD4); // second syn byte
                        break;
                    case 2:
                        SPIPut_sw(TxPacketLen);
                        break;
                    default:
                        break;
                }
                synCount++;
            } else {
                SPIPut_sw(TxPacket[TxPacketPtr]);
                TxPacketPtr++;
            }
        }
    }
    nSEL = 1;
    
    send_cmd(Power_management_cmd | 0x0080);// Turn off the transmitter, disable the Tx register
    send_cmd(Configuration_cmd | 0x0040);

    send_cmd(FIFO_cmd | 0x0002);
    //status = SPIGet_sw();                     //  clock pulse to reset the IT latches ???????????????????????????????????????

    INTCON3bits.INT1IE = 1;
   
}



