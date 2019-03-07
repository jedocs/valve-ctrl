#include	<p18f4680.h>					// Register definitions#include	"MCP23X17.H"
#include 	<delays.h>
#include 	<timers.h>
#include <stdlib.h>
#include <string.h>
#include "../include/Console.h"
#include "../include/Can.h"
#include "../include/GenericTypeDefs.h"

typedef enum _BOOL {
    FALSE = 0, TRUE
} BOOL;

#define VALVE_OPEN 0
#define VALVE_CLOSE 1

#define CLOCK_FREQ          16000000
#define GetInstructionClock()   (CLOCK_FREQ / 4)
#define ENABLE_CONSOLE
/**************************************************************/

// TCN75 register/command/var definitions
#define TEMP   	0x00
#define CONFIG 	0x01
#define THYST  	0x02
#define TSET  	0x03

#define RdCmd 1
#define WrtCmd 0

union Int_Char {
    int Int;
    char Char [2];
};

//*************** I2C defs *************************************************

#define  DATA_LOW   TRISCbits.TRISC0 = 0; // define macro for data pin output
#define  DATA_HI    TRISCbits.TRISC0 = 1; // define macro for data pin input
#define  DATA_LAT   LATCbits.LATC0        // define macro for data pin latch
#define  DATA_PIN   PORTCbits.RC0         // define macro for data pin

#define  CLOCK_LOW  TRISCbits.TRISC1 = 0; // define macro for clock pin output
#define  CLOCK_HI   TRISCbits.TRISC1 = 1; // define macro for clock pin input
#define  SCLK_LAT   LATCbits.LATC1        // define macro for clock pin latch
#define  SCLK_PIN   PORTCbits.RC1         // define macro for clock pin

//******************* uC IO defs ***********************************************

#define SPI_SDO	PORTCbits.RC5 
#define SPI_SDI	PORTCbits.RC4
#define SPI_SCK	PORTCbits.RC3
#define SDOdir	TRISCbits.TRISC5 
#define SDIdir	TRISCbits.TRISC4
#define SCKdir	TRISCbits.TRISC3

#define AI0		PORTAbits.RA0
#define	AI1		PORTAbits.RA1		
#define	AI2		PORTAbits.RA2
#define AI3		PORTAbits.RA3
#define AI4		PORTAbits.RA5
#define AI5		PORTEbits.RE0
#define	AI6		PORTEbits.RE1		
#define	AI7		PORTEbits.RE2

#define DE		PORTDbits.RD3   //RS485 driver enable
#define RE		PORTDbits.RD4   //RS485 receive enable

#define LIMIT_SW	PORTEbits.RE3   //valve motor limit sw

// motor / limit sw addressing
#define A0		PORTDbits.RD5   
#define A1		PORTDbits.RD6
#define A2		PORTDbits.RD7

//motor phases
#define PHASE1	PORTBbits.RB7
#define PHASE2	PORTBbits.RB6
#define PHASE3	PORTBbits.RB5
#define PHASE4	PORTBbits.RB4


#define nFFS	PORTDbits.RD2
#define nSEL	PORTDbits.RD1
#define FFIT	PORTBbits.RB0
#define nIRQ	PORTBbits.RB1

#define RELAY	PORTDbits.RD0   
#define CS	PORTCbits.RC2   //CS for electronic pot.

//typedef union tagRTCC {

typedef struct tagRTCC {
    unsigned char sec;
    unsigned char min;
    unsigned char hr;
    unsigned char wkd;
    unsigned char day;
    unsigned char mth;
    unsigned char yr;
    //  };
} RTCC;

typedef struct pos {
    int act_pos;
    int req_pos;
    int zero_err;
    char phase;
} POS;

/*****   COMMON FUNCTION PROTOTYPES   *****/


void Setup(void); // Configures peripherals and variables
void RFInt(void);
void RF_init(void);
void Delay10us(unsigned long int tenMicroSecondCounter);
void DelayMs(unsigned short int ms);
void ZeroRun(unsigned char index);

void I2CStart(void); // Generate bus start condition
unsigned char I2CAck(void);
unsigned char WriteI2C(unsigned char data_out);
void Init_Sensors(void);
//void ReadTemperature(unsigned char ch);
void ACK(void);
void nACK(void);
void I2CStop(void); // Generate bus stop condition
void I2CRestart(void); // Generate bus restart condition
unsigned char ReadI2C(void);
void I2CReStart(void);
void I2CWriteByte(unsigned char addr, unsigned char reg, unsigned char data);
unsigned char I2CReadByte(unsigned char addr, unsigned char reg);
unsigned int I2CReadWord(unsigned char addr, unsigned char reg);
//char *itoa(auto int value, auto char *s); //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! vagy include stdlib.h
void Tx(unsigned char TxPacketLen);
void DoCommand(void);
void cmd_interpreter(void);
void attach_RFnode(void);
void receive_RFdata(void);
void CANDataTx(void);
void ConsoleWriteTime(void);
void CANReceive(void);

