#include <C8051F120.h>
#include <Delays.h>

sbit SPI_SCK =  P0^2;// Serial Clock (output)              //SFRPAGE = All pages
sbit MISO = P0^3;                                          //SFRPAGE = All pages
sbit MOSI = P0^4;                                          //SFRPAGE = All pages
sbit SPI_NSS = P0^5;// Slave Select (output to chip select)//SFRPAGE = All pages
sbit DC = P1^0;                                            //SFRPAGE = All pages
sbit CS = P1^1;                                            //SFRPAGE = All pages
sbit RES = P1^2;                                           //SFRPAGE = All pages

xdata unsigned int k,m;

void oled_Command(char command_spi){
	SFRPAGE = 0x0F;
	DC = 0;       
	CS = 1;
	CS = 0;			
	SFRPAGE = SPI0_PAGE;
	SPI0DAT = command_spi;
	SPIF = 0;		
}

void oled_Data(char data_spi){
	SFRPAGE = 0x0F;
	DC = 1;
	CS = 1;	
	CS = 0;			
	SFRPAGE = SPI0_PAGE;
	SPI0DAT = data_spi;
	SPIF = 0;	
}

void Set_Command_Lock(unsigned char d)
{
	oled_Command(0xFD);			// Set Command Lock
	oled_Data(d);				// 0x12 = Unlock Basic Commands; 0x16 = lock
}
void Set_Display_Mode(unsigned char d)
{
	oled_Command(d);
}

void Set_Font_Clock_Divider(unsigned char d)
{
	oled_Command(0xB3);	// Set Display Clock Divider / Oscillator Frequency
	oled_Data(d);				// Default => 0xD0
											// A[3:0] => Display Clock Divider
											// A[7:4] => Oscillator Frequency
}
void Set_MUX_Ratio(unsigned char d)
{
	oled_Command(0xCA);			// Set Multiplex Ratio
	oled_Data(d);						//   Default => 0x7F (1/128 Duty)
}

void Set_Column_Address(unsigned char a, unsigned char b)
{
	oled_Command(0x15);	// Set Column Address
	oled_Data(a);				//   Default => 0x00
	oled_Data(b);				//   Default => 0x77
}

void Set_Row_Address(unsigned char a, unsigned char b)
{
	oled_Command(0x75);	// Set Row Address
	oled_Data(a);				//   Default => 0x00
	oled_Data(b);				//   Default => 0x7F
}
void Set_Display_Offset(unsigned char d)
{
	oled_Command(0xA2);			// Set Vertical Scroll by Row
	oled_Data(d);				//   Default => 0x00
}
void Set_Start_Line(unsigned char d)
{
	oled_Command(0xA1);			// Set Vertical Scroll by RAM
	oled_Data(d);				//   Default => 0x00
}
void Set_Remap_Format(unsigned char d)
{
	oled_Command(0xA0);	// Set Re-Map / Dual COM Line Mode
	oled_Data(d);				// Default => 0x40
	  				//     Horizontal Address Increment
						//     Column Address 0 Mapped to SEG0
						//     Disable Nibble Remap
						//     Scan from COM0 to COM[N-1]
						//     Disable COM Split Odd Even
	oled_Data(0x11); //   Default => 0x01 (Disable Dual COM Mode)
}
void Set_Contrast_Current(unsigned char d)
{
	oled_Command(0xC1);			// Set Contrast Current
	oled_Data(d);				//   Default => 0x7F
}
void Set_Master_Contrast_Current_Control(unsigned char d)
{
	oled_Command(0xC7);			// Master Contrast Current Control
	oled_Data(d);				//   Default => 0x0f (Maximum)
}
void Set_Linear_Gray_Scale_Table()
{
	oled_Command(0xB9);			// Set Default Linear Gray Scale Table
}

void Set_Phase_Length(unsigned char d)
{
	oled_Command(0xB1);			// Phase 1 (Reset) & Phase 2 (Pre-Charge) Period Adjustment
	oled_Data(d);				//   Default => 0x74 (7 Display Clocks [Phase 2] / 9 Display Clocks [Phase 1])
						//     D[3:0] => Phase 1 Period in 5~31 Display Clocks
						//     D[7:4] => Phase 2 Period in 3~15 Display Clocks
}

void Set_Display_Enhancement_B(unsigned char d)
{
	oled_Command(0xD1);			// Display Enhancement
	oled_Data(d);						// Default => 0xA2
													// 0x82 (0x00) => Reserved
													// 0xA2 (0x20) => Normal
	oled_Data(0x20);
}

void Set_Precharge_Voltage(unsigned char d)
{
	oled_Command(0xBB);			// Set Pre-Charge Voltage Level
	oled_Data(d);						//   Default => 0x17 (0.50*VCC)
}

void Set_Second_Precharge_Period(unsigned char d)
{
	oled_Command(0xB6);			// Set Second Pre-Charge Period
	oled_Data(d);				//   Default => 0x08 (8 Display Clocks)
}

void Set_VCOMH(unsigned char d)
{
	oled_Command(0xBE);			// Set COM Deselect Voltage Level
	oled_Data(d);				//   Default => 0x04 (0.80*VCC)
}

void Exit_Partial_Display()
{
	oled_Command(0xA9);
}

void Set_Sleep_Mode(unsigned char d)
{
	oled_Command(d);
}


void Set_GPIO(unsigned char d)
{
	oled_Command(0xB5);			// General Purpose IO
	oled_Data(d);				//   Default => 0x0A (GPIO Pins output Low Level.)
}
void Set_Function_Selection(unsigned char d)
{
	oled_Command(0xAB);			// Function Selection
	oled_Data(d);				//   Default => 0x01
						//     Enable Internal VDD Regulator
}
void Set_Display_Enhancement_A(unsigned char a, unsigned char b)
{
	oled_Command(0xB4);			// Display Enhancement
	oled_Data(0xA0|a);			//   Default => 0xA2
						//     0xA0 (0x00) => Enable External VSL
						//     0xA2 (0x02) => Enable Internal VSL (Kept VSL Pin N.C.)
	oled_Data(0x05|b);			//   Default => 0xB5
						//     0xB5 (0xB0) => Normal
						//     0xFD (0xF8) => Enhance Low Gray Scale Display Quality
}


void Set_Gray_Scale_Table()
{
	oled_Command(0xB8);			// Set Gray Scale Table
	oled_Data(0x0C);			//   Gray Scale Level 1
	oled_Data(0x18);			//   Gray Scale Level 2
	oled_Data(0x24);			//   Gray Scale Level 3
	oled_Data(0x30);			//   Gray Scale Level 4
	oled_Data(0x3C);			//   Gray Scale Level 5
	oled_Data(0x48);			//   Gray Scale Level 6
	oled_Data(0x54);			//   Gray Scale Level 7
	oled_Data(0x60);			//   Gray Scale Level 8
	oled_Data(0x6C);			//   Gray Scale Level 9
	oled_Data(0x78);			//   Gray Scale Level 10
	oled_Data(0x84);			//   Gray Scale Level 11
	oled_Data(0x90);			//   Gray Scale Level 12
	oled_Data(0x9C);			//   Gray Scale Level 13
	oled_Data(0xA8);			//   Gray Scale Level 14
	oled_Data(0xB4);			//   Gray Scale Level 15
	oled_Command(0x00);			// Enable Gray Scale Table
}

void writeRAMCommand()
{
	oled_Command(0x5C);
}

void clearDisplay(){
	Set_Column_Address(0x1C, 0x77);
	Set_Row_Address(0x00,0x7F);
	writeRAMCommand();
	for(k = 0; k < 119; k++){
		for(m = 0; m < 128; m++){
			oled_Data(0x00);
			oled_Data(0x00);
		}
	}
}

void fillDisplay(){
	Set_Column_Address(0x1C, 0x77);
	Set_Row_Address(0x00,0x7F);
	writeRAMCommand();
	for(k = 0; k < 119; k++){
		for(m = 0; m < 128; m++){
			oled_Data(0xFF);
			oled_Data(0x00);
		}
	}
}

void initDisplay(){	
	//SFRPAGE = 0x0F;
	RES = 0;
	delay100mS();
	RES = 1;
	delay100mS();	
	Set_Command_Lock(0x12);
	Set_Display_Mode(0xA4);			 	 // Set Display Mode = OFF
	Set_Font_Clock_Divider(0x91);	 // 0x91 = 80FPS; 0xD0 = default / 1100b 
	Set_MUX_Ratio(0x3F);					 // 0x3F = 63d = 64MUX (1/64 duty cycle)
	Set_Display_Offset(0x00);		 	 // 0x00 = (default)
	Set_Start_Line(0x00);				 	 // 0x00 = register 00h
	Set_Remap_Format(0x15);			 	 // 0x14 = Default except Enable Nibble Re-map, Scan from COM[N-1] to COM0, where N is the Multiplex ratio
	Set_GPIO(0x00);                // disable IO input
	Set_Function_Selection(0x01);  // function select
	Set_Contrast_Current(0x7F);	 	 // 0x7F = (default)
	Set_Master_Contrast_Current_Control(0x0F); // 0x0F = (default)
	Set_Linear_Gray_Scale_Table();
	Set_Phase_Length(0xE8);      				// 0xE2 = Phase 1 period (reset phase length) = 5 DCLKs,
	Set_Display_Enhancement_B(0xA2);		// 0xA2 = Normal (default); 0x82 = reserved
	Set_Precharge_Voltage(0x1F);				// 0x17 = default; 0x1F = 0.60*Vcc (spec example)
	Set_Second_Precharge_Period(0x08);	// 0x08 = 8 dclks (default)
	Set_VCOMH(0x07);										// 0x04 = 0.80*Vcc (default); 0x07 = 0.86*Vcc (spec example)
	Exit_Partial_Display();
	Set_Display_Mode(0xA6);							// Set Display Mode = Normal Display
	delay1mS();
	Set_Sleep_Mode(0xAF);								// Set Sleep mode OFF (Display ON)
	delay100mS();
	delay100mS();
}