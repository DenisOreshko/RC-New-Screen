#include <InitCPU.h>
#include <Display.h>
#include <Buttons.h>
#include <DeviceParameters.h>
#include <Menu.h>
#include <Indication.h>
#include <Sound.h>
#include <UART0.h>
#include <FontDisplay.h>

void StartInit(){	
	initCPU();
	initDisplay();
}

void main(void){
	StartInit();
	clearDisplay();
	start_Sound();
	loadParameters();
	
	while(1){
		checkPressButton();
		updateMenu();	
		updateIndication();
		updateParameters();		
	}			
}