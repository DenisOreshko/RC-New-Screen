#include <C8051F120.h>

sbit T0   = P3^4;
sbit T1   = P3^5;

xdata unsigned short byte_T0 = 0xFD7F;//400Hz

void Timer0_Init(){
	
	SFRPAGE = TIMER01_PAGE;
	ET0 = 1;
	PT0 = 0;
  TMOD      = 0x21; //Timer0  16 bit mode; Timer1 auto-reload 8 bit mode
  CKCON     = 0x02;
	T0 = 1;
	T1 = 1;	
}

void Timer0_On(){
	SFRPAGE = TIMER01_PAGE;
	TR0 = 1;

}
void Timer0_Off(){
	SFRPAGE = TIMER01_PAGE;
	TR0 = 0;
	T0 = 1;
	T1 = 1;
}

//SOUND GENERATOR
void Timer0_Interrupt(void) interrupt 1{
	SFRPAGE = TIMER01_PAGE;		
	TL0       = byte_T0 & 0x00FF;
	TH0       = byte_T0 >> 8;		
		
	if(byte_T0 == 0){
		T0 = 1;
		T1 = 1;
	}else{
		T0 = ~T0;
	  T1 = ~T1;
	}
	TF0 = 0;
}