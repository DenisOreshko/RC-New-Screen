#include <C8051F120.h>
#include <Delays.h>
#include <string.h>
sbit PS0 = IP^4;

xdata unsigned char tmp_byte = 0;
xdata unsigned char buffer[128] = {0};
xdata unsigned int ib = 0;

void UART0_Init(){
	SFRPAGE   = UART0_PAGE;
	SCON0 = 0x50;//UART Mode: 8-bit auto-reload
	SSTA0 = 0x0F;//
	ES0 = 1;
}

void UART0_TX_sendByte(unsigned char byte){
	SFRPAGE   = UART0_PAGE;	
	if(TI0 == 0)
	SBUF0 = byte;	
}

void UART0_sendData(unsigned char *string, unsigned short lenght){
	SFRPAGE   = UART0_PAGE;	
	TI0 = 0;
	while(*string != 0){
		UART0_TX_sendByte(*string++);
		delay100mkS();
	}
	memset(string,0,lenght);
}

unsigned char UART0_acceptData(){
	SFRPAGE   = UART0_PAGE;
	if(REN0 == 1){
		if(RI0 == 0);
			tmp_byte = SBUF0;
	}
	return tmp_byte;
}

void UART0_interrupt() interrupt 4{
	SFRPAGE   = UART0_PAGE;	
	
	TI0 = 0;
  if(RI0){
			*(buffer+ib) = UART0_acceptData();	
			ib++;	  
			RI0 = 0;
	}		
}