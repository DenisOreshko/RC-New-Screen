#ifndef _DeviceParameters_h_
#define _DeviceParameters_h_
//State Device Parameters
extern xdata unsigned char NAME;
extern xdata unsigned char CODE_ERROR;
extern xdata unsigned char COUNT_ERROR;
extern xdata unsigned char LAMP_OPTION;
extern xdata unsigned char QSW_OPTION;
extern xdata unsigned char LAMP_STATE;
extern xdata unsigned char QSW1_STATE;
extern xdata unsigned char QSW2_STATE;
extern xdata unsigned char CS_STATE;
extern xdata unsigned char SHUTTER1_STATE;
extern xdata unsigned char SHUTTER2_STATE;
extern xdata unsigned char PILOT;
extern xdata unsigned char STATE1;
extern xdata unsigned char STATE2;
extern xdata unsigned char STATE3;
extern xdata unsigned char STATE4;
extern xdata unsigned char VERS;
extern xdata unsigned char STT[16];
extern xdata unsigned char PS_SERIAL_NUMBER;
extern xdata unsigned char REQUIRED_PLD;
extern xdata unsigned char CURRENT_PLD;

//Model Parameters
extern xdata unsigned short int ID;
extern xdata unsigned char SETUP;
extern xdata unsigned char LASER;
extern xdata unsigned short int CAP;
extern xdata unsigned short int UCMAX;
extern xdata unsigned short int EMIN;
extern xdata unsigned short int EMAX;
extern xdata unsigned short int EDEF;
extern xdata unsigned short int FMIN;
extern xdata unsigned short int FMAX;
extern xdata unsigned short int FDEF;
extern xdata unsigned short int PMIN;
extern xdata unsigned short int PMAX;
extern xdata unsigned short int PDEF;
extern xdata unsigned short int RMAX1;
extern xdata unsigned short int RMAX2;
extern xdata unsigned short int PAUSE;

//Main Parameters
extern xdata unsigned int ENERGY;
extern xdata unsigned short int RADIATION1;
extern xdata unsigned short int RADIATION2;
extern xdata unsigned long int DELAY1;
extern xdata unsigned long int DELAY2;
extern xdata unsigned long int DELAY3;
extern xdata unsigned long int DELAY1_MIN;
extern xdata unsigned long int DELAY2_MIN;
extern xdata unsigned long int DELAY3_MIN;
extern xdata unsigned long int DELAY1_MAX;
extern xdata unsigned long int DELAY2_MAX;
extern xdata unsigned long int DELAY3_MAX;
extern xdata unsigned short int LAMP_FREQUENCY;
extern xdata unsigned short int LAMP_FREQ_MIN;
extern xdata unsigned short int LAMP_FREQ_MAX;
extern xdata unsigned short int LAMP_TRG_MIN;
extern xdata unsigned short int LAMP_TRG_MAX;
extern xdata unsigned char QSW_FREQUENCY;
extern xdata unsigned char QSW_SELECTING;
extern xdata unsigned short int QSW_F01;
extern xdata unsigned short int QSW_S01;
extern xdata unsigned short int QSW_SHOTS;
extern xdata unsigned char EXPONENT;
extern xdata unsigned char SIMMER_TIME;
extern xdata unsigned long int FLASH_NUMBER;
extern xdata unsigned long int OPERATION_TIME;
extern xdata unsigned char DAC2;
extern xdata unsigned char DAC2_MAX;

//PS_PAR
extern xdata unsigned short int SET_VOLTAGE;
extern xdata unsigned short int SET_VOLTAGE_MIN;
extern xdata unsigned short int SET_VOLTAGE_MAX;
extern xdata unsigned short int CAPACITANCE;
extern xdata unsigned short int CAPACITOR_V;
extern xdata unsigned short int LAMP_V;
extern xdata unsigned short int U_PERCENT;

//CS_MAIN_PAR
extern xdata unsigned char CS_FLOW;
extern xdata unsigned char CS_T_WATER;
extern xdata unsigned char CS_T_AIR;
extern xdata unsigned char CS_T_WORK;
extern xdata unsigned char CS_T_READY;
extern xdata unsigned char CS_T_MAX;
extern xdata unsigned char CS_FLOW_MIN;
extern xdata unsigned char CS_HEATER_CUR;
extern xdata unsigned char CS_HEATER_MIN;

//SERVICE_PAR
extern xdata unsigned short int DELAY_RAD1;
extern xdata unsigned short int DELAY_RAD1_MIN;
extern xdata unsigned short int DELAY_RAD1_MAX;
extern xdata unsigned short int intDELAY_RAD2;
extern xdata unsigned short int DELAY_RAD2_MIN;
extern xdata unsigned short int DELAY_RAD2_MAX;
extern xdata unsigned short int ENERGY_MAX;
extern xdata unsigned short int ENERGY_RAD1_MAX;
extern xdata unsigned short int ENERGY_RAD2_MAX;
extern xdata unsigned short int PULS_LONGNESS;
extern xdata unsigned short int PULS_LONG_MIN;
extern xdata unsigned short int PULS_LONG_MAX ;
extern xdata unsigned short int AVR_RAD;
extern xdata unsigned short int AVR_UFB;
extern xdata unsigned char DEBUG;
extern xdata unsigned char HOST_APPEARANCE;
extern xdata unsigned char HOST_USE;
extern xdata unsigned char U_DEMO;
extern xdata unsigned char U_DEMO2;
extern xdata unsigned short int FLASH_MAX;
extern xdata unsigned short int OPERATION_MAX;
extern xdata unsigned short int UFB_SCALE;
extern xdata unsigned short int RAD_SCALE;

//CS_SERVICE_PAR
extern xdata unsigned char CS_GAIN;
extern xdata unsigned char CS_TD;
extern xdata unsigned char CS_TI;
extern xdata unsigned char CS_HEATER;
extern xdata unsigned char CS_T_BIAS;
extern xdata unsigned char CS_TRDY_BIAS;
extern xdata unsigned char CS_OPTION;

//RC parameters
extern xdata unsigned short int RC_SOFT_VERSION;
extern xdata unsigned short int MESSAGE_SOUND_MODE;
extern xdata unsigned short int temp_MESSAGE_SOUND_MODE;
extern xdata unsigned short int BUTTON_SOUND_MODE;
extern xdata unsigned short int temp_BUTTON_SOUND_MODE;
extern xdata unsigned short int DISPLAY_MODE;
extern xdata unsigned short int temp_DISPLAY_MODE;
extern xdata unsigned short int ERROR_DISPLAY_MODE;
extern xdata unsigned short int temp_ERROR_DISPLAY_MODE;

//temp parameters
extern xdata unsigned int temp_ENERGY;
extern xdata unsigned short int temp_ENERGY_MAX;
extern xdata unsigned short int temp_ENERGY_RAD1_MAX;
extern xdata unsigned short int temp_ENERGY_RAD2_MAX;
extern xdata unsigned short int temp_RADIATION1;
extern xdata unsigned short int temp_RADIATION2;
extern xdata unsigned short int temp_LAMP_FREQUENCY;
extern xdata unsigned short int temp_LAMP_FREQ_MAX;
extern xdata unsigned long int temp_DELAY1;
extern xdata unsigned long int temp_DELAY2;
extern xdata unsigned long int temp_OPERATION_TIME;
extern xdata unsigned long int temp_FLASH_NUMBER;
extern xdata unsigned short int temp_QSW_SHOTS;
extern xdata unsigned char output_prr_arr[20];
extern xdata int index_prr;
extern xdata int max_index_prr;


void calcOutputPrrKoefficients();
void loadParameters();
void updateParameters();
#endif //_DeviceParameters_h_