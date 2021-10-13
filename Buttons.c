#include <C8051F120.h>
#include <DeviceParameters.h>
#include <Menu.h>
#include <Delays.h>
#include <Sound.h>
#include <UART0.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
sbit A8 = P5^0;
sbit A9 = P5^1;
sbit A10 = P5^2;

sbit P_0 = P6^0;
sbit P_1 = P6^1;
sbit P_2 = P6^2;
sbit P_3 = P6^3;

unsigned char inut_P0_value = 0;
unsigned char inut_P1_value = 0;
unsigned char inut_P2_value = 0;
unsigned char inut_P3_value = 0;
unsigned char A10_value = 0;

xdata unsigned int timePressFunctionButton = 0;

xdata unsigned char ENERGY_str[4] = {0};
xdata unsigned char LAMP_FREQUENCY_str[4] = {0};
xdata unsigned char DELAY1_str[7] = {0};
xdata unsigned char DELAY2_str[7] = {0};

void buttonDisplay(){
	
	switch(INDEX_MENU){
		case 13:INDEX_MENU = 11;break;
		case 26:INDEX_MENU = 21;break;
		case 31:INDEX_MENU = 40;break;
		case 32:INDEX_MENU = 50;break;
		case 33:INDEX_MENU = 60;break;
		case 421:OPERATION_TIME = 0;break;
		case 422:FLASH_NUMBER = 0;break;
		case 45:INDEX_MENU = 40;break;
		case 54:INDEX_MENU = 50;break;
		case 62:INDEX_MENU = 60;break;
		default:INDEX_MENU++;break;
	}
	
}
void buttonLeft(){
	switch(INDEX_MENU){
		case 10:INDEX_MENU -= 0; break;
		case 11:ENERGY--;						
							
						if(ENERGY < EMIN)
							ENERGY = EMIN;
						/*
						ENERGY_str = malloc((int)(sizeof(char) * (ceil(log10(ENERGY))+1)));
						*/
						sprintf(ENERGY_str, "%d", ENERGY);
						
						SFRPAGE = 0x0F;
						UART0_sendData("ENERGY=",7);
		        UART0_sendData(ENERGY_str,3);
						UART0_TX_sendByte(0x0D);
						SFRPAGE = 0x0F;
						
						//free(ENERGY_str);
						memset(ENERGY_str,0,4);
						
						delay10mS();
						delay10mS();
						delay10mS();
						
						break;
		case 20:INDEX_MENU -= 10; break;
		case 21:LAMP_FREQUENCY--;
						
						if(LAMP_FREQUENCY < LAMP_FREQ_MIN){
						   LAMP_FREQUENCY = LAMP_FREQ_MIN;
						}
						/*
						LAMP_FREQUENCY_str = malloc((int)(sizeof(char) * (ceil(log10(LAMP_FREQUENCY))+1)));
						*/
						sprintf(LAMP_FREQUENCY_str, "%d", LAMP_FREQUENCY);
						
						SFRPAGE = 0x0F;
						UART0_sendData("LAMP_FREQUENCY=",15);
		        UART0_sendData(LAMP_FREQUENCY_str,3);
						UART0_TX_sendByte(0x0D);
						SFRPAGE = 0x0F;
						
						//free(LAMP_FREQUENCY_str);
						memset(LAMP_FREQUENCY_str,0,4);
						
						QSW_SHOTS = 1;
						delay10mS();
						delay10mS();
						break;
		case 22:index_prr--;
						if(index_prr < 0)
							index_prr = 0;
						QSW_F01 = output_prr_arr[index_prr];
						QSW_S01 = LAMP_FREQUENCY*10/QSW_F01;
						QSW_SHOTS = 1;
						delay10mS();
						break;
		case 23:if(QSW_SHOTS > 1){
							QSW_SHOTS--;
						}
						break;
		case 24:DELAY1-=1000;
						
						if(DELAY1 < DELAY1_MIN){
						   DELAY1 = DELAY1_MIN;
						}
						/*
						DELAY1_str = malloc((int)(sizeof(char) * (ceil(log10(DELAY1))+1)));
						*/
						sprintf(DELAY1_str, "%Lu", DELAY1);							
						SFRPAGE = 0x0F;
						UART0_sendData("DELAY1=",7);
		        UART0_sendData(DELAY1_str,7);
						UART0_TX_sendByte(0x0D);
						SFRPAGE = 0x0F;
						
						//free(DELAY1_str);
						memset(DELAY1_str,0,7);
						
						break;
		case 25:DELAY2-=1000;
						if(DELAY2 < DELAY2_MIN){
						   DELAY2 = DELAY2_MIN;
						}
						sprintf(DELAY2_str, "%Lu", DELAY2);							
						SFRPAGE = 0x0F;
						UART0_sendData("DELAY2=",7);
		        UART0_sendData(DELAY2_str,7);
						UART0_TX_sendByte(0x0D);
						SFRPAGE = 0x0F;
						memset(DELAY2_str,0,7);
						break;
		case 26:if(SIMMER_TIME > 0)
							SIMMER_TIME--;
						break;
		case 31:break;
		case 32:INDEX_MENU--;break;
		case 33:INDEX_MENU--;break;
		case 42:INDEX_MENU = 421;break;
		case 421:INDEX_MENU++;break;
		case 422:INDEX_MENU = 42;break;
		case 43:if(ENERGY_MAX > EMIN){
							ENERGY_MAX--;
							if(ENERGY > ENERGY_MAX){
								ENERGY = ENERGY_MAX;
							}
						}
						break;
		case 44:if(LAMP_FREQ_MAX > FMIN){
							LAMP_FREQ_MAX--;
							if(LAMP_FREQUENCY > LAMP_FREQ_MAX){
								LAMP_FREQUENCY = LAMP_FREQ_MAX;
								QSW_SHOTS = 1;
							}
						}
						break;
		case 45:if(ENERGY_RAD1_MAX > 0){
							ENERGY_RAD1_MAX--;
						}
						break;	
		case 51:MESSAGE_SOUND_MODE--;
						if(MESSAGE_SOUND_MODE < 1){
							MESSAGE_SOUND_MODE = 12;
						}
						delay10mS();
						break;
		case 52:BUTTON_SOUND_MODE--;
						if(BUTTON_SOUND_MODE < 1){
							BUTTON_SOUND_MODE = 12;
						}
						delay10mS();
						break;
		case 53:DISPLAY_MODE--;
						if(DISPLAY_MODE < 1){
							DISPLAY_MODE = 2;
						}
						delay10mS();
						delay10mS();
						delay10mS();
						break;
		case 54:ERROR_DISPLAY_MODE--;
						if(ERROR_DISPLAY_MODE < 1){
							ERROR_DISPLAY_MODE = 2;
						}
						delay10mS();
						delay10mS();
						delay10mS();
						break;
		case 60:break;
		case 61:if(U_PERCENT > 1)
						{
							U_PERCENT--;
						}
						break;
		case 62:if(PULS_LONGNESS > PULS_LONG_MIN)
						{
							PULS_LONGNESS--;
						}
						break;
		default:break;
	}
}
void buttonRight(){
	switch(INDEX_MENU){
		case 10:INDEX_MENU += 10;
						break;
		case 11:ENERGY++;					
		
						if(ENERGY > ENERGY_MAX){
						   ENERGY = ENERGY_MAX;							 
						}
						/*
						ENERGY_str = (char*) malloc((int)(sizeof(char) * (ceil(log10(ENERGY))+1)));
						*/
						sprintf(ENERGY_str, "%d", ENERGY);
						
						SFRPAGE = 0x0F;						
						UART0_sendData("ENERGY=",7);
		        UART0_sendData(ENERGY_str,3);
						UART0_TX_sendByte(0x0D);
						SFRPAGE = 0x0F;
						
						//free(ENERGY_str);
						memset(ENERGY_str,0,4);
						
						delay10mS();
						delay10mS();
						delay10mS();
						break;
		case 20:INDEX_MENU += 0;
						break;
		case 21:LAMP_FREQUENCY++;
						
						if(LAMP_FREQUENCY > LAMP_FREQ_MAX){
						   LAMP_FREQUENCY = LAMP_FREQ_MAX;
						}
						/*
						LAMP_FREQUENCY_str = malloc((int)(sizeof(char) * (ceil(log10(LAMP_FREQUENCY))+1)));
						*/
						sprintf(LAMP_FREQUENCY_str, "%d", LAMP_FREQUENCY);
						SFRPAGE = 0x0F;
						UART0_sendData("LAMP_FREQUENCY=",15);
		        UART0_sendData(LAMP_FREQUENCY_str,3);
						UART0_TX_sendByte(0x0D);
						SFRPAGE = 0x0F;
						//free(LAMP_FREQUENCY_str);
						memset(LAMP_FREQUENCY_str,0,4);
						
						QSW_SHOTS = 1;
						delay10mS();
						delay10mS();
						break;
		case 22:if(index_prr < max_index_prr)
							index_prr++;
						if(index_prr > max_index_prr)
							index_prr = max_index_prr;
						QSW_F01 = output_prr_arr[index_prr];
						QSW_S01 = LAMP_FREQUENCY*10/QSW_F01;
						QSW_SHOTS = 1;
						delay10mS();
						break;
		case 23:QSW_S01 = LAMP_FREQUENCY*10/QSW_F01;
						if(QSW_SHOTS < QSW_S01){
							QSW_SHOTS++;
						}
						break;
		case 24:DELAY1+=1000;
						
						if(DELAY1 > DELAY1_MAX){
						   DELAY1 = DELAY1_MAX;
						}
						/*
						DELAY1_str = malloc((int)(sizeof(char) * (ceil(log10(DELAY1))+1)));
						*/
						sprintf(DELAY1_str, "%Lu", DELAY1);
						
						SFRPAGE = 0x0F;
						UART0_sendData("DELAY1=",7);
		        UART0_sendData(DELAY1_str,7);
						UART0_TX_sendByte(0x0D);
						SFRPAGE = 0x0F;
						
						//free(DELAY1_str);
						memset(DELAY1_str,0,7);
						
						break;
		case 25:DELAY2+=1000;
						if(DELAY2 > DELAY2_MAX){
						   DELAY2 = DELAY2_MAX;
						}
						sprintf(DELAY2_str, "%Lu", DELAY2);
						
						SFRPAGE = 0x0F;
						UART0_sendData("DELAY2=",7);
		        UART0_sendData(DELAY2_str,7);
						UART0_TX_sendByte(0x0D);
						SFRPAGE = 0x0F;
						
						memset(DELAY2_str,0,7);
						
						break;
		case 26:if(SIMMER_TIME < 250)
							SIMMER_TIME++;						
						break;
		case 31:INDEX_MENU++;break;
		case 32:INDEX_MENU++;break;
		case 33:break;
		case 42:break;
		case 43:if(ENERGY_MAX < EMAX){
							ENERGY_MAX++;
						}
						break;
		case 44:if(LAMP_FREQ_MAX < FMAX){
							LAMP_FREQ_MAX++;
						}
						break;
		case 45:if(ENERGY_RAD1_MAX < RMAX1){
							ENERGY_RAD1_MAX++;
						}
						break;
		case 51:MESSAGE_SOUND_MODE++;
						if(MESSAGE_SOUND_MODE > 12){
							MESSAGE_SOUND_MODE = 1;
						}
						delay10mS();
						break;
		case 52:BUTTON_SOUND_MODE++;
						if(BUTTON_SOUND_MODE > 12){
							BUTTON_SOUND_MODE = 1;
						}
						delay10mS();
						break;
		case 53:DISPLAY_MODE++;
						if(DISPLAY_MODE > 2){
							DISPLAY_MODE = 1;
						}
						delay10mS();
						delay10mS();
						delay10mS();
						break;
		case 54:ERROR_DISPLAY_MODE++;
						if(ERROR_DISPLAY_MODE > 2)
						{							
							 ERROR_DISPLAY_MODE = 1;							
						}
						delay10mS();
						delay10mS();
						delay10mS();
						break;
		case 60:break;
		case 61:if(U_PERCENT < 30)
						{
							 U_PERCENT++;
						}
						break;
		case 62:if(PULS_LONGNESS < PULS_LONG_MAX)
						{
							 PULS_LONGNESS++;
						}
						break;						
		default:break;
	}
}
void buttonFunction(){  
	switch(INDEX_MENU/10){
		case 1:INDEX_MENU = 11;break;
		case 2:INDEX_MENU = 11;break;
		case 3:INDEX_MENU = 11;break;
		case 4:INDEX_MENU = 31;break;
		case 5:INDEX_MENU = 31;break;
		case 6:INDEX_MENU = 31;break;
		case 42:INDEX_MENU = 31;break;
		default:break;
	}
}
void buttonQsw2(){
	if(QSW2_STATE == 0x30)QSW2_STATE = 0x31;
	else QSW2_STATE = 0x30;
}
void buttonStop(){
	stop_Button_Sound();
	SFRPAGE = 0x0F;
	UART0_sendData("STOP",4);
	UART0_TX_sendByte(0x0D);
	SFRPAGE = 0x0F;	
}
void buttonQsw1(){	

	if(QSW1_STATE == 0x00){
		QSW1_STATE = 0x01;
		
		SFRPAGE = 0x0F;
		UART0_sendData("QSW1_STATE=1",11);
		UART0_TX_sendByte(0x0D);
		SFRPAGE = 0x0F;
	}
	else {
		QSW1_STATE = 0x00;
		
		SFRPAGE = 0x0F;
		UART0_sendData("QSW1_STATE=0",11);
		UART0_TX_sendByte(0x0D);
		SFRPAGE = 0x0F;
	}
}
void buttonLamp(){
	  switch(LAMP_STATE){
			case 0x00:
					SFRPAGE = 0x0F;
					UART0_sendData("START",5);
					UART0_TX_sendByte(0x0D);
					SFRPAGE = 0x0F;
					break;
			case 0x01:
					SFRPAGE = 0x0F;
					UART0_sendData("START",5);
					UART0_TX_sendByte(0x0D);
					SFRPAGE = 0x0F;	
					break;
			case 0x03:
				  SFRPAGE = 0x0F;
					UART0_sendData("LAMP_STATE=1",12);
					UART0_TX_sendByte(0x0D);
					SFRPAGE = 0x0F; 
					break;
			default:break;
		}
	
}
void buttonQswMode(){
  if(QSW_OPTION < 0x04)QSW_OPTION++;
	else QSW_OPTION = 0x00;

	SFRPAGE = 0x0F;
	UART0_sendData("QSW_OPTION=",11);
	UART0_TX_sendByte(QSW_OPTION + 0x30);
	delay100mkS();
	UART0_TX_sendByte(0x0D);
	SFRPAGE = 0x0F;
}
void buttonShutter1(){	
		
	if(SHUTTER1_STATE == 0x00){
		
		SHUTTER1_STATE = 0x01;
		
		SFRPAGE = 0x0F;
		UART0_sendData("SHUTTER1_STATE=1",15);
		UART0_TX_sendByte(0x0D);
		SFRPAGE = 0x0F;
	}else {
		
		SHUTTER1_STATE = 0x00;
		
		SFRPAGE = 0x0F;
		UART0_sendData("SHUTTER1_STATE=0",15);
		UART0_TX_sendByte(0x0D);
		SFRPAGE = 0x0F;
	}	
}
void buttonShutter2(){
	if(SHUTTER2_STATE == 0x30)SHUTTER2_STATE = 0x31;
	else SHUTTER2_STATE = 0x30;
}
void buttonLampMode(){	
	if(LAMP_OPTION < 0x03)
		LAMP_OPTION++;
	else 
		LAMP_OPTION = 0x01;
	
	SFRPAGE = 0x0F;
	UART0_sendData("LAMP_OPTION=",12);
	UART0_TX_sendByte(LAMP_OPTION + 0x30);
	delay100mkS();
	UART0_TX_sendByte(0x0D);
	SFRPAGE = 0x0F;
}

void longPressFunctionButton(){
		INDEX_MENU = 31;  	
}
void checkPressButton(){
	SFRPAGE = 0x0F;	
	A8 = 0;
	A9 = 1;
	A10 = 1;
	if(P_0 == 0){
		buttonDisplay();
		button_Sound();
		delay100mS();		
	}
	if(P_1 == 0){			
		buttonLeft();
		button_Sound();		    		
	}
	if(P_2 == 0){		
		buttonRight();
		button_Sound();	
	}
	if(P_3 == 0){
		buttonFunction();
		button_Sound();
		delay100mS();		
		timePressFunctionButton++;		
		if(timePressFunctionButton == 5){
			longPressFunctionButton();
			timePressFunctionButton = 0;			
		}
	}
	if(P_3 == 1){
		timePressFunctionButton = 0;
	}
	if(P_0 == 0 && P_3 == 0){
		delay100mS();	
		delay100mS();	
		INDEX_MENU = 21;
	}	
	A8 = 1;
	A9 = 0;
	A10 = 1;	
	if(P_0 == 0){
		buttonQsw2();
		button_Sound();
		delay100mS();		
	}	
	if(P_1 == 0){
		buttonStop();
		delay100mS();		
	}
	if(P_2 == 0){
		buttonQsw1();
		button_Sound();
		delay100mS();		
	}	
	if(P_3 == 0){
		buttonLamp();
		button_Sound();
		delay100mS();		
	}
	A8 = 1;
	A9 = 1;
	A10 = 0;
	inut_P0_value = P_0;
	if(P_0 == 0){
		buttonQswMode();
		button_Sound();
		delay100mS();		
	}
	inut_P1_value = P_1;
	if(P_1 == 0){		
		buttonShutter1();
		button_Sound();		
		delay100mS();			
	}
	inut_P2_value = P_2;
	if(P_2 == 0){
		buttonShutter2();
		button_Sound();
		delay100mS();			
	}
	inut_P3_value = P_3;
	if(P_3 == 0){
		buttonLampMode();
		button_Sound();
		delay100mS();			
	}
}