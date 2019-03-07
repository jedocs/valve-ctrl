#include "../include/define.h"

// PIC18F4680 Configuration Bit Settings

// CONFIG1H
#pragma config OSC = HS         // Oscillator Selection bits (HS oscillator)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit 
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit 
// CONFIG2L
#pragma config PWRT = ON        // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOREN = BOHW     // Brown-out Reset Enable bits (Brown-out Reset 
//enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 3         //Brown-out Reset Voltage bits(VBOR set to 2.1V)
// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Selectbits (1:32768)
// CONFIG3H
#pragma config PBADEN = OFF     // PORTB A/D Enable bit 
#pragma config LPT1OSC = OFF    // Low-Power Timer 1 Oscillator Enable bit 
#pragma config MCLRE = OFF      // MCLR Pin Enable bit 
// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit 
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit 
#pragma config BBSIZ = 1024     // (1K words (2K bytes) Boot Block)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit 
// CONFIG5L
#pragma config CP0 = OFF        //(Block 0 (000800-003FFFh) not code-protected)
#pragma config CP1 = OFF        //Block 1 (004000-007FFFh) not code-protected)
#pragma config CP2 = OFF        // Block 2 (008000-00BFFFh) not code-protected)
#pragma config CP3 = OFF        // Block 3 (00C000-00FFFFh) not code-protected)
// CONFIG5H
#pragma config CPB = OFF        //Boot block (000000-0007FFh) not code-protected
#pragma config CPD = OFF        //Data EEPROM not code-protected)
// CONFIG6L
#pragma config WRT0 = OFF       //Block 0 (000800-003FFFh) not write-protected)
#pragma config WRT1 = OFF       //004000-007FFFh) not write-protected)
#pragma config WRT2 = OFF       //Block 2 (008000-00BFFFh) not write-protected)
#pragma config WRT3 = OFF       //Block 3 (00C000-00FFFFh) not write-protected)
// CONFIG6H
#pragma config WRTC = OFF       //Configuration registers not write-protected)
#pragma config WRTB = OFF       //Boot block not write-protected)
#pragma config WRTD = OFF       //(Data EEPROM not write-protected)
// CONFIG7L
#pragma config EBTR0 = OFF      //n other blocks)
#pragma config EBTR1 = OFF      //d from table reads executed in other blocks)
#pragma config EBTR2 = OFF      //ed from table reads executed in other blocks)
#pragma config EBTR3 = OFF      //ed from table reads executed in other blocks)
// CONFIG7H
#pragma config EBTRB = OFF      //d from table reads executed in other blocks)

//---------------------------------------------------------------------
// Variable declarations
//---------------------------------------------------------------------

extern BOOL hasPacket;
unsigned char sensoraddr, valve_index, valve_i;
extern unsigned char TxPacket[20], RxPacket[20], RxPacketLen;
RTCC _time;
POS valve[6];

char data[20];
union Int_Char Temperature[10], Humidity[10], TempBoiler[10];
unsigned char  ccc[10], udata, buf_index, com_parameter, iii, Daymax;
const rom char out[] = {0b00010000, 0b00110000, 0b00100000, 0b01100000, 0b01000000, 0b11000000, 0b10000000, 0b10010000};
const rom char SensorAddress[] = {0b10010000, 0b10010010, 0b10010100, 0b10010110, 0b10011000, 0b10011010, 0b10011100, 0b10011110, 0b11100000};
const char MonthDaymax[] =
        //jan feb mar apr may jun jul aug sep oct nov dec
{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

const rom char ready[] = "\r\n\nReady\r\n";
const rom char error[] = "\r\n\nError\r\n}";

struct { // Holds status bits
    unsigned dir : 1;
    unsigned onemin : 1;
    unsigned time_to_tx : 1;
    unsigned d : 1;
    unsigned h : 1;
    unsigned a : 1;
    unsigned temprea : 1;
    unsigned valve_run : 1;
} flag;

char phase, secflag = 0;
unsigned int step = 0, tav = 0, div500 = 500, temp_value;
int tav1 = 0, tempp;
char inpbuf[10], comcount;
struct CANMessage RX_Message, TX_Message;
void CAN_PCconn(void);

//---------------------------------------------------------------------´
// Interrupt Code
//---------------------------------------------------------------------

#pragma interrupt LowISR

void LowISR(void) {
    if (INTCON3bits.INT1IF) RFInt();
    else if (PIR3) CANISR();
}

#pragma interrupt HighISR

void HighISR(void) {

    if (PIR1bits.TMR2IF) {

        PIR1bits.TMR2IF = 0;

        div500--;
        if (!div500) {
            div500 = 500;

            _time.sec += 1;

            if (_time.sec == 15) flag.time_to_tx = 1;
            if (_time.sec > 59) {
                _time.sec = 0;
                _time.min += 1;

                flag.onemin = 1;

                if (_time.min > 59) {
                    _time.min = 0;
                    _time.hr += 1;

                    if (_time.hr > 23) {
                        _time.hr = 0;
                        _time.day += 1;

                        Daymax = MonthDaymax[_time.mth - 1];

                        // February has one day more for a leap year
                        if (_time.mth == 2)
                            if ((_time.yr % 4) == 0)
                                Daymax++;

                        if (_time.day > Daymax) {
                            _time.day = 1;
                            _time.mth += 1;

                            if (_time.mth > 12) {
                                _time.mth = 1;
                                _time.yr += 1;

                            }
                        }
                    }
                }
            }
        }
        //*************** RTCC END *************************************

        if (flag.valve_run) {

            PORTD &= 0b00011111;
            PORTD |= (valve_index << 5);

            if (valve[valve_index].act_pos == valve[valve_index].req_pos) {
                flag.valve_run = 0;
                //    valve[valve_index].zero_err=0;
            } else {
                if ((valve[valve_index].req_pos - valve[valve_index].act_pos) > 0) { //open

                    valve[valve_index].phase++;
                    if (valve[valve_index].phase > 7) valve[valve_index].phase = 0;
                    LATB = ((LATB & 0x0f) | out[valve[valve_index].phase]);
                    valve[valve_index].act_pos += 1;

                } else if (!LIMIT_SW) {
                    flag.valve_run = 0;
                    valve[valve_index].zero_err = (valve[valve_index].act_pos - valve[valve_index].req_pos);
                } else { //if not on limit: close

                    valve[valve_index].phase--;
                    if (valve[valve_index].phase < 0) valve[valve_index].phase = 7;
                    LATB = ((LATB & 0x0f) | out[valve[valve_index].phase]);
                    valve[valve_index].act_pos -= 1;

                }
            }
        }
    }
}
/***********************************************************************/

#pragma code highVector=0x08

void HighVector(void) {
    _asm goto HighISR _endasm
}

#pragma code lowVector=0x18

void lowVector(void) {
    _asm GOTO LowISR _endasm
}

#pragma code /* return to the default code section */
//---------------------------------------------------------------------
//	Setup() initializes program variables and peripheral registers
//---------------------------------------------------------------------

void Setup(void) {
    char ii;

    TRISA = 0b11111111;
    PORTA = 0;
    TRISB = 0b00001011;
    PORTB = 0;
    TRISC = 0b10011001; //TRISC3 = 1 !!!
    PORTC = 0b00000100;
    TRISD = 0b00000000;
    PORTD = 0b11110000;
    TRISE = 0b00000111; // TRISE bit4: PSPMODE !!!!!!!!!!!!
    PORTE = 0b00000000;

    CMCON = 0b00000111; //comparator off (46k80-nál más!!!?)

    ADCON0 = 0; // A/D off
    ADCON1 = 0b00000111; // an0-7 analog, an 8-10 dig 46k80-nál más!!!!!!!!!!
    ADCON2 = 0b00111101; // Fosc/16, 20Tad

    A0 = 0;
    A1 = 0;
    A2 = 0;

    DE = 1;
    RE = 0;

    _time.mth = 1;
    _time.day = 1;
    _time.hr = 0;
    _time.yr = 0;

    for (ii = 0; ii < 9; ii++) {
        Temperature[ii].Int = 0x504;
        Humidity[ii].Int = 0x102;
        TempBoiler[ii].Int = 0;
        ccc[ii] = 10;
    }

    for (ii = 0; ii < 6; ii++) {
        valve[ii].act_pos = 0;
        valve[ii].req_pos = 0;
        valve[ii].phase = 0;
        valve[ii].zero_err = 0;
    }
    flag.valve_run = 0;

    PIE1 = 0; //peripherial interrupt disabled
    PIE2 = 0; //peripherial interrupt disabled
    PIE3 = 0; //peripherial interrupt disabled,CAN int a CANOpen()-ben eng.ve
    PR2 = 249;
    OpenTimer2(T2_PS_1_16 & T2_POST_1_2 & TIMER_INT_ON); // 500Hz int

    RCONbits.IPEN = 1; //int priority enabled
    IPR1bits.TMR2IP = 1; // TMR2 priority: high
    PIE1bits.TMR2IE = 1; // enable TMR2 int
    INTCON2bits.INTEDG1 = 0; //INT1 int on falling edge
    INTCON3bits.INT1IP = 0; //INT1 priority low
    INTCON3bits.INT1IE = 1; //enable INT1 int (RF module)!!!!!!!!!!!!!!!!!!!!!4
    IPR3 = 0; //CAN int priority low
    INTCONbits.GIEH = 1; //enable HIGH int
    INTCONbits.GIEL = 1; //enable LOW int
}
//---------------------------------------------------------------------
// main()
//---------------------------------------------------------------------

void main(void) {

    Setup();
    Init_Sensors();
    ConsoleInit();
    RF_init();
    CANOpen(CAN_CONFIG_1, CAN_CONFIG_2, CAN_CONFIG_3); //Initialize CAN module

    ConsolePutROMString("szelep teszt\r\n");
    if (CANRXMessageIsPending()) RX_Message = CANGet(); //Get the message
    CAN_PCconn();

    while (1) {

        if (hasPacket) {
            if ((RxPacket[0]&0b11110000) == 0b10100000) attach_RFnode();
            if ((RxPacket[0]&0b11110000) == 0b00110000) receive_RFdata();
        }

        if (flag.onemin) ConsoleWriteTime();
        if (flag.time_to_tx) CANDataTx();
        if (PIR1bits.RCIF) cmd_interpreter(); // Check for USART interrupt
    }
}

/****************************************************************************/

void ZeroRun(unsigned char index) {

    flag.valve_run = 0;
    valve_index = index;
    PORTD &= 0b00011111;
    PORTD |= (valve_index << 5);
    ConsolePutROMString("\r\nzero run, motor: ");
    btoa((valve_index + 1), data);
    ConsolePutString(data);

    DelayMs(5);

    if (!LIMIT_SW) { //valve closed, on limit

        ConsolePutROMString("\r\nlimiten van");
        valve[valve_index].act_pos = 0;
        valve[valve_index].req_pos = 5120;
        flag.valve_run = 1; //run off limit

        while (flag.valve_run && !LIMIT_SW);
        flag.valve_run=0;
        DelayMs(1);//????????????????????????
        if (!LIMIT_SW) { //if still on limit
            ConsolePutROMString("\r\n nem jön le limitröl...");
            goto TOVABB;
        }
    }
    valve[valve_index].act_pos = 20480; //2mm ?
    valve[valve_index].req_pos = 0;
    flag.valve_run = 1; //find limit

    while (flag.valve_run);

    if (!LIMIT_SW) {
        valve[valve_index].act_pos = 0;
        ConsolePutROMString("\r\n zero ok, ");
        itoa(valve[valve_index].zero_err, data);
        ConsolePutString(data);
    } else ConsolePutROMString("\r\n nem talalom a limitet");

TOVABB:
    ;
}

/********************************************************************/

void Init_Sensors(void) {

    SCLK_LAT = 0; //?????????????????????????????????????
    DATA_LAT = 0; //????????????????????????????????????
    //  I2CWriteByte(Temp[8].Address, CONFIG, 0b00000000);
    //for (x = 0; x < 8; x++) {
    //  I2CWriteByte(Temp[x].Address, CONFIG, 0b01100000);
    //}
}

/*********************************************************************/

void CAN_PCconn(void) {
    unsigned int i;

    TX_Message.Address = 0x111; //Set the address to My identifier
    TX_Message.Ext = 0; //If the identifier is standard, clear the Ext flag
    TX_Message.NoOfBytes = 0; //Set number of bytes to send
    TX_Message.Remote = 1;
    TX_Message.Priority = 0; //Internal CAN module priority 0-> least priority, 3-> most priority
    CANPut(TX_Message); //Put the message in the FIFO

    for (i = 0; i < 65535; i++) if (CANRXMessageIsPending()) break; //wait

    if (CANRXMessageIsPending()) {

        RX_Message = CANGet(); //Get the message

        _time.sec = RX_Message.Data[1]; //sec
        _time.min = RX_Message.Data[2]; //min
        _time.hr = RX_Message.Data[3]; //hr
        _time.day = RX_Message.Data[5];
        _time.mth = RX_Message.Data[6]; //mth
        _time.yr = RX_Message.Data[7]; //yr

        ConsolePutROMString("\r\n diff: ");
        itoa((i), data);
        ConsolePutString(data);

    } else {
        //error
    }
}

/*************************************cmd intrtpreter******************************/

void cmd_interpreter(void) {

    switch (udata = RCREG) {

        case 'w':
            flag.valve_run = 0;
            valve_index = 1;
            valve[valve_index].req_pos = 0;
            valve[valve_index].act_pos = 10000;

            flag.valve_run = 1;
            break;

        case ',': DoCommand(); // process the string
            memset(inpbuf, 0, 8); // clear the input buffer
            buf_index = 0; // clear the buffer index
            comcount++; // increment comma count
            ConsolePut(udata); // echo the character
            break;

        case 0x0d: DoCommand(); // process the string
            memset(inpbuf, 0, 8); // clear the input buffer
            buf_index = 0; // clear the buffer index
            comcount = 0; // clear comma count
            ConsolePutROMString(ready); // put prompt to USART
            break;

        default: inpbuf[buf_index] = udata; // get received char
            buf_index++; // increment buffer index
            if (buf_index > 7) // If more than 8 chars
            { // received before getting
                ConsolePutROMString(ready); // a <CR>, clear input
                memset(inpbuf, 0, 8); // buffer
                buf_index = 0; // the buffer index
            } else ConsolePut(udata); // echo character
            break; //
    }
}

/***********************************************************************/

void DoCommand(void) {
    if (comcount == 0) // If this is the first parameter of the input
    { // command...
        switch (inpbuf[0]) {

            case 'g': com_parameter = 'g';
                break;
            case 'z': com_parameter = 'z';
                break;

            case 'q':
                ConsolePutROMString("\r\n\n");
                for (valve_i = 0; valve_i < 6; valve_i++) {

                    ConsolePutROMString("\r\nszelep ");
                    btoa((valve_i + 1), data);
                    ConsolePutString(data);
                    ConsolePutROMString(" , act. pos.: ");
                    itoa(valve[valve_i].act_pos, data);
                    ConsolePutString(data);
                    ConsolePutROMString(" , req. pos.: ");
                    itoa(valve[valve_i].req_pos, data);
                    ConsolePutString(data);
                    ConsolePutROMString(" , zero err.: ");
                    itoa(valve[valve_i].zero_err, data);
                    ConsolePutString(data);
                    ConsolePutROMString(" , phase: ");
                    itoa(valve[valve_i].phase, data);
                    ConsolePutString(data);
                }
                break;

            default: if (inpbuf[0] != '\0') {
                    ConsolePutROMString(error);
                }
                break;
        }
    } else if (comcount == 1) // If this is the second parameter of the
    { // input command.
        switch (com_parameter) {
            case 'g':
                valve_i = atob(inpbuf);
                if (valve_i < 1 | valve_i > 6) {
                    memset(inpbuf, 0, 8); // clear the input buffer
                    buf_index = 0; // clear the buffer index
                    comcount = 0;
                    com_parameter = 0;
                    ConsolePutROMString("\r\nHibás paraméter!");
                    ConsolePutROMString(ready);
                }
                break;

            case 'z': valve_i = atob(inpbuf);

                //ellen?rizni hogy megy-e motor
                if (valve_i == 0) for (valve_i = 0; valve_i < 6; valve_i++) ZeroRun(valve_i);
                else if (valve_i > 0 && valve_i < 7) ZeroRun(valve_i - 1);
                else {
                    memset(inpbuf, 0, 8); // clear the input buffer
                    buf_index = 0; // clear the buffer index
                    comcount = 0;
                    com_parameter = 0;
                    ConsolePutROMString("\r\nHibás paraméter!");
                    ConsolePutROMString(ready);
                }
                break;

            default: break;
        }
    } else if (comcount == 2) {
        switch (com_parameter) {

            case 'g': temp_value = atoi(inpbuf); // Get value for
                if ((temp_value < 0) | (temp_value > 20480)) {
                    memset(inpbuf, 0, 8); // clear the input buffer
                    buf_index = 0; // clear the buffer index
                    comcount = 0;
                    com_parameter = 0;
                    ConsolePutROMString("\r\nHibás paraméter!");
                    ConsolePutROMString(ready);
                } else if (!flag.valve_run) {
                    valve_index = valve_i - 1;
                    valve[valve_index].req_pos = temp_value;
                    flag.valve_run = 1;
                ConsolePutROMString("\r\nRUN");
                }
                break;
            default: break;
        }
    } else;
}

/****************************************************************************/

void attach_RFnode(void) {
    sensoraddr = (RxPacket[0] & 0b00001111);
    TxPacket[0] = (0b01010000 | sensoraddr);
    TxPacket[1] = _time.sec; //sec
    TxPacket[2] = _time.min; //min
    TxPacket[3] = _time.hr; //hr
    TxPacket[4] = 1;
    TxPacket[5] = _time.day;
    TxPacket[6] = _time.mth; //mth
    TxPacket[7] = _time.yr; //yr
    Tx(8);

    ConsolePutROMString("/r/nRFjoin: ");
    btoa(sensoraddr, data);
    ConsolePutString(data);

    RxPacketLen = 0;
    hasPacket = FALSE;
}

/************************************************************************/

void receive_RFdata(void) {

    if (RxPacketLen == 6) {
        sensoraddr = (RxPacket[0] & 0b00001111);

        if (sensoraddr < 9) {
            Temperature[sensoraddr].Char[1] = RxPacket[1];
            Temperature[sensoraddr].Char[0] = RxPacket[2];
            Humidity[sensoraddr].Char[1] = RxPacket[3];
            Humidity[sensoraddr].Char[0] = RxPacket[4];
            ccc[sensoraddr] = RxPacket[5];
        }
    }
    RxPacketLen = 0;
    hasPacket = FALSE;
    ConsolePutROMString("\r\nRX: ");
    btoa(sensoraddr, data);
    ConsolePutString(data);
}

/*************************************************************************/

void ConsoleWriteTime(void) {

    flag.onemin = 0;
    btoa(_time.yr, data);
    ConsolePutROMString("\r\n 20");
    ConsolePutString(data);
    btoa(_time.mth, data);
    ConsolePutROMString(".");
    ConsolePutString(data);
    btoa(_time.day, data);
    ConsolePutROMString(".");
    ConsolePutString(data);
    btoa(_time.hr, data);
    ConsolePutROMString("  ");
    ConsolePutString(data);
    btoa(_time.min, data);
    ConsolePutROMString(":");
    ConsolePutString(data);
    btoa(_time.sec, data);
    ConsolePutROMString(":");
    ConsolePutString(data);
}

/***********************************************************************/

void CANDataTx(void) {

    unsigned char xx;
    flag.time_to_tx = 0;

    TX_Message.Address = 10;
    TX_Message.Ext = 0; //If the identifier is standard, clear the Ext flag
    TX_Message.Remote = 0; //clear the remote flag
    TX_Message.Priority = 0; //I
    TX_Message.NoOfBytes = 3; //RxPacketLen;	//Set number of bytes to send

    for (xx = 0; xx < 9; xx++) {
        DATA_LAT = 0; //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!4
        SCLK_LAT = 0; //!!!!!!!!!!!!!!!!!!!!!!!!!!!

        TempBoiler[xx].Int = I2CReadWord(SensorAddress[xx], TEMP);

        TX_Message.Data[0] = (0b01000000 | xx);
        TX_Message.Data[1] = TempBoiler[xx].Char[0];
        TX_Message.Data[2] = TempBoiler[xx].Char[1];
        CANPut(TX_Message); //addig próbál amíg üres helyet talál !!!!!!!!!!!!!!!!!!!!!!!!!
    }

    TX_Message.Address = 10;
    TX_Message.NoOfBytes = 6; //RxPacketLen;	//Set number of bytes to send

    Delay10us(100);
    for (xx = 0; xx < 9; xx++) //Fill the Data bytes
    {
        TX_Message.Data[0] = (0b10000000 | xx);
        TX_Message.Data[1] = Temperature[xx].Char[0];
        TX_Message.Data[2] = Temperature[xx].Char[1];
        TX_Message.Data[3] = Humidity[xx].Char[0];
        TX_Message.Data[4] = Humidity[xx].Char[1];
        TX_Message.Data[5] = ccc[xx];
        CANPut(TX_Message); //addig próbál amíg üres helyet talál !!!!!!!!!!!!!!!!!!!!!!!
        Delay10us(150);
    }
}

/*************************************************************************/

void CANErrorHandler(void) {
}