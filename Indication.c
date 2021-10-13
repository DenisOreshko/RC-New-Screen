#include <C8051F120.h>
#include <DeviceParameters.h>
#include <SFRPage.h>
#include <Delays.h>
sbit A14 = P5^6;
sbit A15 = P5^7;
sbit DOIDGREEN = P1^6;

xdata unsigned char P4_REG3 = 0xFF;
xdata unsigned char P4_REG4 = 0xFF;
xdata unsigned char trigger = 0;

void REG3_On(){
	SFRPAGE = 0x0F;
	A14 = 0;
	
}
void REG3_Off(){
	SFRPAGE = 0x0F;
	A14 = 1;
}

void REG4_On(){
	SFRPAGE = 0x0F;
	A15 = 0;
}
void REG4_Off(){
	SFRPAGE = 0x0F;
	A15 = 1;
}

void Timer2_Interrupt(void) interrupt 5{
	SFRPAGE = TMR2_PAGE;
	TF2 = 0;
	
	if(trigger){
		REG4_Off();
		REG3_On();
		P4 = P4_REG3;
		

	}else{
		REG3_Off();
		REG4_On();
		P4 = P4_REG4;
	}
	trigger = ~trigger;
	DOIDGREEN = ~DOIDGREEN;	
}
void updateIndication(){
	SFRPAGE = 0x0F;
	
	switch(LAMP_STATE){
			case 0x00: P4_REG3|=1<<0;   P4_REG4|=1<<0;   break;//setLampDiod_Off();
			case 0x01: P4_REG3&=~(1<<0);P4_REG4|=1<<0;   break;//setLampDiod_Red();
			case 0x03: P4_REG3|=1<<0;   P4_REG4&=~(1<<0);break;//setLampDiod_Green();
			default: break;
	}
	switch(SHUTTER1_STATE){
			case 0x00: P4_REG3|=(1<<1); P4_REG4|=1<<1;break;//setShutter1Diod_Off();
			case 0x01: P4_REG3&=~(1<<1);P4_REG4|=1<<1;break;//setShutter1Diod_On();									
			default: break;
	}
	switch(QSW1_STATE){
			case 0x00: P4_REG3|=(1<<2); P4_REG4|=1<<2;break;//setQSW1Diod_Off();
			case 0x01: P4_REG3&=~(1<<2);P4_REG4|=1<<2;break;//setQSW1Diod_On();
			default: break;
	}
	switch(LAMP_OPTION){
			case 0x01: P4_REG3&=~(1<<3);P4_REG4|=1<<3;//setLampIntDiod_On();
								 P4_REG3|=(1<<4); P4_REG4|=1<<4;//setLampExtDiod_Off();
								 P4_REG3|=(1<<5); P4_REG4|=1<<5;//setLampSingleDiod_Off();
								 break;
			case 0x02: P4_REG3|=(1<<3); P4_REG4|=1<<3;//setLampIntDiod_Off();
								 P4_REG3&=~(1<<4);P4_REG4|=1<<4;//setLampExtDiod_On();
								 P4_REG3|=(1<<5); P4_REG4|=1<<5;//setLampSingleDiod_Off();
								 break;
			case 0x03: P4_REG3|=(1<<3); P4_REG4|=1<<3;//setLampIntDiod_Off();
								 P4_REG3|=(1<<4); P4_REG4|=1<<4;//setLampExtDiod_Off();
								 P4_REG3&=~(1<<5);P4_REG4|=1<<5;//setLampSingleDiod_On();
								 break;
			default: break;
	}
	switch(QSW_OPTION){
			case 0x00: P4_REG3|=1<<6;
															 P4_REG4&=~(1<<1);//setQswFrDiod_On();
								               P4_REG4|=1<<2;
															 P4_REG4|=1<<3;
															 P4_REG4|=1<<4;
								 break;								 
			case 0x01: P4_REG3&=~(1<<6);              //setQswIntDiod_On();								  
								               P4_REG4|=1<<1;
															 P4_REG4|=1<<2;
		                           P4_REG4|=1<<3;
															 P4_REG4|=1<<4;
								 break;
			case 0x02: P4_REG3|=1<<6; 
															 P4_REG4|=1<<1;
							                 P4_REG4&=~(1<<2);//setQswExt1Diod_On();
															 P4_REG4|=1<<3;
															 P4_REG4|=1<<4;
									break;
			case 0x03: P4_REG3|=1<<6; 
															 P4_REG4|=1<<1;
								               P4_REG4|=1<<2;
						                   P4_REG4&=~(1<<3);//setQswExt2Diod_On();
															 P4_REG4|=1<<4;
									break;
			case 0x04: P4_REG3|=1<<6; 
															 P4_REG4|=1<<1;
								               P4_REG4|=1<<2;
															 P4_REG4|=1<<3;
						                   P4_REG4&=~(1<<4);//setQswSingleDiod_On();
								 break;
			default: break;
  }
}