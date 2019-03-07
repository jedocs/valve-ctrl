#ifndef __CAN_H
#define __CAN_H

//100Kbps prop 3 sjw 1 phase1 6 phase2 6

#define BAUD_RATE_PRESC 5
#define SJW_TIME 1
#define SAMPLES 1
#define PROP_TIME 3
#define PH_SEG_1 6
#define PH_SEG_2 6
#define CAN_CONFIG_1 BAUD_RATE_PRESC-1|(SJW_TIME-1<<6)
#if SAMPLES == 1
#define CAN_CONFIG_2 0x80|(PH_SEG_1-1<<3)|(PROP_TIME-1)
#elif SAMPLES == 3
#define CAN_CONFIG_2 0xC0|(PH_SEG_1-1<<3)|(PROP_TIME-1)
#else
#error "Number of samples is out of range"
#endif
#define CAN_CONFIG_3 PH_SEG_2-1
#define RXMASK0 0
#define RXMASK1 0
#define RXFILT0 0
#define RXFILT1 0
#define RXFILT2 0
#define RXFILT3 0
#define RXFILT4 0
#define RXFILT5 0
#define MY_IDENT 0x5af
#define RX_BUFFER 4
#define TX_BUFFER 4
//#define CAN_INT_LOW
//#define CAN_ERROR_HANDLER_ENABLE

#define MASK_ENABLE_ALL 0x00000000			//The mask to enable all incoming messages

#define ACCEPTANCE_MASK_0_1 RXMASK0			//Acceptance mask for filters 0 and 1
#define ACCEPTANCE_MASK_2_5 RXMASK1			//Acceptance mask for filters 2, 3, 4 and 5

//                          0x1FFFFFFF      Maximum extended identifier
//                          0x000007FF      Maximum standard identifier
#define ACCEPTANCE_FILTER_0 RXFILT0			//Acceptance filter 0
#define ACCEPTANCE_FILTER_1 RXFILT1			//Acceptance filter 1

//                          0x1FFFFFFF      Maximum extended identifier
//                          0x000007FF      Maximum standard identifier
#define ACCEPTANCE_FILTER_2 RXFILT2			//Acceptance filter 2
#define ACCEPTANCE_FILTER_3 RXFILT3			//Acceptance filter 3
#define ACCEPTANCE_FILTER_4 RXFILT4			//Acceptance filter 4
#define ACCEPTANCE_FILTER_5 RXFILT5			//Acceptance filter 5


//                            0x1FFFFFFF      Maximum extended identifier
//                            0x000007FF      Maximum standard identifier
#define MY_ADDRESS_IDENTIFIER MY_IDENT

//Size of RX buffer (14 bytes per buffer entry) has to be greater than or equal to 2
#define RXBUF RX_BUFFER

//Size of TX buffer (14 bytes per buffer entry) has to be greater than or equal to 2
#define TXBUF TX_BUFFER

#if TXBUF < 2 || RXBUF < 2
#error "The RXbuffer and TXbuffer has to greater than or equal to 2"
#endif

//CAN message structure (one message is 15 bytes wide)

struct CANMessage {
    unsigned long Address;
    unsigned char Data[8];
    unsigned char NoOfBytes;
    unsigned char Priority;
    unsigned Ext : 1;
    unsigned Remote : 1;
};

#define CAN_LISTEN_MODE 0x7F 
#define CAN_LOOPBACK_MODE 0x5F 
#define CAN_DISABLE_MODE 0x3F  
#define CAN_NORMAL_MODE 0x1F

char CANOpen(unsigned char, unsigned char, unsigned char);
void CANISR(void);
char CANPut(struct CANMessage);
char CANRXMessageIsPending(void);
struct CANMessage CANGet(void);
void CANSetMode(unsigned char);

#endif

