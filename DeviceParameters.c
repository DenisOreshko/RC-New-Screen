#include <C8051F120.h>
#include <Menu.h>
#include <Delays.h>
#include <UART0.h>
#include <string.h>
#include <FontDisplay.h>
#include <stdlib.h>
#include <MiddleFont.h>

void updateLampFreqMax();
//State Device Parameters
xdata unsigned char NAME;
xdata unsigned char CODE_ERROR;
xdata unsigned char COUNT_ERROR;
xdata unsigned char LAMP_OPTION;
xdata unsigned char QSW_OPTION;
xdata unsigned char LAMP_STATE;
xdata unsigned char QSW1_STATE;
xdata unsigned char QSW2_STATE;
xdata unsigned char CS_STATE;
xdata unsigned char SHUTTER1_STATE;
xdata unsigned char SHUTTER2_STATE;
xdata unsigned char PILOT;
xdata unsigned char STATE1;
xdata unsigned char STATE2;
xdata unsigned char STATE3;
xdata unsigned char STATE4;
xdata unsigned char VERS;
xdata unsigned char STT[16] = {0};
xdata unsigned char PS_SERIAL_NUMBER;
xdata unsigned char REQUIRED_PLD;
xdata unsigned char CURRENT_PLD;

//Model Parameters
xdata unsigned short int ID;
xdata unsigned char SETUP;
xdata unsigned char LASER;
xdata unsigned short int CAP;
xdata unsigned short int UCMAX;
xdata unsigned short int EMIN;
xdata unsigned short int EMAX;
xdata unsigned short int EDEF;
xdata unsigned short int FMIN;
xdata unsigned short int FMAX;
xdata unsigned short int FDEF;
xdata unsigned short int PMIN;
xdata unsigned short int PMAX;
xdata unsigned short int PDEF;
xdata unsigned short int RMAX1;
xdata unsigned short int RMAX2;
xdata unsigned short int PAUSE;

//Main Parameters
xdata unsigned int ENERGY;
xdata unsigned short int RADIATION1;
xdata unsigned short int RADIATION2;
xdata unsigned long int DELAY1;
xdata unsigned long int DELAY2;
xdata unsigned long int DELAY3;
xdata unsigned long int DELAY1_MIN;
xdata unsigned long int DELAY2_MIN;
xdata unsigned long int DELAY3_MIN;
xdata unsigned long int DELAY1_MAX;
xdata unsigned long int DELAY2_MAX;
xdata unsigned long int DELAY3_MAX;
xdata unsigned short int LAMP_FREQUENCY;
xdata unsigned short int LAMP_FREQ_MIN;
xdata unsigned short int LAMP_FREQ_MAX;
xdata unsigned short int LAMP_TRG_MIN;
xdata unsigned short int LAMP_TRG_MAX;
xdata unsigned short int QSW_FREQUENCY;
xdata unsigned short int QSW_SELECTING;
xdata unsigned short int QSW_F01;
xdata unsigned short int QSW_S01;
xdata unsigned short int QSW_SHOTS;
xdata unsigned char EXPONENT;
xdata unsigned char SIMMER_TIME;
xdata unsigned long int FLASH_NUMBER;
xdata unsigned long int OPERATION_TIME;
xdata unsigned char DAC2;
xdata unsigned char DAC2_MAX;

//PS_PAR
xdata unsigned short int SET_VOLTAGE;
xdata unsigned short int SET_VOLTAGE_MIN;
xdata unsigned short int SET_VOLTAGE_MAX;
xdata unsigned short int CAPACITANCE;
xdata unsigned short int CAPACITOR_V;
xdata unsigned short int LAMP_V;
xdata unsigned short int U_PERCENT;

//CS_MAIN_PAR
xdata unsigned char CS_FLOW;
xdata unsigned char CS_T_WATER;
xdata unsigned char CS_T_AIR;
xdata unsigned char CS_T_WORK;
xdata unsigned char CS_T_READY;
xdata unsigned char CS_T_MAX;
xdata unsigned char CS_FLOW_MIN;
xdata unsigned char CS_HEATER_CUR;
xdata unsigned char CS_HEATER_MIN;

//SERVICE_PAR
xdata unsigned short int DELAY_RAD1;
xdata unsigned short int DELAY_RAD1_MIN;
xdata unsigned short int DELAY_RAD1_MAX;
xdata unsigned short int DELAY_RAD2;
xdata unsigned short int DELAY_RAD2_MIN;
xdata unsigned short int DELAY_RAD2_MAX;
xdata unsigned short int ENERGY_MAX;
xdata unsigned short int ENERGY_RAD1_MAX;
xdata unsigned short int ENERGY_RAD2_MAX;
xdata unsigned short int PULS_LONGNESS;
xdata unsigned short int PULS_LONG_MIN;
xdata unsigned short int PULS_LONG_MAX;
xdata unsigned short int AVR_RAD;
xdata unsigned short int AVR_UFB;
xdata unsigned char DEBUG;
xdata unsigned char HOST_APPEARANCE;
xdata unsigned char HOST_USE;
xdata unsigned char U_DEMO;
xdata unsigned char U_DEMO2;
xdata unsigned short int FLASH_MAX;
xdata unsigned short int OPERATION_MAX;
xdata unsigned short int UFB_SCALE;
xdata unsigned short int RAD_SCALE;

//CS_SERVICE_PAR
xdata unsigned char CS_GAIN;
xdata unsigned char CS_TD;
xdata unsigned char CS_TI;
xdata unsigned char CS_HEATER;
xdata unsigned char CS_T_BIAS;
xdata unsigned char CS_TRDY_BIAS;
xdata unsigned char CS_OPTION;

//RC parameters
xdata unsigned short int RC_SOFT_VERSION = 100;
xdata unsigned short int MESSAGE_SOUND_MODE = 3;
xdata unsigned short int temp_MESSAGE_SOUND_MODE = 0;
xdata unsigned short int BUTTON_SOUND_MODE = 3;
xdata unsigned short int temp_BUTTON_SOUND_MODE = 0;
xdata unsigned short int DISPLAY_MODE = 2;
xdata unsigned short int temp_DISPLAY_MODE = 0;
xdata unsigned short int ERROR_DISPLAY_MODE = 2;
xdata unsigned short int temp_ERROR_DISPLAY_MODE = 0;

//temp parameters
xdata unsigned int temp_ENERGY = 0;
xdata unsigned short int temp_ENERGY_MAX = 0;
xdata unsigned short int temp_ENERGY_RAD1_MAX = 0;
xdata unsigned short int temp_ENERGY_RAD2_MAX = 0;
xdata unsigned short int temp_RADIATION1 = 0;
xdata unsigned short int temp_RADIATION2 = 0;
xdata unsigned short int temp_LAMP_FREQUENCY = 0;
xdata unsigned short int temp_LAMP_FREQ_MAX = 0;
xdata unsigned long int temp_DELAY1 = 0;
xdata unsigned long int temp_DELAY2 = 0;
xdata unsigned long int temp_OPERATION_TIME = 0;
xdata unsigned long int temp_FLASH_NUMBER = 0;
xdata unsigned short int temp_QSW_SHOTS = 0;
xdata unsigned char output_prr_arr[20];
xdata int index_prr;
xdata int max_index_prr;
xdata unsigned char tmp_string_ENERGY[4] = {0};
xdata unsigned char tmp_string_LAMP_FREQUENCY[4] = {0};
xdata unsigned char tmp_string_LAMP_FREQ_MAX[4] = {0};
xdata unsigned short int str = 0, lng = 0;

void calcOutputPrrKoefficients(){
	int i;	
	unsigned char tmpLF = LAMP_FREQUENCY * 10;
	index_prr = 0;
	output_prr_arr[20] = 0;
	for(i = 0; i < 20; i++)
		output_prr_arr[i] = 0;
	for(i = tmpLF; i > 0;i--){
		if(tmpLF%i == 0){
				if(tmpLF/i > 10){
					if((tmpLF/i)%10 == 0){
						output_prr_arr[index_prr] = tmpLF/i;
						max_index_prr = index_prr;	
						index_prr++;
					}
				}else{
					output_prr_arr[index_prr] = tmpLF/i;
					max_index_prr = index_prr;	
					index_prr++;
				}	
		}
	}		
}

void loadParameters(){
	CODE_ERROR = 0x1E;
	ENERGY = 60;
	RADIATION1 = 24;
	RADIATION2 = 435;
	SIMMER_TIME = 60;	
	DELAY1 = 150000;
	DELAY2 = 150000;
	DELAY1_MIN = 40000;
	DELAY2_MIN = 40000;
	DELAY1_MAX = 240000;
	DELAY2_MAX = 240000;
	EMIN = 50;
	EMAX = 700;
	ENERGY_MAX = 600;
	ENERGY_RAD1_MAX = 900;
	RMAX1 = 900;
	ENERGY_RAD2_MAX = 900;
	LAMP_STATE = 0x30;
	SHUTTER1_STATE = 0x30;
	QSW1_STATE = 0x30;
	LAMP_OPTION = 0x01;
	QSW_OPTION = 0x01;
	LAMP_FREQUENCY = 10;
	LAMP_FREQ_MIN = 1;
	LAMP_FREQ_MAX = 10;
	FMIN = 1;
	FMAX = 20;
	QSW_F01 = LAMP_FREQUENCY*10;
	QSW_S01 = 1;
	QSW_SHOTS = 1;		
	OPERATION_TIME = 110125;//maximum number 4294967295
	FLASH_NUMBER = 20545;  //maximum number 4294967295
	CAPACITOR_V = 1015;
	SET_VOLTAGE = 258;
	U_PERCENT = 5;
	PULS_LONGNESS = 200;
	PULS_LONG_MIN = 150;
	PULS_LONG_MAX = 16000;	
	updateLampFreqMax();
}

void updateEnergy(){	
	ib = 0;
	lng = 0;
	str = 0;
	memset(buffer,0,128);
	memset(tmp_string_ENERGY,0,4);
	SFRPAGE = 0x0F;
	UART0_sendData("energy ?",8);
	UART0_TX_sendByte(0x0D);
	SFRPAGE = 0x0F;
	delay35mS();
	
	while(*(buffer+str) != '='){
		str++;
	}
	
	while(*(buffer+str+lng) != ';'){
		lng++;
	}
	
	memcpy(tmp_string_ENERGY, &buffer[str+1], sizeof(char) * lng-1);
	//memcpy(tmp_string_ENERGY, &buffer[17], sizeof(char) * lng-1);
	//write_8x16ASCII_String(tmp_string_ENERGY, 20, 0x3A, 0x30, 0x0F);
	if(buffer[16] == '='){
		memcpy(tmp_string_ENERGY, &buffer[str+1], sizeof(char) * lng-1);
		ENERGY = atoi(tmp_string_ENERGY);	
	}
}
void updateRadiation1(){	

}
void updateRadiation2(){	

}
void updateQsw_f01(){

}
void updateDelay1(){	

}
void updateDelay2(){	

}
void updateSimmerOff(){	

}
void updateLampFrequency(){	
	ib = 0;
	lng = 0;
	str = 0;
	memset(buffer,0,128);
	memset(tmp_string_LAMP_FREQUENCY,0,4);
	SFRPAGE = 0x0F;
	UART0_sendData("LAMP_FREQUENCY ?",16);
	UART0_TX_sendByte(0x0D);
	SFRPAGE = 0x0F;
	delay1mS();
	
	while(*(buffer+str) != '='){
		str++;
	}
	while(*(buffer+str+lng) != ';'){
		lng++;
	}
	memcpy(tmp_string_LAMP_FREQUENCY, &buffer[str+1], sizeof(char) * lng-1);
	LAMP_FREQUENCY = atoi(tmp_string_LAMP_FREQUENCY);
}
void updateOperation_Time(){
	
}
void updateFlash_Number(){
	
}
void updateLampEnergyMax(){	

}

void updateLampFreqMax(){	
	ib = 0;
	lng = 0;
	str = 0;
	memset(buffer,0,128);
	memset(tmp_string_LAMP_FREQ_MAX,0,4);
	SFRPAGE = 0x0F;
	UART0_sendData("LAMP_FREQ_MAX ?",16);
	UART0_TX_sendByte(0x0D);
	SFRPAGE = 0x0F;
	delay1mS();
	
	while(*(buffer+str) != '='){
		str++;
	}
	while(*(buffer+str+lng) != ';'){
		lng++;
	}
	memcpy(tmp_string_LAMP_FREQ_MAX, &buffer[str+1], sizeof(char) * lng-1);
	LAMP_FREQ_MAX = atoi(tmp_string_LAMP_FREQ_MAX);

}
void updateEnergyRadiation1Max(){	

}
void updateQSW_SHOTS(){	

}
void update_CAPACITOR_V(){	

}
void update_SET_VOLTAGE(){	

}
void update_U_PERCENT(){	

}
void update_PULS_LONGNESS(){	

}

void updateSTT(){
	ib = 0;
	memset(buffer,0,128);
	SFRPAGE = 0x0F;
	UART0_sendData("STT ?",5);
	UART0_TX_sendByte(0x0D);
	SFRPAGE = 0x0F;
	delay35mS();
	memcpy(STT, &buffer[12], sizeof(char) * 16);
	//write_8x16ASCII_String(STT,           16, 0x3A, 0x30, 0x0F);	
	LAMP_STATE = (STT[15])&0x03;
	LAMP_OPTION =(STT[14])&0x03;
	QSW1_STATE = (STT[13])&0x01;
	QSW2_STATE = (STT[13]>>1)&0x01;
	SHUTTER1_STATE = (STT[13]>>2)&0x01;
	SHUTTER2_STATE = (STT[13]>>3)&0x01;
	QSW_OPTION = (STT[12])&0x07;
	
	CODE_ERROR = STT[11];
	COUNT_ERROR = STT[9];
	
	put_8x16ASCII_CharOnDisplay(LAMP_STATE, 0x3A, 0x30, 0x0F);
	put_8x16ASCII_CharOnDisplay(LAMP_OPTION, 0x3C, 0x30, 0x0F);
	put_8x16ASCII_CharOnDisplay(QSW1_STATE, 0x3E, 0x30, 0x0F);
	put_8x16ASCII_CharOnDisplay(QSW2_STATE, 0x40, 0x30, 0x0F);
	put_8x16ASCII_CharOnDisplay(SHUTTER1_STATE, 0x42, 0x30, 0x0F);
	put_8x16ASCII_CharOnDisplay(SHUTTER2_STATE, 0x44, 0x30, 0x0F);
	put_8x16ASCII_CharOnDisplay(QSW_OPTION, 0x46, 0x30, 0x0F);
	put_8x16ASCII_CharOnDisplay(CODE_ERROR, 0x4A, 0x30, 0x0F);
	put_8x16ASCII_CharOnDisplay(COUNT_ERROR, 0x4E, 0x30, 0x0F);
	//write_8x16ASCII_String(LAMP_STATE,           1, 0x3A, 0x30, 0x0F);/*
	//write_8x16ASCII_String(LAMP_OPTION,          1, 0x3C, 0x30, 0x0F);
	//write_8x16ASCII_String(QSW_OPTION,           1, 0x3E, 0x30, 0x0F);*/	
}

void updateParameters(){
	
	updateSTT();
	switch(INDEX_MENU){
		case 10:updateEnergy();break;
		case 11:updateEnergy();break;
		case 12:updateRadiation1();break;
		case 13:updateRadiation2();break;
		case 20:updateLampFrequency();break;
		case 21:updateLampFrequency();break;
		case 22:updateQsw_f01();break;
		case 23:updateQSW_SHOTS();break;
		case 24:updateDelay1();break;
		case 25:updateDelay2();break;
		case 26:updateSimmerOff();break;
		case 42:updateOperation_Time();
						updateFlash_Number();
						break;
		case 421:updateOperation_Time();
						updateFlash_Number();
						break;
		case 422:updateOperation_Time();
						updateFlash_Number();
						break;
		case 43:updateLampEnergyMax();break;
		case 44:updateLampFreqMax();break;
		case 45:updateEnergyRadiation1Max();break;
		case 60:update_CAPACITOR_V();
						update_SET_VOLTAGE();
						break;
		case 61:update_U_PERCENT();
						update_PULS_LONGNESS();
						break;
		default:break;
	}	
}