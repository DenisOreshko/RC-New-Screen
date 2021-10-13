#ifndef _Display_h_
#define _Display_h_
void initDisplay();
void clearDisplay();
void fillDisplay();
void oled_Command(char command_spi);
void oled_Data(char data_spi);
void Set_Column_Address(unsigned char a, unsigned char b);
void Set_Row_Address(unsigned char a, unsigned char b);
void writeRAMCommand();
#endif //_Display_h_