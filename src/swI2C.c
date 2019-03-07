
#include "../include/define.h"
unsigned char dly=50;

void I2CStart(void) {
    CLOCK_HI;
    Delay10TCYx(dly); // user may need to modify based on Fosc
    DATA_LOW; // set pin to output to drive low
    Delay10TCYx(dly); // user may need to modify based on Fosc
}

void I2CReStart(void) {
    CLOCK_LOW; // set clock pin to output to drive low
    DATA_HI; // release data pin to float high
    Delay10TCYx(dly);
    CLOCK_HI; // release clock pin to float high
    Delay10TCYx(dly);
    DATA_LOW; // set data pin output to drive low
    Delay10TCYx(dly);
}

void I2CStop(void) {
    CLOCK_LOW; // set clock pin to output to drive low
    DATA_LOW; // set data pin output to drive low
    Delay10TCYx(dly);
    CLOCK_HI; // release clock pin to float high
    Delay10TCYx(dly);
    DATA_HI; // release data pin to float high
    Delay10TCYx(dly);
}
/**********************************************************************
 *     Function Name:    signed char SWWriteI2C(unsigned char data_out)*
 *     Return Value:     error condition if bus error occurred         *
 *     Parameters:       Single data byte for I2C bus.                 *
 *     Description:      This routine writes a single byte to the      *
 *                       I2C bus.                                      *
 **********************************************************************/
unsigned char WriteI2C(unsigned char data_out) {
    unsigned char i;
    for (i = 0; i < 8; i++) {
        // clock stretch?????

        if (data_out & 0b10000000) {
            CLOCK_LOW; // set clock pin output to drive low
            DATA_HI; // release data line to float high
            Delay10TCYx(dly); // user may need to modify based on Fosc
            CLOCK_HI; // release clock line to float high
            Delay10TCYx(dly); // user may need to modify based on Fosc
        } else // transmit out logic 0
        {
            CLOCK_LOW; // set clock pin output to drive low
            DATA_LOW; // set data pin output to drive low
            Delay10TCYx(dly); // user may need to modify based on Fosc
            CLOCK_HI; // release clock line to float high
            Delay10TCYx(dly); // user may need to modify based on Fosc
        }
        data_out = data_out << 1;
    }
    return ( 0); // return with no error
}

unsigned char ReadI2C(void) {
    unsigned char recv, kk;

    Delay10TCYx(dly);   //50!!!!!!!!!!!!44

    for (kk = 0; kk < 8; kk++) {
        CLOCK_LOW; // set clock pin output to drive low
        DATA_HI; // release data line to float high
        Delay10TCYx(dly);
        CLOCK_HI; // release clock line to float high
        Delay10TCYx(dly);

        recv <<= 1; // shift composed byte by 1
        if (DATA_PIN) recv |= 0x01; // set bit 0 to logic 1
    }
    return ( recv); // return with data
}

/********************************************************************
 *     Function Name:    void SWStopI2C(void)                        *
 *     Return Value:     void                                        *
 *     Parameters:       void                                        *
 *     Description:      Send I2C bus stop condition.                *
 ********************************************************************/


unsigned char I2CAck(void) {
    CLOCK_LOW; // set clock pin to output to drive low
    DATA_HI; // release data line to float high
    Delay10TCYx(dly);
    CLOCK_HI; // release clock line to float high
    Delay10TCYx(dly); // 1 cycle delay

    if (DATA_PIN) return ( 1); // return with acknowledge error// error if ack = 1, slave did not ack
    else return ( 0); // return with no error
}

void ACK(void) {
    CLOCK_LOW; // make clock pin output to drive low
    DATA_LOW; // make data pin output to drive low
    Delay10TCYx(dly);
    CLOCK_HI; // release clock line to float high
    Delay10TCYx(dly);
}

//********************************************** NACK *******************

void nACK(void) // initiate NOT ACK
{
    CLOCK_LOW; // make clock pin output to drive low
    DATA_HI; // release data line to float high
    Delay10TCYx(dly); // user may need to modify based on Fosc
    CLOCK_HI; // release clock line to float high
    Delay10TCYx(dly); // user may need to modify based on Fosc
}

/*****************************************************************
        I2CWriteByte(unsigned char addr, unsigned char byte)
 *****************************************************************/
void I2CWriteByte(unsigned char addr, unsigned char reg, unsigned char data) {
    char x;
    I2CStart();
    WriteI2C(addr | WrtCmd);
    x = I2CAck();
    WriteI2C(reg);
    x = I2CAck();
    WriteI2C(data);
    x = I2CAck();
    I2CStop();
}

/*****************************************************************
        int I2CReadByte(unsigned char addr))
 ******************************************************************/
unsigned char I2CReadByte(unsigned char addr, unsigned char reg) {
    unsigned char num, x;

    I2CStart();
    WriteI2C(addr | WrtCmd);
    x = I2CAck();
    WriteI2C(reg);
    x = I2CAck();

    I2CReStart();
    WriteI2C(addr | RdCmd);

    x = I2CAck();
    num = ReadI2C();

    nACK();
    I2CStop();
    return (num);
}

unsigned int I2CReadWord(unsigned char addr, unsigned char reg) {
    union Int_Char tmp;
    char x;

    I2CStart();
    WriteI2C(addr | WrtCmd);
    x = I2CAck();
    WriteI2C(reg);
    x = I2CAck();

    I2CReStart();
    WriteI2C(addr | RdCmd);

    x = I2CAck();

    tmp.Char[1] = ReadI2C();
    ACK();
    tmp.Char[0] = ReadI2C();

    nACK();
    I2CStop();
    return (tmp.Int);
}








