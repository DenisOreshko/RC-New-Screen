#ifndef _FontDisplay_h_
#define _FontDisplay_h_
unsigned test_bit(unsigned aValue, unsigned aNumber);
void init_M_5x7_ASCII(unsigned char ch);
void init_Alphabet_5x7_ASCII();
void init_Alphabet_8x16_ASCII();
void init_Alphabet_WidthAdjusted_Arial_ASCII();
void draw4Pixels(unsigned char fData,unsigned char sData,unsigned char row, unsigned char col);
void put_5x7ASCII_CharOnDisplay(unsigned char ch, unsigned char col, unsigned char row, unsigned char brightness);
void put_7x8ASCII_CharOnDisplay(unsigned char ch, unsigned char col, unsigned char row);
void put_8x16ASCII_CharOnDisplay(unsigned char ch, unsigned char col, unsigned char row, unsigned char brightness);
void put_WidthAdjusted_Arial_ASCII_CharOnDisplay(unsigned char ch, unsigned char col, unsigned char row);
void put_8x16Latin_CharOnDisplay(unsigned char ch, unsigned char col, unsigned char row);
void put_WidthAdjusted_Latin_CharOnDisplay(unsigned char ch, unsigned char col, unsigned char row);
void put_GB2312_CharOnDisplay(unsigned int ch, unsigned char col, unsigned char row);
void put_5x10LCM_1_CharOnDisplay(unsigned int ch, unsigned char col, unsigned char row);
void put_5x10LCM_2_CharOnDisplay(unsigned int ch, unsigned char col, unsigned char row);
void put_5x10LCM_3_CharOnDisplay(unsigned int ch, unsigned char col, unsigned char row);
void put_5x10LCM_8_CharOnDisplay(unsigned int ch, unsigned char col, unsigned char row);
void put_5x10LCM_11_CharOnDisplay(unsigned int ch, unsigned char col, unsigned char row);
void put_5x10LCM_12_CharOnDisplay(unsigned int ch, unsigned char col, unsigned char row);
void put_5x10LCM_13_CharOnDisplay(unsigned int ch, unsigned char col, unsigned char row);
#endif //_FontDisplay_h_