#include <C8051F120.h>

//UART
void Timer1_Init(){
    SFRPAGE   = TIMER01_PAGE;
	  TMOD = 0x21; //Timer1 auto-reload 8 bit mode
	  TR1 = 1;
		TH1 = 0xF9;
}