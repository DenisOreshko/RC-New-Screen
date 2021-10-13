#include <C8051F120.h>

sbit DIOD = P1^6;

void Timer4_Init(){
	 SFRPAGE   = TMR4_PAGE;   
   TMR4CF    = 0x08;
   RCAP4L    = 0xE5;
   RCAP4H    = 0xFF;
	 TMR4CN    = 0x04;//Timer 4 On
}
