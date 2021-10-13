#include <C8051F120.h>
#include <Timer0.h>
#include <Sound.h>

xdata unsigned short times_sound = 0;
xdata unsigned short note_delay = 0;

void Interrupts_Timer3_Init(){
	EIE2 = 0x01;	
  EIP2 = 0x00;
}

void Timer3_Init(){
	 SFRPAGE   = TMR3_PAGE;
	 TMR3CF    = 0x18;
	 RCAP3H    = 0xCF; 
	 RCAP3L    = 0xCC; //1mS
	 Interrupts_Timer3_Init();		
}

void Timer3_On(){
		SFRPAGE   = TMR3_PAGE;
		TR3 = 1;	
}
void Timer3_Off(){
		SFRPAGE   = TMR3_PAGE;
		TR3 = 0;	
}
//SOUND DELAY
void Timer3_Interrupt(void) interrupt 14{
	SFRPAGE   = TMR3_PAGE;
	TF3 = 0;
	
	times_sound++;	
	if(times_sound > note_delay)
	{
		sper++;
		times_sound = 0;
		Timer3_Off();
		Timer0_Off();		
	}	
}