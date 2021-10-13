#ifndef _UART0_h_
#define _UART0_h_
void UART0_Init();
void UART0_TX_sendByte(unsigned char byte);
void UART0_sendData(unsigned char *string, unsigned short lenght);
unsigned char UART0_acceptData();
extern xdata unsigned char buffer[128];
extern xdata unsigned int ib;
#endif //_UART0_h_

