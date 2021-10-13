#include <C8051F120.h>
//indication
void Timer2_Init(){
	 
	 SFRPAGE = TMR2_PAGE;
	 ET2 = 1;	
	 PT2 = 1;
	 TMR2CF = 0x00;
	 TR2 = 1;	//Enable and start Timer 2  
   RCAP2L = 0x00;
   RCAP2H = 0xFF;	
   	
}