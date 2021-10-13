#include <FontDisplay.h>
#include <DeviceParameters.h>
#include <BigFont.h>
#include <MiddleFont.h>
#include <Display.h>
#include <Delays.h>
#include <math.h>
#include <stdio.h>
#include <UART0.h>
int INDEX_MENU = 11;
int A = 0;
xdata unsigned char showOneTimeFREQ = 1;
xdata unsigned char showOneTime_RC_SOFT_VERS = 1;
xdata unsigned char needClearMainDisplay = 0;
xdata unsigned char needClearUserMenuDisplay = 0;
xdata unsigned char needClearSelectMenuDisplay = 0;
xdata unsigned char needClearWorkTimeShotsDisplay = 1;
xdata unsigned char needClearRC_Menu_Display = 1;
xdata unsigned int lenght_time_hour_str = 0;
xdata unsigned int lenght_time_min_str = 0;
xdata unsigned long int time_hour = 0;
xdata unsigned long int time_min = 0;
xdata char time_hour_str[10] = {0};
xdata char time_min_str[3] = {0};
xdata char flash_number_str[12] = {0};
xdata unsigned int lenght_flash_number_str = 0;
xdata unsigned char str_CAPACITOR_V[5]={0};
xdata unsigned short int lenght_CAPACITOR_V = 0;
xdata unsigned char str_SET_VOLTAGE[5]={0};
xdata unsigned short int lenght_SET_VOLTAGE = 0;
xdata unsigned char str_U_PERCENT[5] = {0};
xdata unsigned short int lenght_U_PERCENT = 0;
xdata unsigned char str_PULS_LONGNESS[5] = {0};
xdata unsigned short int lenght_PULS_LONGNESS = 0;

void write_5x7ASCII_String(const unsigned char* arr, int arr_size, unsigned char col, unsigned char row, unsigned char brightness){
	int i;
	for(i = 0; i < arr_size; i++){
		put_5x7ASCII_CharOnDisplay(arr[i], col, row, brightness);
		col+=2.0;
	}
}

void write_8x16ASCII_String(const unsigned char* arr, int arr_size, unsigned char col, unsigned char row, unsigned char brightness){
	int i;
	for(i = 0; i < arr_size; i++){
		put_8x16ASCII_CharOnDisplay(arr[i], col, row, brightness);
		col+=2.0;
	}
}
void write_WidthAdjusted_Arial_ASCII_String(const unsigned char* arr, int arr_size, unsigned char col, unsigned char row){
	int i;	
	for(i = 0; i < arr_size; i++){
		put_WidthAdjusted_Arial_ASCII_CharOnDisplay(arr[i], col, row);
		switch(arr[i]){
			case 'f': col+=2.0; break;
			case 'i': col+=2.0; break;
			case 'j': col+=2.0; break;
			case 'l': col+=2.0; break;
			case 'm': col+=4.0; break;
			case 'r': col+=2.0; break;
			case 't': col+=2.0; break;
			case 'w': col+=4.0; break;
			case 'W': col+=4.0; break;
			default : col+=3.0; break;
		}
	}
}
void swingToEndArr(unsigned char *arr, unsigned short int lenght_arr, unsigned short int lenght_number){
	int i = 0,j = 0;
	for(i = 0; i < lenght_arr-lenght_number; i++){	
		arr[4] = arr[3];
		arr[3] = arr[2];
		arr[2] = arr[1];
		arr[1] = arr[0];	
		
		arr[0] = ' ';
	}	
}


void resetTempParam(){
	switch(INDEX_MENU){
		case 10:temp_RADIATION1 = 0;
						temp_RADIATION2 = 0;
						break;
		case 11:temp_RADIATION1 = 0;
						temp_RADIATION2 = 0;
						showOneTimeFREQ = 1;
						break;
		case 12:temp_ENERGY = 0;
						temp_RADIATION2 = 0;
						break;
		case 13:temp_ENERGY = 0;
						temp_RADIATION1 = 0;
						break;
		case 20:temp_ENERGY = 0;
						temp_DELAY1 = 0;
						temp_DELAY2 = 0;
						break;
		case 21:temp_ENERGY = 0;
						temp_DELAY1 = 0;
						temp_DELAY2 = 0;
						showOneTimeFREQ = 1;
						break;
		case 22:temp_ENERGY = 0;
						showOneTimeFREQ = 1;
						temp_DELAY1 = 0;
						temp_DELAY2 = 0;
						break;
		case 23:temp_ENERGY = 0;
						showOneTimeFREQ = 1;
						temp_DELAY1 = 0;
						temp_DELAY2 = 0;
						temp_QSW_SHOTS = 0;
						break;
		case 24:temp_ENERGY = 0;
						showOneTimeFREQ = 1;
						temp_DELAY2 = 0;
						break;
		case 25:temp_ENERGY = 0;
						showOneTimeFREQ = 1;
						temp_DELAY1 = 0;
						break;
		case 26:temp_ENERGY = 0;
						showOneTimeFREQ = 1;
						temp_DELAY1 = 0;
						temp_DELAY2 = 0;
						break;
		case 30:temp_ENERGY = 0;break;
		case 31:temp_ENERGY = 0;break;
		case 32:temp_ENERGY = 0;break;
		case 33:temp_ENERGY = 0;break;
		case 34:temp_ENERGY = 0;break;
		case 42:temp_ENERGY_MAX = 0;break;
		case 43:temp_LAMP_FREQ_MAX = 0;break;
		case 44:temp_ENERGY_RAD1_MAX = 0;break;
		case 50:temp_MESSAGE_SOUND_MODE = 0;
						temp_BUTTON_SOUND_MODE = 0;
						temp_DISPLAY_MODE = 0;
						temp_ERROR_DISPLAY_MODE = 0;
						break;
		default:break;
	}
}

void updateMenu(){
	
	if(A != INDEX_MENU){
		A = INDEX_MENU;
		switch(A){
			case 10://write_5x7ASCII_String("<", 1, 0x3D, 0x01, 0x00);
							//write_5x7ASCII_String("MAIN MENU",9,0x40, 0x01, 0x0F);
							//write_5x7ASCII_String(" > ", 3, 0x52, 0x01, 0x00);
							write_8x16ASCII_String("ENERGY, J        ", 17, 0x3A, 0x0E, 0x06);
							write_8x16ASCII_String("RADIATION 1, mJ  ", 17, 0x3A, 0x1E, 0x06);
							write_8x16ASCII_String("RADIATION 2, mJ  ", 17, 0x3A, 0x2E, 0x06);
							setBrightnessBigNumber(0x0A);
							break;
			case 11:if(needClearMainDisplay){
									needClearMainDisplay = 0;
				          clearDisplay();			
							}
							needClearSelectMenuDisplay = 1;			
							//write_5x7ASCII_String("<", 1, 0x3D, 0x01, 0x00);
							
							//write_5x7ASCII_String(" > ", 3, 0x52, 0x01, 0x00);/**/
							write_8x16ASCII_String("ENERGY, J        ", 17, 0x3A, 0x00, 0x0F);
							write_8x16ASCII_String("RADIATION 1, mJ  ", 17, 0x3A, 0x0F, 0x06);
							write_8x16ASCII_String("RADIATION 2, mJ  ", 17, 0x3A, 0x1F, 0x06);
							//write_5x7ASCII_String(buffer,           9, 0x3A, 0x30, 0x0F);
							setBrightnessBigNumber(0x0F);			
							break;
			case 12:/*write_5x7ASCII_String("<", 1, 0x3D, 0x01, 0x00);
							write_5x7ASCII_String("MAIN MENU",9,0x40, 0x01, 0x06);
							write_5x7ASCII_String(" > ", 3, 0x52, 0x01, 0x00);*/
							write_8x16ASCII_String("ENERGY, J        ", 17, 0x3A, 0x00, 0x06);
							write_8x16ASCII_String("RADIATION 1, mJ  ", 17, 0x3A, 0x0F, 0x0F);
							write_8x16ASCII_String("RADIATION 2, mJ  ", 17, 0x3A, 0x1F, 0x06);
							setBrightnessBigNumber(0x0F);
							break;
			case 13:/*write_5x7ASCII_String("<", 1, 0x3D, 0x01, 0x00);
							write_5x7ASCII_String("MAIN MENU",9,0x40, 0x01, 0x06);
							write_5x7ASCII_String(" > ", 3, 0x52, 0x01, 0x00);*/
							write_8x16ASCII_String("ENERGY, J        ", 17, 0x3A, 0x00, 0x06);
							write_8x16ASCII_String("RADIATION 1, mJ  ", 17, 0x3A, 0x0F, 0x06);
							write_8x16ASCII_String("RADIATION 2, mJ  ", 17, 0x3A, 0x1F, 0x0F);
							setBrightnessBigNumber(0x0F);
							break;
			case 20:/*write_5x7ASCII_String("<", 1, 0x3D, 0x01, 0x0F);
							write_5x7ASCII_String("USER MENU",9,0x40, 0x01, 0x0F);
							write_5x7ASCII_String(" > ", 3, 0x52, 0x01, 0x00);		*/					
							write_8x16ASCII_String("LAMP FREQUENCY,Hz", 17, 0x3A, 0x00, 0x06);
							write_8x16ASCII_String("OUTPUT prr       ", 17, 0x3A, 0x0F, 0x06);
							write_8x16ASCII_String("BURST MODE SHOTS ", 17, 0x3A, 0x1F, 0x06);	
							setBrightnessBigNumber(0x0A);							
							break;
			case 21:if(needClearUserMenuDisplay){
									needClearUserMenuDisplay = 0;
									clearDisplay();	
							}
							/*write_5x7ASCII_String("<", 1, 0x3D, 0x01, 0x00);
							write_5x7ASCII_String("USER MENU",9,0x40, 0x01, 0x06);
							write_5x7ASCII_String(" > ", 3, 0x52, 0x01, 0x00);*/							
							write_8x16ASCII_String("LAMP FREQUENCY,Hz", 17, 0x3A, 0x00, 0x0F);//LAMP FREQUENCY, Hz
							write_8x16ASCII_String("OUTPUT prr       ", 17, 0x3A, 0x0F, 0x06);
							write_8x16ASCII_String("BURST MODE SHOTS ", 17, 0x3A, 0x1F, 0x06);
							setBrightnessBigNumber(0x0F);
							delay100mS();
							break;
			case 22:/*write_5x7ASCII_String("<", 1, 0x3D, 0x01, 0x00);
							write_5x7ASCII_String("USER MENU",9,0x40, 0x01, 0x06);
							write_5x7ASCII_String(" > ", 3, 0x52, 0x01, 0x00);*/							
							write_8x16ASCII_String("LAMP FREQUENCY,Hz", 17, 0x3A, 0x00, 0x06);
							write_8x16ASCII_String("OUTPUT prr       ", 17, 0x3A, 0x0F, 0x0F);//OUPUT, prr
							write_8x16ASCII_String("BURST MODE SHOTS ", 17, 0x3A, 0x1F, 0x06);
							setBrightnessBigNumber(0x0F);
							break;
			case 23:/*write_5x7ASCII_String("<", 1, 0x3D, 0x01, 0x00);
							write_5x7ASCII_String("USER MENU",9,0x40, 0x01, 0x06);
							write_5x7ASCII_String(" > ", 3, 0x52, 0x01, 0x00);*/							
							write_8x16ASCII_String("LAMP FREQUENCY,Hz", 17, 0x3A, 0x00, 0x06);
							write_8x16ASCII_String("OUTPUT prr       ", 17, 0x3A, 0x0F, 0x06);
							write_8x16ASCII_String("BURST MODE SHOTS ", 17, 0x3A, 0x1F, 0x0F);//BURST MODE SHOTS
							setBrightnessBigNumber(0x0F);
							break;
			case 24:/*write_5x7ASCII_String("<", 1, 0x3D, 0x01, 0x00);
							write_5x7ASCII_String("USER MENU",9,0x40, 0x01, 0x06);
							write_5x7ASCII_String(" > ", 3, 0x52, 0x01, 0x00);*/							
							write_8x16ASCII_String("Q-SW1 delay, uS  ", 17, 0x3A, 0x00, 0x0F);//DELAY1
							write_8x16ASCII_String("Q-SW2 delay, uS  ", 17, 0x3A, 0x0F, 0x06);
							write_8x16ASCII_String("SIMMER OFF, S    ", 17, 0x3A, 0x1F, 0x06);
							setBrightnessBigNumber(0x0F);
							break;
			case 25:/*write_5x7ASCII_String("<", 1, 0x3D, 0x01, 0x00);
							write_5x7ASCII_String("USER MENU",9,0x40, 0x01, 0x06);
							write_5x7ASCII_String(" > ", 3, 0x52, 0x01, 0x00);*/							
							write_8x16ASCII_String("Q-SW1 delay, uS  ", 17, 0x3A, 0x00, 0x06);
							write_8x16ASCII_String("Q-SW2 delay, uS  ", 17, 0x3A, 0x0F, 0x0F);//DELAY2
							write_8x16ASCII_String("SIMMER OFF, S    ", 17, 0x3A, 0x1F, 0x06);	
							setBrightnessBigNumber(0x0F);
							break;
			case 26:/*write_5x7ASCII_String("<", 1, 0x3D, 0x01, 0x00);
							write_5x7ASCII_String("USER MENU",9,0x40, 0x01, 0x06);
							write_5x7ASCII_String(" > ", 3, 0x52, 0x01, 0x00);*/							
							write_8x16ASCII_String("Q-SW1 delay, uS  ", 17, 0x3A, 0x00, 0x06);
							write_8x16ASCII_String("Q-SW2 delay, uS  ", 17, 0x3A, 0x0F, 0x06);
							write_8x16ASCII_String("SIMMER OFF, S    ", 17, 0x3A, 0x1F, 0x0F);//SIMMER OFF
							setBrightnessBigNumber(0x0F);
							break;
			case 30:needClearMainDisplay = 1;
							if(needClearSelectMenuDisplay){
								needClearSelectMenuDisplay = 0;
								clearBigFontSquare(0x1C, 0x00, 0x00);	
							}
							write_5x7ASCII_String("SELECT MENU BY ",    15,0x1C, 0x00, 0x06);//SELECT MENU3
							write_5x7ASCII_String("USING ",              6,0x1C, 0x0E, 0x06);
							write_5x7ASCII_String("\"<\" \">\"  ",       9,0x27, 0x0E, 0x0F);
							write_5x7ASCII_String("   AND PRESS   ",    15,0x1C, 0x1E, 0x06);
							write_5x7ASCII_String("\"DISPLAY\"",         9,0x1C, 0x2E, 0x0F);
							write_5x7ASCII_String("BUTTON",              6,0x2E, 0x2E, 0x06);
							write_8x16ASCII_String("Main Service Menu", 17, 0x3A, 0x00, 0x0F);//Main Service Menu
							write_8x16ASCII_String("RC Service Menu  ", 17, 0x3A, 0x0F, 0x06);
							write_8x16ASCII_String("PS Service Menu  ", 17, 0x3A, 0x1E, 0x06);
							write_8x16ASCII_String("CS Service Menu  ", 17, 0x3A, 0x2E, 0x06);	
							needClearUserMenuDisplay = 1;
							setBrightnessBigNumber(0x02);
							break;
			case 31:needClearMainDisplay = 1;
							if(needClearSelectMenuDisplay){
								needClearSelectMenuDisplay = 0;
								clearBigFontSquare(0x1C, 0x00, 0x00);	
							}							
							write_5x7ASCII_String("SELECT MENU BY ",    15,0x1C, 0x00, 0x06);//SELECT MENU3
							write_5x7ASCII_String("USING ",              6,0x1C, 0x0E, 0x06);
							write_5x7ASCII_String("\"<\" \">\"  ",       9,0x27, 0x0E, 0x0F);
							write_5x7ASCII_String("   AND PRESS   ",    15,0x1C, 0x1E, 0x06);
							write_5x7ASCII_String("\"DISPLAY\"",         9,0x1C, 0x2E, 0x0F);
							write_5x7ASCII_String("BUTTON",              6,0x2E, 0x2E, 0x06);
							write_8x16ASCII_String("Main Service Menu", 17, 0x3A, 0x00, 0x0F);//Main Service Menu
							write_8x16ASCII_String("RC Service Menu  ", 17, 0x3A, 0x0F, 0x06);
							write_8x16ASCII_String("PS Service Menu  ", 17, 0x3A, 0x1E, 0x06);
							write_8x16ASCII_String("CS Service Menu  ", 17, 0x3A, 0x2E, 0x06);	
							write_8x16ASCII_String("                   ", 17, 0x3A, 0x2E, 0x06);
							needClearUserMenuDisplay = 1;
							needClearRC_Menu_Display = 1;
							delay100mS();
							delay100mS();
							delay100mS();
							delay100mS();
							break;
			case 32:write_5x7ASCII_String("SELECT MENU BY ",    15,0x1C, 0x00, 0x06);//SELECT MENU3
							write_5x7ASCII_String("USING ",              6,0x1C, 0x0E, 0x06);
							write_5x7ASCII_String("\"<\" \">\"  ",       9,0x27, 0x0E, 0x0F);
							write_5x7ASCII_String("   AND PRESS   ",    15,0x1C, 0x1E, 0x06);
							write_5x7ASCII_String("\"DISPLAY\"",         9,0x1C, 0x2E, 0x0F);
							write_5x7ASCII_String("BUTTON",              6,0x2E, 0x2E, 0x06);		
							write_8x16ASCII_String("Main Service Menu", 17, 0x3A, 0x00, 0x06);
							write_8x16ASCII_String("RC Service Menu  ", 17, 0x3A, 0x0F, 0x0F);//RC Service Menu
							write_8x16ASCII_String("PS Service Menu  ", 17, 0x3A, 0x1E, 0x06);	
							write_8x16ASCII_String("CS Service Menu  ", 17, 0x3A, 0x2E, 0x06);	
							delay100mS();			
							break;
		  case 33:write_5x7ASCII_String("SELECT MENU BY ",    15,0x1C, 0x00, 0x06);//SELECT MENU3
							write_5x7ASCII_String("USING ",              6,0x1C, 0x0E, 0x06);//SELECT MENU3
							write_5x7ASCII_String("\"<\" \">\"  ",       9,0x27, 0x0E, 0x0F);//SELECT MENU3
							write_5x7ASCII_String("   AND PRESS   ",    15,0x1C, 0x1E, 0x06);//SELECT MENU3
							write_5x7ASCII_String("\"DISPLAY\"",         9,0x1C, 0x2E, 0x0F);//SELECT MENU3
							write_5x7ASCII_String("BUTTON",              6,0x2E, 0x2E, 0x06);//SELECT MENU3				
							write_8x16ASCII_String("Main Service Menu", 17, 0x3A, 0x00, 0x06);
							write_8x16ASCII_String("RC Service Menu  ", 17, 0x3A, 0x0F, 0x06);
							write_8x16ASCII_String("PS Service Menu  ", 17, 0x3A, 0x1E, 0x0F);//PS Service Menu
							write_8x16ASCII_String("CS Service Menu  ", 17, 0x3A, 0x2E, 0x06);									
							delay100mS();			
							break;
			case 40:clearDisplay();
							needClearSelectMenuDisplay = 1;
							write_8x16ASCII_String("LOTIS TII Minsk, Belarus        ", 32, 0x1C, 0x00, 0x0F);
							write_8x16ASCII_String("phone:+375(17) 321-21-55        ", 32, 0x1C, 0x0F, 0x0F);
							write_8x16ASCII_String("fax  :+375(17) 321-21-56        ", 32, 0x1C, 0x1E, 0x0F);
							write_8x16ASCII_String("www.lotis-tii.com               ", 32, 0x1C, 0x2E, 0x0F);
							break;
			case 41:clearDisplay();
							needClearWorkTimeShotsDisplay = 1;
							write_8x16ASCII_String("ID = 6176  LS2137/3             ", 32, 0x1C, 0x00, 0x0F);
							write_8x16ASCII_String("2.21 SOFTWARE VERSION OF PS     ", 32, 0x1C, 0x0F, 0x0F);
							break;
		  case 42:if(needClearWorkTimeShotsDisplay){
								needClearWorkTimeShotsDisplay = 0;
								clearDisplay();
							}
							temp_OPERATION_TIME = OPERATION_TIME;
							time_hour = OPERATION_TIME/60;
							if(time_hour > 0){
								lenght_time_hour_str = (int)(log10(time_hour) + 1);//count symbol in number of time hours	
								sprintf(time_hour_str,"%lu",time_hour);
							}else{						
								lenght_time_hour_str = 1;			
								time_hour_str[0] = '0';								
							}
							time_min = OPERATION_TIME%60;
							if(time_min > 0){
								lenght_time_min_str = (int)(log10(time_min) + 1);//count symbol in number of time min
								sprintf(time_min_str,"%lu",time_min);
							}else{
								lenght_time_min_str = 1;	
								time_min_str[0] = '0';
							}
							
							temp_FLASH_NUMBER = FLASH_NUMBER;
							if(FLASH_NUMBER > 0){
									lenght_flash_number_str = (int)(log10(FLASH_NUMBER) + 1);
									sprintf(flash_number_str,"%lu",FLASH_NUMBER);
							}else{
									flash_number_str[0] = '0';
									lenght_flash_number_str = 1;
							}			
							
							write_8x16ASCII_String("WORK TIME =                     ",32, 0x1C, 0x00, 0x0F);
							write_8x16ASCII_String(time_hour_str,  lenght_time_hour_str, 0x34, 0x00, 0x0F);
							write_8x16ASCII_String(" hour ",                          6, 0x34 + lenght_time_hour_str*2, 0x00, 0x0F);
							write_8x16ASCII_String(time_min_str,    lenght_time_min_str, 0x40 + lenght_time_hour_str*2, 0x00, 0x0F);
							write_8x16ASCII_String(" min",                            4, 0x40 + lenght_time_hour_str*2 + lenght_time_min_str*2, 0x00, 0x0F);
							write_8x16ASCII_String("Reset time                      ",32, 0x1C, 0x0F, 0x06);
							write_5x7ASCII_String("PRESS \"<\" TO SELECT",           20, 0x36, 0x15, 0x06);
							write_8x16ASCII_String("SHOTS =                         ",32, 0x1C, 0x1E, 0x0F);
							write_8x16ASCII_String(flash_number_str,lenght_flash_number_str, 0x2C, 0x1E, 0x0F);
							write_8x16ASCII_String("Reset shots                     ",32, 0x1C, 0x2E, 0x06);
							write_5x7ASCII_String("PRESS \"<\" TO SELECT",           20, 0x36, 0x33, 0x06);
							break;
			case 421:
							time_hour = OPERATION_TIME/60;
							if(time_hour > 0){
								lenght_time_hour_str = (int)(log10(time_hour) + 1);//count symbol in number of time hours	
								sprintf(time_hour_str,"%lu",time_hour);
							}else{						
								lenght_time_hour_str = 1;			
								time_hour_str[0] = '0';								
							}
							time_min = OPERATION_TIME%60;
							if(time_min > 0){
								lenght_time_min_str = (int)(log10(time_min) + 1);//count symbol in number of time min
								sprintf(time_min_str,"%lu",time_min);
							}else{
								lenght_time_min_str = 1;	
								time_min_str[0] = '0';
							}	
							
							if(FLASH_NUMBER > 0){
									lenght_flash_number_str = (int)(log10(FLASH_NUMBER) + 1);
									sprintf(flash_number_str,"%lu",FLASH_NUMBER);
							}else{
									flash_number_str[0] = '0';
									lenght_flash_number_str = 1;
							}	
							
							write_8x16ASCII_String("WORK TIME =                     ",32, 0x1C, 0x00, 0x06);
							write_8x16ASCII_String(time_hour_str,  lenght_time_hour_str, 0x34, 0x00, 0x0F);
							write_8x16ASCII_String(" hour ",                          6, 0x34 + lenght_time_hour_str*2, 0x00, 0x06);
							write_8x16ASCII_String(time_min_str,    lenght_time_min_str, 0x40 + lenght_time_hour_str*2, 0x00, 0x0F);
							write_8x16ASCII_String(" min",                            4, 0x40 + lenght_time_hour_str*2 + lenght_time_min_str*2, 0x00, 0x06);
							write_8x16ASCII_String("Reset time                     ",32, 0x1C, 0x0F, 0x0F);
							write_5x7ASCII_String("PRESS \"DISPLAY\"     ",          20, 0x36, 0x15, 0x0F);
							write_8x16ASCII_String("SHOTS =                        ",32, 0x1C, 0x1E, 0x06);
							write_8x16ASCII_String(flash_number_str                 ,lenght_flash_number_str, 0x2C, 0x1E, 0x06);
							write_8x16ASCII_String("Reset shots                    ",32, 0x1C, 0x2E, 0x06);
							write_5x7ASCII_String("PRESS \"<\" TO SELECT",           20, 0x36, 0x33, 0x06);
							delay100mS();
							needClearWorkTimeShotsDisplay = 0;
							break;
			case 422:
							time_hour = OPERATION_TIME/60;
							if(time_hour > 0){
								lenght_time_hour_str = (int)(log10(time_hour) + 1);//count symbol in number of time hours	
								sprintf(time_hour_str,"%lu",time_hour);
							}else{						
								lenght_time_hour_str = 1;			
								time_hour_str[0] = '0';								
							}
							time_min = OPERATION_TIME%60;
							if(time_min > 0){
								lenght_time_min_str = (int)(log10(time_min) + 1);//count symbol in number of time min
								sprintf(time_min_str,"%lu",time_min);
							}else{
								lenght_time_min_str = 1;	
								time_min_str[0] = '0';
							}	
							
							if(FLASH_NUMBER > 0){
									lenght_flash_number_str = (int)(log10(FLASH_NUMBER) + 1);
									sprintf(flash_number_str,"%lu",FLASH_NUMBER);
							}else{
									flash_number_str[0] = '0';
									lenght_flash_number_str = 1;
							}	
							
							write_8x16ASCII_String("WORK TIME =                     ",32, 0x1C, 0x00, 0x06);
							write_8x16ASCII_String(time_hour_str,  lenght_time_hour_str, 0x34, 0x00, 0x06);
							write_8x16ASCII_String(" hour ",                          6, 0x34 + lenght_time_hour_str*2, 0x00, 0x06);
							write_8x16ASCII_String(time_min_str,    lenght_time_min_str, 0x40 + lenght_time_hour_str*2, 0x00, 0x06);
							write_8x16ASCII_String(" min",                            4, 0x40 + lenght_time_hour_str*2 + lenght_time_min_str*2, 0x00, 0x06);
							write_8x16ASCII_String("Reset time                     ",32, 0x1C, 0x0F, 0x06);
							write_5x7ASCII_String("PRESS \"<\" TO SELECT ",          20, 0x36, 0x15, 0x06);
							write_8x16ASCII_String("SHOTS =                        ",32, 0x1C, 0x1E, 0x06);
							write_8x16ASCII_String(flash_number_str                 ,lenght_flash_number_str, 0x2C, 0x1E, 0x0F);
							write_8x16ASCII_String("Reset shots                    ",32, 0x1C, 0x2E, 0x0F);
							write_5x7ASCII_String("PRESS \"DISPLAY\"     ",          20, 0x36, 0x33, 0x0F);
							delay100mS();
							needClearWorkTimeShotsDisplay = 0;
							break;
			case 43:clearDisplay();
							write_8x16ASCII_String("J, Maximum of    ", 17, 0x3A, 0x00, 0x0F);
							write_8x16ASCII_String("PUMP ENERGY      ", 17, 0x3A, 0x0F, 0x0F);
							write_8x16ASCII_String("Hz, Maximum of   ", 17, 0x3A, 0x1F, 0x06);
							write_8x16ASCII_String("LAMP FREQUENCY   ", 17, 0x3A, 0x2F, 0x06);
							break;	
			case 44:write_8x16ASCII_String("J, Maximum of    ", 17, 0x3A, 0x00, 0x06);
							write_8x16ASCII_String("PUMP ENERGY      ", 17, 0x3A, 0x0F, 0x06);
							write_8x16ASCII_String("Hz, Maximum of   ", 17, 0x3A, 0x1F, 0x0F);
							write_8x16ASCII_String("LAMP FREQUENCY   ", 17, 0x3A, 0x2F, 0x0F);
							break;
			case 45:write_8x16ASCII_String("mJ, Maximum of   ", 17, 0x3A, 0x00, 0x0F);
							write_8x16ASCII_String("OUTPUT ENERGY 1  ", 17, 0x3A, 0x0F, 0x0F);
							write_8x16ASCII_String("                 ", 17, 0x3A, 0x1F, 0x06);
							write_8x16ASCII_String("                 ", 17, 0x3A, 0x2F, 0x06);
							break;
			case 50:
							needClearSelectMenuDisplay = 1;
							showOneTime_RC_SOFT_VERS = 1;
							if(needClearRC_Menu_Display){
								needClearRC_Menu_Display = 0;
								clearDisplay();
							}
							write_8x16ASCII_String("                 ", 17, 0x3A, 0x00, 0x06);
							write_8x16ASCII_String("SOFTWARE VERSION ", 17, 0x3A, 0x0F, 0x0F);//SOFTWARE
							write_8x16ASCII_String("OF REMOTE CONTROL", 17, 0x3A, 0x1F, 0x0F);//VERSION OF RC
							write_8x16ASCII_String("                 ", 17, 0x3A, 0x2F, 0x06);
							break;
			case 51:write_8x16ASCII_String("  MESSAGE SOUND  ", 17, 0x3A, 0x00, 0x0F);//MESSAGE SOUND MODE
							write_8x16ASCII_String("      MODE       ", 17, 0x3A, 0x0F, 0x0F);//MESSAGE SOUND MODE
							write_8x16ASCII_String("  BUTTONS SOUND  ", 17, 0x3A, 0x1F, 0x06);
							write_8x16ASCII_String("      MODE       ", 17, 0x3A, 0x2F, 0x06);
							break;
			case 52:write_8x16ASCII_String("  MESSAGE SOUND  ", 17, 0x3A, 0x00, 0x06);
							write_8x16ASCII_String("      MODE       ", 17, 0x3A, 0x0F, 0x06);
							write_8x16ASCII_String("  BUTTONS SOUND  ", 17, 0x3A, 0x1F, 0x0F);//BUTTONS SOUND MODE
							write_8x16ASCII_String("      MODE       ", 17, 0x3A, 0x2F, 0x0F);//BUTTONS SOUND MODE
							break;
			case 53:write_8x16ASCII_String("  DISPLAY MODE   ", 17, 0x3A, 0x00, 0x0F);//DISPLAY MODE
							write_8x16ASCII_String("    (GENERAL)    ", 17, 0x3A, 0x0F, 0x0F);//  (GENERAL)
							write_8x16ASCII_String("  ERROR DISPLAY  ", 17, 0x3A, 0x1F, 0x06);
							write_8x16ASCII_String("      MODE       ", 17, 0x3A, 0x2F, 0x06);
							break;
			case 54:write_8x16ASCII_String("  DISPLAY MODE   ", 17, 0x3A, 0x00, 0x06);
							write_8x16ASCII_String("    (GENERAL)    ", 17, 0x3A, 0x0F, 0x06);
							write_8x16ASCII_String("  ERROR DISPLAY  ", 17, 0x3A, 0x1F, 0x0F);//ERROR DISPLAY
							write_8x16ASCII_String("      MODE       ", 17, 0x3A, 0x2F, 0x0F);//    MODE
							break;
			case 60:clearDisplay();
							needClearSelectMenuDisplay = 1;						
							
							lenght_CAPACITOR_V = (int)(log10(CAPACITOR_V) + 1);							
							sprintf(str_CAPACITOR_V,"%d",CAPACITOR_V);							
							swingToEndArr(str_CAPACITOR_V, 5, lenght_CAPACITOR_V);
							
							write_20x32_Arial_String(str_CAPACITOR_V,    5, 0x30, 0x00, 0x0F);
							write_8x16ASCII_String("    V, ACTUAL    ", 17, 0x3A, 0x00, 0x0F);
							write_8x16ASCII_String("CAPACITOR VOLTAGE", 17, 0x3A, 0x0F, 0x0F);
							
							if(SET_VOLTAGE > 0){
								lenght_SET_VOLTAGE = (int)((log10(SET_VOLTAGE)) + 1);
							}else{
								lenght_SET_VOLTAGE = 1;
							}								
							sprintf(str_SET_VOLTAGE,"%d",SET_VOLTAGE);							
							swingToEndArr(str_SET_VOLTAGE, 5, lenght_SET_VOLTAGE);	
							
							write_20x32_Arial_String(str_SET_VOLTAGE,    5, 0x30, 0x1F, 0x0F);							
							write_8x16ASCII_String("                 ", 17, 0x3A, 0x1F, 0x06);
							write_8x16ASCII_String(" V, SET  VOLTAGE ", 17, 0x3A, 0x2F, 0x0F);
							break;
			case 61:
							lenght_U_PERCENT = (int)(log10(U_PERCENT) + 1);							
							sprintf(str_U_PERCENT,"%d",U_PERCENT);							
							swingToEndArr(str_U_PERCENT, 5, lenght_U_PERCENT);							
							write_20x32_Arial_String(str_U_PERCENT,      5, 0x30, 0x00, 0x0F);							
							write_8x16ASCII_String("%,MAXIMUM VOLTAGE", 17, 0x3A, 0x00, 0x0F);
							write_8x16ASCII_String("    DEVIATION    ", 17, 0x3A, 0x10, 0x0F);
			
							lenght_PULS_LONGNESS = (int)(log10(PULS_LONGNESS) + 1);							
							sprintf(str_PULS_LONGNESS,"%d",PULS_LONGNESS);							
							swingToEndArr(str_PULS_LONGNESS, 5, lenght_PULS_LONGNESS);							
							write_20x32_Arial_String(str_PULS_LONGNESS,  5, 0x30, 0x1F, 0x06);			
							write_8x16ASCII_String("                 ", 17, 0x3A, 0x1F, 0x06);
							write_8x16ASCII_String("uS,PULSE DURATION", 17, 0x3A, 0x2F, 0x06);
							break;
			case 62:lenght_U_PERCENT = (int)(log10(U_PERCENT) + 1);							
							sprintf(str_U_PERCENT,"%d",U_PERCENT);							
							swingToEndArr(str_U_PERCENT, 5, lenght_U_PERCENT);							
							write_20x32_Arial_String(str_U_PERCENT,      5, 0x30, 0x00, 0x0F);			
							write_8x16ASCII_String("%,MAXIMUM VOLTAGE", 17, 0x3A, 0x00, 0x06);
							write_8x16ASCII_String("    DEVIATION    ", 17, 0x3A, 0x10, 0x06);
							
							lenght_PULS_LONGNESS = (int)(log10(PULS_LONGNESS) + 1);							
							sprintf(str_PULS_LONGNESS,"%d",PULS_LONGNESS);							
							swingToEndArr(str_PULS_LONGNESS, 5, lenght_PULS_LONGNESS);							
							write_20x32_Arial_String(str_PULS_LONGNESS,  5, 0x30, 0x1F, 0x06);
							write_8x16ASCII_String("                 ", 17, 0x3A, 0x1F, 0x0F);
							write_8x16ASCII_String("uS,PULSE DURATION", 17, 0x3A, 0x2F, 0x0F);
							break;
			default:break;
		}
	}
	
	resetTempParam();
	switch(INDEX_MENU){
			case 10:if(temp_ENERGY != ENERGY){
								 temp_ENERGY = ENERGY;
								 showBigNumber(ENERGY);
							}
							break;
			case 11:if(temp_ENERGY != ENERGY){
								 temp_ENERGY = ENERGY;
								 showBigNumber(ENERGY);
							}
							break;
			case 12:if(temp_RADIATION1 != RADIATION1){
								temp_RADIATION1 = RADIATION1;
								showBigNumber(RADIATION1);
							}
							break;
			case 13:if(temp_RADIATION2 != RADIATION2){
								 temp_RADIATION2 = RADIATION2;
								 showBigNumber(RADIATION2);
							}	
							break;
			case 20:if(temp_LAMP_FREQUENCY != LAMP_FREQUENCY){
								temp_LAMP_FREQUENCY = LAMP_FREQUENCY;
								calcOutputPrrKoefficients();	
								QSW_F01 = output_prr_arr[max_index_prr];
								showBigNumber(LAMP_FREQUENCY);					
							}				
							break;
			case 21:if(temp_LAMP_FREQUENCY != LAMP_FREQUENCY){
								temp_LAMP_FREQUENCY = LAMP_FREQUENCY;
								calcOutputPrrKoefficients();	
								QSW_F01 = output_prr_arr[max_index_prr];
								showBigNumber(LAMP_FREQUENCY);					
							}else{
								if(showOneTimeFREQ){
									showBigNumber(LAMP_FREQUENCY);
									showOneTimeFREQ = 0;
								}
							}								
							break;
			case 22:showBigNumber(QSW_F01);							
							break;
			case 23:if(temp_QSW_SHOTS != QSW_SHOTS){
								temp_QSW_SHOTS = QSW_SHOTS;
								showBigNumber(QSW_SHOTS);
							}
							break;
			case 24:if(temp_DELAY1 != DELAY1){
								 temp_DELAY1 = DELAY1;
								 showBigNumber(DELAY1);	
							}	
							break;
			case 25:if(temp_DELAY2 != DELAY2){
								 temp_DELAY2 = DELAY2;
								 showBigNumber(DELAY2);	
							}			
							break;
			case 26:showBigNumber(SIMMER_TIME);				
							break;
			case 42:if(temp_OPERATION_TIME != OPERATION_TIME){
										temp_OPERATION_TIME = OPERATION_TIME;								
										time_hour = OPERATION_TIME/60;
										if(time_hour > 0){
											lenght_time_hour_str = (int)(log10(time_hour) + 1);//count symbol in number of time hours	
											sprintf(time_hour_str,"%lu",time_hour);
										}else{						
											lenght_time_hour_str = 1;			
											time_hour_str[0] = '0';								
										}
										time_min = OPERATION_TIME%60;
										if(time_min > 0){
											lenght_time_min_str = (int)(log10(time_min) + 1);//count symbol in number of time min
											sprintf(time_min_str,"%lu",time_min);
										}else{
											lenght_time_min_str = 1;	
											time_min_str[0] = '0';
										}																
										write_8x16ASCII_String("                     ",32, 0x32, 0x00, 0x0F);
										write_8x16ASCII_String(time_hour_str,  lenght_time_hour_str, 0x34, 0x00, 0x0F);
										write_8x16ASCII_String(" hour ",                          6, 0x34 + lenght_time_hour_str*2, 0x00, 0x0F);
										write_8x16ASCII_String(time_min_str,    lenght_time_min_str, 0x40 + lenght_time_hour_str*2, 0x00, 0x0F);
										write_8x16ASCII_String(" min ",                            5, 0x40 + lenght_time_hour_str*2 + lenght_time_min_str*2, 0x00, 0x0F);
									}
									if(temp_FLASH_NUMBER != FLASH_NUMBER){
										temp_FLASH_NUMBER = FLASH_NUMBER;
										if(FLASH_NUMBER > 0){
											lenght_flash_number_str = (int)(log10(FLASH_NUMBER) + 1);
											sprintf(flash_number_str,"%lu",FLASH_NUMBER);
										}else{
											flash_number_str[0] = '0';
											lenght_flash_number_str = 1;
										}
										write_8x16ASCII_String(flash_number_str,lenght_flash_number_str, 0x2C, 0x1E, 0x0F);
										write_8x16ASCII_String("         ",9, 0x2C + lenght_flash_number_str*2, 0x1E, 0x0F);
									}
							
							break;
				case 421:if(temp_OPERATION_TIME != OPERATION_TIME){
									temp_OPERATION_TIME = OPERATION_TIME;								
									time_hour = OPERATION_TIME/60;
									if(time_hour > 0){
										lenght_time_hour_str = (int)(log10(time_hour) + 1);//count symbol in number of time hours	
										sprintf(time_hour_str,"%lu",time_hour);
									}else{						
										lenght_time_hour_str = 1;			
										time_hour_str[0] = '0';								
									}
									time_min = OPERATION_TIME%60;
									if(time_min > 0){
										lenght_time_min_str = (int)(log10(time_min) + 1);//count symbol in number of time min
										sprintf(time_min_str,"%lu",time_min);
									}else{
										lenght_time_min_str = 1;	
										time_min_str[0] = '0';
									}									
									write_8x16ASCII_String("WORK TIME =                     ",32, 0x1C, 0x00, 0x06);
									write_8x16ASCII_String(time_hour_str,  lenght_time_hour_str, 0x34, 0x00, 0x0F);
									write_8x16ASCII_String(" hour ",                          6, 0x34 + lenght_time_hour_str*2, 0x00, 0x06);
									write_8x16ASCII_String(time_min_str,    lenght_time_min_str, 0x40 + lenght_time_hour_str*2, 0x00, 0x0F);
									write_8x16ASCII_String(" min",                            4, 0x40 + lenght_time_hour_str*2 + lenght_time_min_str*2, 0x00, 0x06);
									}
									if(temp_FLASH_NUMBER != FLASH_NUMBER){
										temp_FLASH_NUMBER = FLASH_NUMBER;
										if(FLASH_NUMBER > 0){
											lenght_flash_number_str = (int)(log10(FLASH_NUMBER) + 1);
											sprintf(flash_number_str,"%lu",FLASH_NUMBER);
										}else{
											flash_number_str[0] = '0';
											lenght_flash_number_str = 1;
										}
										write_8x16ASCII_String("SHOTS =                         ",32, 0x1C, 0x1E, 0x06);
										write_8x16ASCII_String(flash_number_str,lenght_flash_number_str, 0x2C, 0x1E, 0x06);
									}							
							break;
				case 422:if(temp_OPERATION_TIME != OPERATION_TIME){
									temp_OPERATION_TIME = OPERATION_TIME;								
									time_hour = OPERATION_TIME/60;
									if(time_hour > 0){
										lenght_time_hour_str = (int)(log10(time_hour) + 1);//count symbol in number of time hours	
										sprintf(time_hour_str,"%lu",time_hour);
									}else{						
										lenght_time_hour_str = 1;			
										time_hour_str[0] = '0';								
									}
									time_min = OPERATION_TIME%60;
									if(time_min > 0){
										lenght_time_min_str = (int)(log10(time_min) + 1);//count symbol in number of time min
										sprintf(time_min_str,"%lu",time_min);
									}else{
										lenght_time_min_str = 1;	
										time_min_str[0] = '0';
									}									
								  write_8x16ASCII_String("WORK TIME =                     ",32, 0x1C, 0x00, 0x06);
									write_8x16ASCII_String(time_hour_str,  lenght_time_hour_str, 0x34, 0x00, 0x06);
									write_8x16ASCII_String(" hour ",                          6, 0x34 + lenght_time_hour_str*2, 0x00, 0x06);
									write_8x16ASCII_String(time_min_str,    lenght_time_min_str, 0x40 + lenght_time_hour_str*2, 0x00, 0x06);
									write_8x16ASCII_String(" min",                            4, 0x40 + lenght_time_hour_str*2 + lenght_time_min_str*2, 0x00, 0x06);
									write_8x16ASCII_String("Shots =                        ",32, 0x1C, 0x1E, 0x06);
								}
								if(temp_FLASH_NUMBER != FLASH_NUMBER){
										temp_FLASH_NUMBER = FLASH_NUMBER;
										if(FLASH_NUMBER > 0){
											lenght_flash_number_str = (int)(log10(FLASH_NUMBER) + 1);
											sprintf(flash_number_str,"%lu",FLASH_NUMBER);
										}else{
											flash_number_str[0] = '0';
											lenght_flash_number_str = 1;
										}
										write_8x16ASCII_String("SHOTS =                         ",32, 0x1C, 0x1E, 0x06);
										write_8x16ASCII_String(flash_number_str,lenght_flash_number_str, 0x2C, 0x1E, 0x0F);
									}							
							break;
			case 43:
							if(temp_ENERGY_MAX != ENERGY_MAX){
								temp_ENERGY_MAX = ENERGY_MAX;
								showBigNumber(ENERGY_MAX);
							}
							break;
			case 44:
							if(temp_LAMP_FREQ_MAX != LAMP_FREQ_MAX){
								temp_LAMP_FREQ_MAX = LAMP_FREQ_MAX;
								showBigNumber(LAMP_FREQ_MAX);
							}
							break;
			case 45:
							if(temp_ENERGY_RAD1_MAX != ENERGY_RAD1_MAX){
								temp_ENERGY_RAD1_MAX = ENERGY_RAD1_MAX;
								showBigNumber(ENERGY_RAD1_MAX);
							}
							break;
			case 50:if(showOneTime_RC_SOFT_VERS){
								showOneTime_RC_SOFT_VERS = 0;
								showBigNumber(RC_SOFT_VERSION); 
							}
							break;
			case 51:if(temp_MESSAGE_SOUND_MODE != MESSAGE_SOUND_MODE){
								temp_MESSAGE_SOUND_MODE = MESSAGE_SOUND_MODE;
								showBigNumber(MESSAGE_SOUND_MODE); 
							}
							break;
			case 52:if(temp_BUTTON_SOUND_MODE != BUTTON_SOUND_MODE){
								temp_BUTTON_SOUND_MODE = BUTTON_SOUND_MODE;
								showBigNumber(BUTTON_SOUND_MODE); 
							}
							break;	
			case 53:if(temp_DISPLAY_MODE != DISPLAY_MODE){
								temp_DISPLAY_MODE = DISPLAY_MODE;
								showBigNumber(DISPLAY_MODE); 
							}
							break;
			case 54:if(temp_ERROR_DISPLAY_MODE != ERROR_DISPLAY_MODE){
								temp_ERROR_DISPLAY_MODE = ERROR_DISPLAY_MODE;
								showBigNumber(ERROR_DISPLAY_MODE); 
							}
							break;
			case 60:
							if(CAPACITOR_V > 0){
								lenght_CAPACITOR_V = (int)((log10(CAPACITOR_V)) + 1);
							}else{
								lenght_CAPACITOR_V = 1;
							}								
							sprintf(str_CAPACITOR_V,"%d",CAPACITOR_V);							
							swingToEndArr(str_CAPACITOR_V, 5, lenght_CAPACITOR_V);							
							write_20x32_Arial_String(str_CAPACITOR_V, 5, 0x30, 0x00, 0x0F);
							
							if(SET_VOLTAGE > 0){
								lenght_SET_VOLTAGE = (int)((log10(SET_VOLTAGE)) + 1);
							}else{
								lenght_SET_VOLTAGE = 1;
							}								
							sprintf(str_SET_VOLTAGE,"%d",SET_VOLTAGE);							
							swingToEndArr(str_SET_VOLTAGE, 5, lenght_SET_VOLTAGE);	
							write_20x32_Arial_String(str_SET_VOLTAGE, 5, 0x30, 0x1F, 0x0F);
							break;
			case 61:lenght_U_PERCENT = (int)(log10(U_PERCENT) + 1);							
							sprintf(str_U_PERCENT,"%d",U_PERCENT);							
							swingToEndArr(str_U_PERCENT, 5, lenght_U_PERCENT);							
							write_20x32_Arial_String(str_U_PERCENT,      5, 0x30, 0x00, 0x0F);
							
							lenght_PULS_LONGNESS = (int)(log10(PULS_LONGNESS) + 1);							
							sprintf(str_PULS_LONGNESS,"%d",PULS_LONGNESS);							
							swingToEndArr(str_PULS_LONGNESS, 5, lenght_PULS_LONGNESS);							
							write_20x32_Arial_String(str_PULS_LONGNESS,  5, 0x30, 0x1F, 0x06);
							break;
			case 62:lenght_U_PERCENT = (int)(log10(U_PERCENT) + 1);							
							sprintf(str_U_PERCENT,"%d",U_PERCENT);							
							swingToEndArr(str_U_PERCENT, 5, lenght_U_PERCENT);							
							write_20x32_Arial_String(str_U_PERCENT,      5, 0x30, 0x00, 0x06);			
							
							lenght_PULS_LONGNESS = (int)(log10(PULS_LONGNESS) + 1);							
							sprintf(str_PULS_LONGNESS,"%d",PULS_LONGNESS);							
							swingToEndArr(str_PULS_LONGNESS, 5, lenght_PULS_LONGNESS);							
							write_20x32_Arial_String(str_PULS_LONGNESS,  5, 0x30, 0x1F, 0x0F);

							break;
			default:break;
		}	
}



