#include <C8051F120.h>
#include <Display.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <Delays.h>

sbit MF_SCLK = P3^0;
sbit MF_SI   = P3^1;
sbit MF_CS2  = P3^2;
sbit MF_SO   = P3^3;

xdata unsigned char read_command = 0x0B;
xdata unsigned long int  Address = 0;
xdata unsigned char address_BYTE1_MSB = 0x00;
xdata unsigned char address_BYTE2 = 0x00;
xdata unsigned char address_BYTE3_LSB = 0x00;
xdata unsigned char dummyBYTE = 0xFF;
xdata unsigned char font5x7[8], font7x8[8], font8x16[16], font16x16[34], fontGB2312[32];
xdata unsigned int f = 0, t = 0, dr = 0;
xdata int r = 0;
xdata unsigned char MSB = 0;
xdata unsigned char LSB = 0;
xdata unsigned char draw_byte = 0x00;
xdata unsigned char ReadByte = 0x00;

code unsigned char space_5x7_ASCII[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
code unsigned char A_5x7_ASCII[8] = {0x7E,0x11,0x11,0x11,0x7E,0x00,0x00,0x00};
code unsigned char B_5x7_ASCII[8] = {0x7F,0x49,0x49,0x49,0x36,0x00,0x00,0x00};
code unsigned char C_5x7_ASCII[8] = {0x3E,0x41,0x41,0x41,0x22,0x00,0x00,0x00};
code unsigned char D_5x7_ASCII[8] = {0x7F,0x41,0x41,0x22,0x1C,0x00,0x00,0x00};
code unsigned char E_5x7_ASCII[8] = {0x7F,0x49,0x49,0x49,0x41,0x00,0x00,0x00};
code unsigned char F_5x7_ASCII[8] = {0x7F,0x09,0x09,0x09,0x01,0x00,0x00,0x00};
code unsigned char G_5x7_ASCII[8] = {0x3E,0x41,0x49,0x49,0x7A,0x00,0x00,0x00};
code unsigned char H_5x7_ASCII[8] = {0x7F,0x08,0x08,0x08,0x7F,0x00,0x00,0x00};
code unsigned char I_5x7_ASCII[8] = {0x00,0x41,0x7F,0x41,0x00,0x00,0x00,0x00};
code unsigned char J_5x7_ASCII[8] = {0x20,0x40,0x41,0x3F,0x01,0x00,0x00,0x00};
code unsigned char K_5x7_ASCII[8] = {0x7F,0x08,0x14,0x22,0x41,0x00,0x00,0x00};
code unsigned char L_5x7_ASCII[8] = {0x7F,0x40,0x40,0x40,0x40,0x00,0x00,0x00};
code unsigned char M_5x7_ASCII[8] = {0x7F,0x02,0x0C,0x02,0x7F,0x00,0x00,0x00};
code unsigned char N_5x7_ASCII[8] = {0x7F,0x04,0x08,0x10,0x7F,0x00,0x00,0x00};
code unsigned char O_5x7_ASCII[8] = {0x3E,0x41,0x41,0x41,0x3E,0x00,0x00,0x00};
code unsigned char P_5x7_ASCII[8] = {0x7F,0x09,0x09,0x09,0x06,0x00,0x00,0x00};
code unsigned char Q_5x7_ASCII[8] = {0x3E,0x41,0x51,0x21,0x5E,0x00,0x00,0x00};
code unsigned char R_5x7_ASCII[8] = {0x7F,0x09,0x19,0x29,0x46,0x00,0x00,0x00};
code unsigned char S_5x7_ASCII[8] = {0x46,0x49,0x49,0x49,0x31,0x00,0x00,0x00};
code unsigned char T_5x7_ASCII[8] = {0x01,0x01,0x7F,0x01,0x01,0x00,0x00,0x00};
code unsigned char U_5x7_ASCII[8] = {0x3F,0x40,0x40,0x40,0x3F,0x00,0x00,0x00};
code unsigned char V_5x7_ASCII[8] = {0x1F,0x20,0x40,0x20,0x1F,0x00,0x00,0x00};
code unsigned char W_5x7_ASCII[8] = {0x3F,0x40,0x38,0x40,0x3F,0x00,0x00,0x00};
code unsigned char X_5x7_ASCII[8] = {0x63,0x14,0x08,0x14,0x63,0x00,0x00,0x00};
code unsigned char Y_5x7_ASCII[8] = {0x07,0x08,0x70,0x08,0x07,0x00,0x00,0x00};
code unsigned char Z_5x7_ASCII[8] = {0x61,0x51,0x49,0x45,0x43,0x00,0x00,0x00};
code unsigned char left_5x7_ASCII[8] = {0x08,0x14,0x22,0x41,0x00,0x00,0x00,0x00};
code unsigned char right_5x7_ASCII[8] = {0x00,0x41,0x22,0x14,0x08,0x00,0x00,0x00};
code unsigned char quote_5x7_ASCII[8] = {0x00,0x07,0x00,0x07,0x00,0x00,0x00,0x00};
code unsigned char zpt_5x7_ASCII[8] = {0x00,0x50,0x30,0x00,0x00,0x00,0x00,0x00};

code unsigned char space_8x16_ASCII[16] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
code unsigned char A_8x16_ASCII[16] = {0x00,0x00,0xE0,0x10,0xE0,0x00,0x00,0x00,0x78,0x07,0x04,0x04,0x04,0x07,0x78,0x00};
code unsigned char B_8x16_ASCII[16] = {0x00,0xF0,0x10,0x10,0x10,0x10,0xE0,0x00,0x00,0x7F,0x42,0x42,0x42,0x45,0x38,0x00};
code unsigned char C_8x16_ASCII[16] = {0xC0,0x20,0x10,0x10,0x10,0x20,0x40,0x00,0x1F,0x20,0x40,0x40,0x40,0x20,0x10,0x00};
code unsigned char D_8x16_ASCII[16] = {0x00,0xF0,0x10,0x10,0x10,0x20,0xC0,0x00,0x00,0x7F,0x40,0x40,0x40,0x20,0x1F,0x00};
code unsigned char E_8x16_ASCII[16] = {0x00,0xF0,0x10,0x10,0x10,0x10,0x10,0x00,0x00,0x7F,0x42,0x42,0x42,0x42,0x40,0x00};
code unsigned char F_8x16_ASCII[16] = {0x00,0xF0,0x10,0x10,0x10,0x10,0x10,0x00,0x00,0x7F,0x01,0x01,0x01,0x01,0x00,0x00};
code unsigned char G_8x16_ASCII[16] = {0xC0,0x20,0x10,0x10,0x10,0x20,0x40,0x00,0x1F,0x20,0x40,0x40,0x42,0x22,0x7E,0x00};
code unsigned char H_8x16_ASCII[16] = {0x00,0xF0,0x00,0x00,0x00,0x00,0xF0,0x00,0x00,0x7F,0x02,0x02,0x02,0x02,0x7F,0x00};
code unsigned char I_8x16_ASCII[16] = {0x00,0x10,0x10,0xF0,0x10,0x10,0x00,0x00,0x00,0x40,0x40,0x7F,0x40,0x40,0x00,0x00};
code unsigned char J_8x16_ASCII[16] = {0x00,0x00,0x10,0x10,0xF0,0x10,0x10,0x00,0x30,0x40,0x40,0x40,0x3F,0x00,0x00,0x00};
code unsigned char K_8x16_ASCII[16] = {0x00,0xF0,0x00,0x80,0x60,0x10,0x00,0x00,0x00,0x7F,0x02,0x0D,0x30,0x40,0x00,0x00};
code unsigned char L_8x16_ASCII[16] = {0x00,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x40,0x7F,0x40,0x40,0x40,0x70,0x00,0x00};
code unsigned char M_8x16_ASCII[16] = {0xF0,0x60,0x80,0x00,0x80,0x60,0xF0,0x00,0x7F,0x00,0x03,0x06,0x03,0x00,0x7F,0x00};
code unsigned char N_8x16_ASCII[16] = {0x00,0xF0,0x60,0x80,0x00,0x00,0xF0,0x00,0x00,0x7F,0x00,0x01,0x0E,0x30,0x7F,0x00};
code unsigned char O_8x16_ASCII[16] = {0xC0,0x20,0x10,0x10,0x10,0x20,0xC0,0x00,0x1F,0x20,0x40,0x40,0x40,0x20,0x1F,0x00};
code unsigned char P_8x16_ASCII[16] = {0x00,0xF0,0x10,0x10,0x10,0x20,0xC0,0x00,0x00,0x7F,0x02,0x02,0x02,0x01,0x00,0x00};
code unsigned char Q_8x16_ASCII[16] = {0xC0,0x20,0x10,0x10,0x10,0x20,0xC0,0x00,0x1F,0x20,0x40,0x48,0x50,0x20,0x5F,0x00};
code unsigned char R_8x16_ASCII[16] = {0x00,0xF0,0x10,0x10,0x10,0x20,0xC0,0x00,0x00,0x7F,0x02,0x06,0x1A,0x21,0x40,0x00};
code unsigned char S_8x16_ASCII[16] = {0xC0,0x20,0x10,0x10,0x10,0x20,0x40,0x00,0x10,0x21,0x42,0x42,0x44,0x24,0x18,0x00};
code unsigned char T_8x16_ASCII[16] = {0x10,0x10,0x10,0xF0,0x10,0x10,0x10,0x00,0x00,0x00,0x00,0x7F,0x00,0x00,0x00,0x00};
code unsigned char U_8x16_ASCII[16] = {0xF0,0x00,0x00,0x00,0x00,0x00,0xF0,0x00,0x1F,0x20,0x40,0x40,0x40,0x20,0x1F,0x00};
code unsigned char V_8x16_ASCII[16] = {0x70,0x80,0x00,0x00,0x00,0x80,0x70,0x00,0x00,0x03,0x1C,0x60,0x1C,0x03,0x00,0x00};
code unsigned char W_8x16_ASCII[16] = {0xF0,0x00,0x00,0xE0,0x00,0x00,0xF0,0x00,0x7F,0x30,0x0E,0x01,0x0E,0x30,0x7F,0x00};
code unsigned char X_8x16_ASCII[16] = {0x10,0x60,0x80,0x00,0x80,0x60,0x10,0x00,0x40,0x30,0x0D,0x02,0x0D,0x30,0x40,0x00};
code unsigned char Y_8x16_ASCII[16] = {0x30,0xC0,0x00,0x00,0x00,0xC0,0x30,0x00,0x00,0x00,0x03,0x7C,0x03,0x00,0x00,0x00};
code unsigned char Z_8x16_ASCII[16] = {0x10,0x10,0x10,0x10,0x10,0xD0,0x30,0x00,0x40,0x60,0x58,0x46,0x41,0x40,0x40,0x00};
code unsigned char a__8x16_ASCII[16] = {0x00,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x39,0x44,0x44,0x44,0x44,0x28,0x7F,0x00};
code unsigned char b__8x16_ASCII[16] = {0x00,0xF8,0x00,0x80,0x80,0x00,0x00,0x00,0x00,0x7F,0x21,0x40,0x40,0x21,0x1E,0x00};
code unsigned char c__8x16_ASCII[16] = {0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x1E,0x21,0x40,0x40,0x40,0x40,0x21,0x00};
code unsigned char d__8x16_ASCII[16] = {0x00,0x00,0x80,0x80,0x00,0xF8,0x00,0x00,0x1E,0x21,0x40,0x40,0x21,0x7F,0x00,0x00};
code unsigned char e__8x16_ASCII[16] = {0x00,0x00,0x80,0x80,0x80,0x00,0x00,0x00,0x1E,0x25,0x44,0x44,0x44,0x45,0x26,0x00};
code unsigned char f__8x16_ASCII[16] = {0x00,0x80,0x80,0xF8,0x84,0x84,0x18,0x00,0x00,0x00,0x00,0x7F,0x00,0x00,0x00,0x00};
code unsigned char g__8x16_ASCII[16] = {0x00,0xE0,0x10,0x10,0x20,0xF0,0x00,0x00,0x00,0x23,0x44,0x44,0x42,0x3F,0x00,0x00};
code unsigned char h__8x16_ASCII[16] = {0x00,0xF8,0x00,0x80,0x80,0x80,0x00,0x00,0x00,0x7F,0x01,0x00,0x00,0x00,0x7F,0x00};
code unsigned char i__8x16_ASCII[16] = {0x00,0x00,0x00,0x98,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0x00,0x00,0x00,0x00};
code unsigned char j__8x16_ASCII[16] = {0x00,0x00,0x00,0x00,0xF3,0x00,0x00,0x00,0x00,0x20,0x40,0x40,0x3F,0x00,0x00,0x00};
code unsigned char k__8x16_ASCII[16] = {0x00,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0x08,0x14,0x22,0x41,0x00,0x00};
code unsigned char l__8x16_ASCII[16] = {0x00,0x00,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3F,0x40,0x40,0x20,0x00,0x00};
code unsigned char m__8x16_ASCII[16] = {0x80,0x00,0x80,0x00,0x80,0x80,0x00,0x00,0x7F,0x01,0x00,0x7F,0x00,0x00,0x7F,0x00};
code unsigned char n__8x16_ASCII[16] = {0x00,0x80,0x00,0x80,0x80,0x80,0x00,0x00,0x00,0x7F,0x01,0x00,0x00,0x00,0x7F,0x00};
code unsigned char o__8x16_ASCII[16] = {0x00,0x00,0x80,0x80,0x80,0x00,0x00,0x00,0x1E,0x21,0x40,0x40,0x40,0x21,0x1E,0x00};
code unsigned char p__8x16_ASCII[16] = {0x00,0xF0,0x20,0x10,0x10,0x20,0xC0,0x00,0x00,0x7F,0x04,0x08,0x08,0x04,0x03,0x00};
code unsigned char q__8x16_ASCII[16] = {0xC0,0x20,0x10,0x10,0x20,0xF0,0x00,0x00,0x03,0x04,0x08,0x08,0x04,0x7F,0x00,0x00};
code unsigned char r__8x16_ASCII[16] = {0x00,0x00,0x80,0x00,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x7F,0x00,0x00,0x01,0x00};
code unsigned char s__8x16_ASCII[16] = {0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x23,0x44,0x44,0x48,0x48,0x31,0x00};
code unsigned char t__8x16_ASCII[16] = {0x00,0x80,0x80,0xF0,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x3F,0x40,0x40,0x20,0x00};
code unsigned char u__8x16_ASCII[16] = {0x00,0x80,0x00,0x00,0x00,0x00,0x80,0x00,0x00,0x3F,0x40,0x40,0x40,0x20,0x7F,0x00};
code unsigned char v__8x16_ASCII[16] = {0x80,0x00,0x00,0x00,0x00,0x00,0x80,0x00,0x01,0x06,0x18,0x60,0x18,0x06,0x01,0x00};
code unsigned char w__8x16_ASCII[16] = {0x80,0x00,0x00,0x00,0x00,0x00,0x80,0x00,0x1F,0x60,0x18,0x07,0x18,0x60,0x1F,0x00};
code unsigned char x__8x16_ASCII[16] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x41,0x22,0x14,0x08,0x14,0x22,0x41,0x00};
code unsigned char y__8x16_ASCII[16] = {0x60,0x80,0x00,0x00,0x00,0x80,0x60,0x00,0x40,0x21,0x16,0x08,0x06,0x01,0x00,0x00};
code unsigned char z__8x16_ASCII[16] = {0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x40,0x60,0x50,0x48,0x44,0x42,0x41,0x00};
code unsigned char number_0_8x16_ASCII[16] = {0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00,0x00,0x1F,0x20,0x40,0x40,0x20,0x1F,0x00};
code unsigned char number_1_8x16_ASCII[16] = {0x00,0x00,0x20,0x10,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0x00,0x00,0x00};
code unsigned char number_2_8x16_ASCII[16] = {0x20,0x10,0x08,0x08,0x08,0x10,0xE0,0x00,0x60,0x50,0x48,0x44,0x42,0x41,0x40,0x00};
code unsigned char number_3_8x16_ASCII[16] = {0x08,0x88,0xC8,0x28,0x18,0x08,0x00,0x00,0x20,0x40,0x40,0x41,0x21,0x1E,0x00,0x00};
code unsigned char number_4_8x16_ASCII[16] = {0x00,0x00,0xC0,0x30,0x88,0x00,0x00,0x00,0x0C,0x0B,0x08,0x08,0x7F,0x08,0x08,0x00};
code unsigned char number_5_8x16_ASCII[16] = {0x00,0xF8,0x88,0x88,0x88,0x08,0x08,0x00,0x00,0x20,0x40,0x40,0x40,0x21,0x1E,0x00};
code unsigned char number_6_8x16_ASCII[16] = {0x00,0xC0,0x20,0x10,0x08,0x00,0x00,0x00,0x1F,0x22,0x41,0x41,0x41,0x22,0x1C,0x00};
code unsigned char number_7_8x16_ASCII[16] = {0x08,0x08,0x08,0x08,0x88,0x68,0x18,0x00,0x00,0x00,0x00,0x7C,0x03,0x00,0x00,0x00};
code unsigned char number_8_8x16_ASCII[16] = {0x60,0x90,0x08,0x08,0x08,0x90,0x60,0x00,0x1C,0x22,0x41,0x41,0x41,0x22,0x1C,0x00};
code unsigned char number_9_8x16_ASCII[16] = {0xE0,0x10,0x08,0x08,0x08,0x10,0xE0,0x00,0x00,0x01,0x42,0x22,0x12,0x0D,0x03,0x00};
code unsigned char zpt_8x16_ASCII[16] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x58,0x38,0x00,0x00,0x00,0x00};
code unsigned char tck_8x16_ASCII[16] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x00,0x00};
code unsigned char double_tck_8x16_ASCII[16] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x33,0x33,0x00,0x00,0x00,0x00};
code unsigned char minus_8x16_ASCII[16] = {0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
code unsigned char ravno_8x16_ASCII[16] = {0x00,0x40,0x40,0x40,0x40,0x40,0x40,0x00,0x00,0x02,0x02,0x02,0x02,0x02,0x02,0x00};
code unsigned char plus_8x16_ASCII[16] = {0x80,0x80,0x80,0xF8,0x80,0x80,0x80,0x00,0x00,0x00,0x00,0x0F,0x00,0x00,0x00,0x00};
code unsigned char skobka1_8x16_ASCII[16] = {0x00,0x00,0xE0,0x18,0x04,0x02,0x00,0x00,0x00,0x00,0x07,0x18,0x20,0x40,0x00,0x00};
code unsigned char skobka2_8x16_ASCII[16] = {0x00,0x02,0x04,0x18,0xE0,0x00,0x00,0x00,0x00,0x40,0x20,0x18,0x07,0x00,0x00,0x00};
code unsigned char kosaya_right_8x16_ASCII[16] = {0x00,0x00,0x00,0x80,0x60,0x18,0x06,0x00,0x60,0x18,0x06,0x01,0x00,0x00,0x00,0x00};
code unsigned char procent_8x16_ASCII[16] = {0x3C,0x42,0x42,0xBC,0x60,0x10,0x0C,0x00,0xC0,0x30,0x0C,0x7A,0x84,0x84,0x78,0x00};


code unsigned char space_WidthAdjusted_Arial_ASCII[34] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
																											 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
code unsigned char A_WidthAdjusted_Arial_ASCII[34] = {0x00,0x0B,0x00,0x00,0x00,0xE0,0xF8,0x18,0xF8,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,
																											 0x00,0x00,0x40,0x78,0x3F,0x0F,0x0C,0x0C,0x0C,0x0F,0x3F,0x78,0x40,0x00,0x00,0x00,0x00,0x00};
code unsigned char B_WidthAdjusted_Arial_ASCII[34] = {0x00,0x0C,0x00,0xF8,0xF8,0x18,0x18,0x18,0x18,0x18,0xF8,0xF0,0x00,0x00,0x00,0x00,
																											 0x00,0x00,0x00,0x7F,0x7F,0x63,0x63,0x63,0x63,0x63,0x67,0x3E,0x1C,0x00,0x00,0x00,0x00,0x00};
code unsigned char C_WidthAdjusted_Arial_ASCII[34] = {0x00,0x0C,0x00,0xC0,0xF0,0x30,0x18,0x18,0x18,0x18,0x38,0x70,0x20,0x00,0x00,0x00,
																											 0x00,0x00,0x00,0x0F,0x3F,0x30,0x60,0x60,0x60,0x60,0x70,0x38,0x10,0x00,0x00,0x00,0x00,0x00};
code unsigned char D_WidthAdjusted_Arial_ASCII[34] = {0x00,0x0C,0x00,0xF8,0xF8,0x18,0x18,0x18,0x18,0x18,0x30,0xF0,0xC0,0x00,0x00,0x00,
																											 0x00,0x00,0x00,0x7F,0x7F,0x60,0x60,0x60,0x60,0x60,0x30,0x3F,0x0F,0x00,0x00,0x00,0x00,0x00};
code unsigned char E_WidthAdjusted_Arial_ASCII[34] = {0x00,0x0B,0x00,0xF8,0xF8,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x00,0x00,0x00,0x00,
																											 0x00,0x00,0x00,0x7F,0x7F,0x63,0x63,0x63,0x63,0x63,0x63,0x63,0x00,0x00,0x00,0x00,0x00,0x00};
code unsigned char F_WidthAdjusted_Arial_ASCII[34] = {0x00,0x0A,0x00,0xF8,0xF8,0x18,0x18,0x18,0x18,0x18,0x18,0x00,0x00,0x00,0x00,0x00,
																											 0x00,0x00,0x00,0x7F,0x7F,0x03,0x03,0x03,0x03,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
code unsigned char G_WidthAdjusted_Arial_ASCII[34] = {0x00,0x0C,0x00,0xC0,0xF0,0x30,0x18,0x18,0x18,0x18,0x38,0x70,0x20,0x00,0x00,0x00,
																											 0x00,0x00,0x00,0x0F,0x3F,0x30,0x60,0x60,0x66,0x66,0x76,0x3E,0x1E,0x00,0x00,0x00,0x00,0x00};
code unsigned char H_WidthAdjusted_Arial_ASCII[34] = {0x00,0x0C,0x00,0xF8,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0xF8,0x00,0x00,0x00,
																											 0x00,0x00,0x00,0x7F,0x7F,0x03,0x03,0x03,0x03,0x03,0x03,0x7F,0x7F,0x00,0x00,0x00,0x00,0x00};
code unsigned char I_WidthAdjusted_Arial_ASCII[34] = {0x00,0x04,0x00,0xF8,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
																											 0x00,0x00,0x00,0x7F,0x7F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
code unsigned char J_WidthAdjusted_Arial_ASCII[34] = {0x00,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,
																											 0x00,0x00,0x18,0x38,0x70,0x60,0x60,0x70,0x3F,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
code unsigned char K_WidthAdjusted_Arial_ASCII[34] = {0x00,0x0C,0x00,0xF8,0xF8,0x00,0x00,0x80,0xC0,0x60,0x30,0x18,0x08,0x00,0x00,0x00,
																											 0x00,0x00,0x00,0x7F,0x7F,0x06,0x03,0x01,0x03,0x03,0x0F,0x3C,0x70,0x60,0x00,0x00,0x00,0x00};
code unsigned char L_WidthAdjusted_Arial_ASCII[34] = {0x00,0x0A,0x00,0xF8,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
																											 0x00,0x00,0x00,0x7F,0x7F,0x60,0x60,0x60,0x60,0x60,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
code unsigned char M_WidthAdjusted_Arial_ASCII[34] = {0x00,0x0D,0x00,0xF8,0xF8,0x78,0xE0,0x00,0x00,0x00,0xE0,0x78,0xF8,0xF8,0x00,0x00,
																											 0x00,0x00,0x00,0x7F,0x7F,0x00,0x07,0x3F,0x78,0x3F,0x07,0x00,0x7F,0x7F,0x00,0x00,0x00,0x00};
code unsigned char N_WidthAdjusted_Arial_ASCII[34] = {0x00,0x0C,0x00,0xF8,0xF8,0x70,0xE0,0x80,0x00,0x00,0x00,0xF8,0xF8,0x00,0x00,0x00,
																											 0x00,0x00,0x00,0x7F,0x7F,0x00,0x00,0x03,0x07,0x1C,0x38,0x7F,0x7F,0x00,0x00,0x00,0x00,0x00};
code unsigned char O_WidthAdjusted_Arial_ASCII[34] = {0x00,0x0C,0x00,0xC0,0xF0,0x30,0x18,0x18,0x18,0x18,0x30,0xF0,0xC0,0x00,0x00,0x00,
																											 0x00,0x00,0x00,0x0F,0x3F,0x30,0x60,0x60,0x60,0x60,0x30,0x3F,0x0F,0x00,0x00,0x00,0x00,0x00};
code unsigned char P_WidthAdjusted_Arial_ASCII[34] = {0x00,0x0B,0x00,0xF8,0xF8,0x18,0x18,0x18,0x18,0xB8,0xF0,0xE0,0x00,0x00,0x00,0x00,
																											 0x00,0x00,0x00,0x7F,0x7F,0x03,0x03,0x03,0x03,0x03,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
code unsigned char Q_WidthAdjusted_Arial_ASCII[34] = {0x00,0x0C,0x00,0xC0,0xF0,0x18,0x18,0x18,0x18,0x18,0x30,0xF0,0xC0,0x00,0x00,0x00,
																											 0x00,0x00,0x00,0x0F,0x3F,0x30,0x60,0x68,0x78,0x30,0x70,0xFF,0x8F,0x00,0x00,0x00,0x00,0x00};
code unsigned char R_WidthAdjusted_Arial_ASCII[34] = {0x00,0x0C,0x00,0xF8,0xF8,0x18,0x18,0x18,0x18,0x18,0xB8,0xF0,0xE0,0x00,0x00,0x00,
																											 0x00,0x00,0x00,0x7F,0x7F,0x03,0x03,0x03,0x07,0x0F,0x3D,0x79,0x60,0x40,0x00,0x00,0x00,0x00};
code unsigned char S_WidthAdjusted_Arial_ASCII[34] = {0x00,0x0B,0x00,0xE0,0xF0,0x98,0x98,0x18,0x18,0x38,0x70,0x60,0x00,0x00,0x00,0x00,
																											 0x00,0x00,0x00,0x18,0x39,0x71,0x63,0x63,0x63,0x77,0x3E,0x1C,0x00,0x00,0x00,0x00,0x00,0x00};
code unsigned char T_WidthAdjusted_Arial_ASCII[34] = {0x00,0x0A,0x18,0x18,0x18,0x18,0xF8,0xF8,0x18,0x18,0x18,0x18,0x00,0x00,0x00,0x00,
																											 0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0x7F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
code unsigned char U_WidthAdjusted_Arial_ASCII[34] = {0x00,0x0C,0x00,0xF8,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0xF8,0x00,0x00,0x00,
																											 0x00,0x00,0x00,0x1F,0x3F,0x70,0x60,0x60,0x60,0x60,0x70,0x3F,0x1F,0x00,0x00,0x00,0x00,0x00};
code unsigned char V_WidthAdjusted_Arial_ASCII[34] = {0x00,0x0B,0x18,0x78,0xE0,0x80,0x00,0x00,0x00,0x80,0xE0,0x78,0x18,0x00,0x00,0x00,
																											 0x00,0x00,0x00,0x00,0x03,0x0F,0x7C,0x70,0x7C,0x0F,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
code unsigned char W_WidthAdjusted_Arial_ASCII[34] = {0x00,0x0F,0x18,0xF8,0xF0,0x00,0x00,0xC0,0xF8,0x38,0xF8,0xC0,0x00,0x00,0xE0,0xF8,
																											 0x18,0x00,0x00,0x00,0x07,0x7F,0x78,0x7F,0x07,0x00,0x07,0x7F,0x78,0x7F,0x0F,0x00,0x00,0x00};
code unsigned char X_WidthAdjusted_Arial_ASCII[34] = {0x00,0x0B,0x00,0x18,0x38,0xF0,0xC0,0x80,0xC0,0xF0,0x38,0x18,0x00,0x00,0x00,0x00,
																											 0x00,0x00,0x00,0x60,0x70,0x3C,0x0F,0x07,0x0F,0x3C,0x70,0x60,0x00,0x00,0x00,0x00,0x00,0x00};
code unsigned char Y_WidthAdjusted_Arial_ASCII[34] = {0x00,0x0A,0x18,0x38,0xF0,0xC0,0x00,0x00,0xC0,0xF0,0x38,0x18,0x00,0x00,0x00,0x00,
																											 0x00,0x00,0x00,0x00,0x00,0x01,0x7F,0x7F,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
code unsigned char Z_WidthAdjusted_Arial_ASCII[34] = {0x00,0x09,0x00,0x18,0x18,0x18,0x98,0xD8,0xF8,0x38,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                                 0x60,0x70,0x7C,0x6E,0x67,0x61,0x60,0x60,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

code unsigned char a__WidthAdjusted_Arial_ASCII[34] = {0x00,0x09,0x00,0x80,0xC0,0xC0,0xC0,0xC0,0xC0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
																																	0x00,0x39,0x7D,0x6C,0x66,0x66,0x3F,0x7F,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
code unsigned char b__WidthAdjusted_Arial_ASCII[34] = {0x00,0x0A,0x00,0xF8,0xF8,0x80,0xC0,0xC0,0xC0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
																																	0x00,0x7F,0x7F,0x31,0x60,0x60,0x71,0x3F,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
code unsigned char c__WidthAdjusted_Arial_ASCII[34] = {0x00,0x09,0x00,0x00,0x80,0xC0,0xC0,0xC0,0xC0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
																																	0x00,0x1F,0x3F,0x71,0x60,0x60,0x71,0x31,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
code unsigned char d__WidthAdjusted_Arial_ASCII[34] = {0x00,0x0A,0x00,0x00,0x80,0xC0,0xC0,0xC0,0x80,0xF8,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
																																	0x00,0x1F,0x3F,0x71,0x60,0x60,0x31,0x7F,0x7F,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
code unsigned char e__WidthAdjusted_Arial_ASCII[34] = {0x00,0x09,0x00,0x00,0x80,0xC0,0xC0,0xC0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
																																	0x00,0x1F,0x3F,0x76,0x66,0x66,0x37,0x17,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
code unsigned char f__WidthAdjusted_Arial_ASCII[34] = {0x00,0x05,0xC0,0xF0,0xF8,0xD8,0xD8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
																																	0x00,0x7F,0x7F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
code unsigned char g__WidthAdjusted_Arial_ASCII[34] = {0x00,0x0A,0x00,0xC0,0xE0,0x70,0x30,0x30,0x60,0xF0,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
																																	0x00,0x67,0xEF,0xDC,0xD8,0xD8,0xCC,0xFF,0x7F,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
code unsigned char h__WidthAdjusted_Arial_ASCII[34] = {0x00,0x0A,0x00,0xF8,0xF8,0x80,0xC0,0xC0,0xC0,0xC0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
																																	0x00,0x7F,0x7F,0x01,0x00,0x00,0x00,0x7F,0x7F,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
code unsigned char i__WidthAdjusted_Arial_ASCII[34] = {0x00,0x04,0x00,0x00,0xD8,0xD8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
																																	0x00,0x00,0x7F,0x7F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
code unsigned char j__WidthAdjusted_Arial_ASCII[34] = {0x00,0x04,0x00,0xF6,0xF6,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
																																	0xC0,0xFF,0x7F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
code unsigned char k__WidthAdjusted_Arial_ASCII[34] = {0x00,0x09,0x00,0xF8,0xF8,0x00,0x00,0x80,0xC0,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
																																	0x00,0x7F,0x7F,0x0E,0x07,0x1F,0x78,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
code unsigned char l__WidthAdjusted_Arial_ASCII[34] = {0x00,0x04,0x00,0xF8,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
																																	0x00,0x7F,0x7F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
code unsigned char m__WidthAdjusted_Arial_ASCII[34] = {0x00,0x0E,0x00,0xC0,0xC0,0x80,0xC0,0xC0,0xC0,0x80,0x80,0xC0,0xC0,0xC0,0x80,0x00,0x00,0x00,
																																	0x00,0x7F,0x7F,0x01,0x00,0x00,0x7F,0x7F,0x01,0x00,0x00,0x7F,0x7F,0x00,0x00,0x00};
code unsigned char n__WidthAdjusted_Arial_ASCII[34] = {0x00,0x0A,0x00,0xC0,0xC0,0x80,0xC0,0xC0,0xC0,0xC0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
																																	0x00,0x7F,0x7F,0x01,0x00,0x00,0x00,0x7F,0x7F,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
code unsigned char o__WidthAdjusted_Arial_ASCII[34] = {0x00,0x0A,0x00,0x00,0x80,0xC0,0xC0,0xC0,0xC0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
																																	0x00,0x1F,0x3F,0x71,0x60,0x60,0x71,0x3F,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
code unsigned char p__WidthAdjusted_Arial_ASCII[34] = {0x00,0x0A,0x00,0xF0,0xF0,0x60,0x30,0x30,0x70,0xE0,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
																																	0x00,0xFF,0xFF,0x0C,0x18,0x18,0x1C,0x0F,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
code unsigned char q__WidthAdjusted_Arial_ASCII[34] = {0x00,0x0A,0x00,0xC0,0xE0,0x70,0x30,0x30,0x60,0xF0,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
																																	0x00,0x07,0x0F,0x1C,0x18,0x18,0x0C,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
code unsigned char r__WidthAdjusted_Arial_ASCII[34] = {0x00,0x06,0x00,0xC0,0xC0,0x80,0xC0,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
																																	0x00,0x7F,0x7F,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
code unsigned char s__WidthAdjusted_Arial_ASCII[34] = {0x00,0x09,0x00,0x80,0xC0,0xC0,0xC0,0xC0,0xC0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
																																	0x00,0x33,0x77,0x66,0x6E,0x6C,0x7D,0x39,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
code unsigned char t__WidthAdjusted_Arial_ASCII[34] = {0x00,0x05,0xC0,0xF0,0xF8,0xC0,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
																																	0x00,0x3F,0x7F,0x60,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0x7F,0x00,0x00,0x00};
code unsigned char u__WidthAdjusted_Arial_ASCII[34] = {0x00,0x0A,0x00,0xC0,0xC0,0x00,0x00,0x00,0x00,0xC0,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
																																	0x00,0x3F,0x7F,0x60,0x60,0x60,0x30,0x7F,0x7F,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
code unsigned char v__WidthAdjusted_Arial_ASCII[34] = {0x00,0x09,0x00,0xC0,0xC0,0x00,0x00,0x00,0xC0,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
																																	0x00,0x01,0x0F,0x7E,0x70,0x7E,0x0F,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
code unsigned char w__WidthAdjusted_Arial_ASCII[34] = {0x00,0x0D,0xC0,0xC0,0x00,0x00,0x00,0xC0,0xC0,0xC0,0x00,0x00,0x00,0xC0,0xC0,0x00,0x00,0x00,
																																	0x00,0x0F,0x7F,0x70,0x7E,0x0F,0x01,0x0F,0x7E,0x70,0x7F,0x0F,0x00,0x00,0x00,0x00};
code unsigned char x__WidthAdjusted_Arial_ASCII[34] = {0x00,0x09,0x00,0xC0,0xC0,0x80,0x00,0x80,0xC0,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
																																	0x00,0x60,0x71,0x3F,0x0E,0x3F,0x71,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
code unsigned char y__WidthAdjusted_Arial_ASCII[34] = {0x00,0x09,0x30,0xF0,0xC0,0x00,0x00,0x00,0xC0,0xF0,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
																																	0x00,0xC0,0xC7,0xFF,0x7C,0x1F,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
code unsigned char z__WidthAdjusted_Arial_ASCII[34] = {0x00,0x09,0x00,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
																																	0x00,0x60,0x78,0x7C,0x6E,0x67,0x63,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

code unsigned char number_0_WidthAdjusted_Arial_ASCII[34] = {0x00,0x09,0xE0,0xF0,0x38,0x18,0x18,0x38,0xF0,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
																																			  0x1F,0x3F,0x70,0x60,0x60,0x70,0x3F,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
code unsigned char number_1_WidthAdjusted_Arial_ASCII[34] = {0x00,0x09,0x00,0xC0,0x60,0x30,0xF8,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
																																				0x00,0x00,0x00,0x00,0x7F,0x7F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
code unsigned char number_2_WidthAdjusted_Arial_ASCII[34] = {0x00,0x09,0x60,0x70,0x38,0x18,0x18,0x18,0xF0,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
																																				0x60,0x70,0x78,0x6C,0x6E,0x67,0x63,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
code unsigned char number_3_WidthAdjusted_Arial_ASCII[34] = {0x00,0x09,0x20,0x30,0x18,0x98,0x98,0x98,0xF8,0x70,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
																																				0x18,0x38,0x70,0x61,0x61,0x73,0x3F,0x1E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
code unsigned char number_4_WidthAdjusted_Arial_ASCII[34] = {0x00,0x09,0x00,0x00,0x80,0xC0,0x70,0xF8,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
																																				0x1C,0x1E,0x1B,0x19,0x18,0x7F,0x7F,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
code unsigned char number_5_WidthAdjusted_Arial_ASCII[34] = {0x00,0x09,0xC0,0xF8,0xF8,0x98,0x98,0x98,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
																																				0x1B,0x3B,0x71,0x61,0x73,0x3F,0x1E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
code unsigned char number_6_WidthAdjusted_Arial_ASCII[34] = {0x00,0x09,0xC0,0xF0,0x38,0x98,0x98,0x98,0x38,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
																																				0x0F,0x3F,0x73,0x61,0x61,0x63,0x3F,0x1E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
code unsigned char number_7_WidthAdjusted_Arial_ASCII[34] = {0x00,0x09,0x18,0x18,0x18,0x18,0xD8,0xF8,0x38,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
																																				0x00,0x00,0x78,0x7F,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
code unsigned char number_8_WidthAdjusted_Arial_ASCII[34] = {0x00,0x09,0xE0,0xF0,0x18,0x18,0x18,0x18,0xF0,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
																																				0x1C,0x3F,0x63,0x63,0x63,0x63,0x3F,0x1C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
code unsigned char number_9_WidthAdjusted_Arial_ASCII[34] = {0x00,0x09,0xE0,0xF0,0x18,0x18,0x18,0x38,0xF0,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
																																				0x31,0x73,0x67,0x66,0x66,0x73,0x3F,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
code unsigned char zpt_WidthAdjusted_Arial_ASCII[34] = {0x00,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
																																	 0x98,0x78,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
code unsigned char tck_WidthAdjusted_Arial_ASCII[34] = {0x00,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
																																	 0x60,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
																																	 

unsigned long int getAddress_5x7_ASCII(unsigned int ASCIICODE){
	if(ASCIICODE >= 0x20 && ASCIICODE <= 0xFF){
		Address = (ASCIICODE - 32.0) * 8.0;
	}	
	return Address;
}

unsigned long int getAddress_7x8_ASCII(unsigned int ASCIICODE){
	if(ASCIICODE >= 0x20 && ASCIICODE <= 0xFF){
		Address = ((ASCIICODE - 32.0) * 8.0) + 768.0;
	}	
	return Address;
}

unsigned long int getAddress_8x16_ASCII(unsigned int ASCIICODE){
	if(ASCIICODE >= 0x20 && ASCIICODE <= 0xFF){
		Address = ((ASCIICODE - 32.0) * 16.0) + 1536.0;
	}	
	return Address;
}


unsigned long int getAddress_WidthAdjusted_Arial_ASCII(unsigned int ASCIICODE){	
	if(ASCIICODE >= 0x20 && ASCIICODE <= 0xFF)
		Address = ((ASCIICODE - 32.0)) * 34.0 + 3072.0;	
	return Address;
}
/*unsigned long int getAddress_WidthAdjusted_Latin(unsigned int UNICODE){	
	if(UNICODE >= 0x0020 && UNICODE <= 0x007F)
		Address = ((UNICODE - 0x0020) * 34.0) + 22976.0;	
	else if(UNICODE >= 0x00A0 && UNICODE <= 0x017F)
		Address = ((UNICODE - 0x00A0 + 96.0) * 34.0) + 22976.0;
	else if(UNICODE >= 0x01A0 && UNICODE <= 0x01CF)
		Address = ((UNICODE - 0x01A0 + 320.0) * 34.0) + 22976.0;
	else if(UNICODE >= 0x01F0 && UNICODE <= 0x01FF)
		Address = ((UNICODE - 0x01F0 + 368.0) * 34.0) + 22976.0;
	else if(UNICODE >= 0x0210 && UNICODE <= 0x021F)
		Address = ((UNICODE - 0x0210 + 384.0) * 34.0) + 22976.0;
	else if(UNICODE >= 0x1EA0 && UNICODE <= 0x1EFF)
		Address = ((UNICODE - 0x1EA0 + 400.0) * 34.0) + 22976.0;
	return Address;
}*/
unsigned long int getAddress_8x16_Latin(unsigned int UNICODE){
	if(UNICODE >= 0x0020 && UNICODE <= 0x007F)
		Address = ((UNICODE - 32.0) * 16.0) + 6336.0;
	else if(UNICODE >= 0x00A0 && UNICODE <= 0x017F)
		Address = ((UNICODE - 160.0 + 96.0) * 16.0) + 6336.0;
	else if(UNICODE >= 0x01A0 && UNICODE <= 0x01CF)
		Address = ((UNICODE - 416.0 + 320.0) * 16.0) + 6336.0;
	else if(UNICODE >= 0x01F0 && UNICODE <= 0x01FF)
		Address = ((UNICODE - 496.0 + 368.0) * 16.0) + 6336.0;
	else if(UNICODE >= 0x0210 && UNICODE <= 0x021F)
		Address = ((UNICODE - 582.0 + 384.0) * 16.0) + 6336.0;
	else if(UNICODE >= 0x1EA0 && UNICODE <= 0x1EFF)
		Address = ((UNICODE - 7840.0 + 400.0) * 16.0) + 6336.0;	
	return Address;
}

/*unsigned long int getAddress_GB2312(unsigned int GBCODE){
	MSB = (GBCODE & 0xFF00)>>8;
	LSB = (GBCODE & 0x00FF);
	
	if(MSB >= 0xA1 && MSB <= 0xA9 && LSB >= 0xA1)
		Address = (((MSB - 161.0)) * 94.0 + (LSB-161.0)) * 32.0 + 69760.0;
	else if(MSB>=0xB0 && MSB <= 0xF7 && LSB >= 0xA1)
		Address = (((MSB - 176.0) * 94.0) + (LSB-161.0)+846.0) * 32.0 + 69760.0;	
	return Address;
}*/

/*unsigned long int getAddress_5x10_LCM_1(unsigned int UNICODE){
	if(UNICODE >= 0x0000 && UNICODE <= 0x00FF)
		Address = (UNICODE * 10.0) + 961328.0;	
	return Address;
}*/
/*unsigned long int getAddress_5x10_LCM_2(unsigned int UNICODE){
	if(UNICODE >= 0x0000 && UNICODE <= 0x00FF)
		Address = (UNICODE * 10.0) + 961328.0 + 2560.0;	
	return Address;
}*/
/*unsigned long int getAddress_5x10_LCM_3(unsigned int UNICODE){
	if(UNICODE >= 0x0000 && UNICODE <= 0x00FF)
		Address = (UNICODE * 10.0) + 961328.0 + 2560.0 * 2.0;	
	return Address;
}*/
/*unsigned long int getAddress_5x10_LCM_8(unsigned int UNICODE){
	if(UNICODE >= 0x0000 && UNICODE <= 0x00FF)
		Address = (UNICODE * 10.0) + 961328.0 + 2560.0 * 3.0;	
	return Address;
}*/
/*unsigned long int getAddress_5x10_LCM_11(unsigned int UNICODE){
	if(UNICODE >= 0x0000 && UNICODE <= 0x00FF)
		Address = (UNICODE * 10.0) + 961328.0 + 2560.0 * 4.0;	
	return Address;
}*/
/*unsigned long int getAddress_5x10_LCM_12(unsigned int UNICODE){
	if(UNICODE >= 0x0000 && UNICODE <= 0x00FF)
		Address = (UNICODE * 10.0) + 961328.0 + 2560.0 * 5.0;	
	return Address;
}*/
/*unsigned long int getAddress_5x10_LCM_13(unsigned int UNICODE){
	if(UNICODE >= 0x0000 && UNICODE <= 0x00FF)
		Address = (UNICODE * 10.0) + 961328.0 + 2560.0 * 6.0;	
	return Address;
}*/
unsigned test_bit(unsigned aValue, unsigned aNumber){
		return aValue & (1 << aNumber);
}
void sendMF_byte_to_Display(unsigned char data_byte){	
	SFRPAGE = 0x0F;
	MF_SI = 0;
	MF_SCLK = 0;	
	for(t = 0; t < 8; t++){	
		MF_SCLK = 0;
		MF_SI = 0;
		if(test_bit(data_byte, (7-t)) > 0)
			MF_SI = 1;
		else
			MF_SI = 0;		
		MF_SCLK = 1;		
	}
	MF_SCLK = 0;
	MF_SI = 0;
}

void send24bitAddressToDisplay(){

	sendMF_byte_to_Display(address_BYTE1_MSB);
	sendMF_byte_to_Display(address_BYTE2);
	sendMF_byte_to_Display(address_BYTE3_LSB);
}

void clearArray(unsigned char* arr){		
	while(*arr != 0)
		*arr++ = 0;
}
unsigned char readByteFromDisplay(){
	ReadByte = 0x00;
	MF_SCLK = 0;	
	for(r = 7; r >= 0; r--){					
		MF_SCLK = 1;
		delay100mkS();		
		if(MF_SO == 1)
			ReadByte |= 1<<r;
		MF_SCLK = 0;
		delay100mkS();
	}	
	return ReadByte;
}


/*
void read_5x7_ASCII_CharFromDisplay(unsigned char ch){	
	SFRPAGE = 0x0F;
	//MF_SCLK = 0;	
	for(f = 0;f < 8;f++){
		ReadByte = 0x00;
		MF_SCLK = 0;
		for(r = 7; r >= 0; r--){	
			
			MF_SCLK = 0;
			//delay100mkS();
			
			if(MF_SO == 1)
				ReadByte |= 1<<r;
			else 					
				ReadByte |= 0<<r;			
			
			MF_SCLK = 1;
			//delay100mkS();			
	  }
		//font5x7[f] = ReadByte;	
		//M_5x7_ASCII[f] = ReadByte;
		switch(ch){
			case 'A': A_5x7_ASCII[f] = ReadByte; break;
			case 'B': B_5x7_ASCII[f] = ReadByte; break;
			case 'C': C_5x7_ASCII[f] = ReadByte; break;
			case 'D': D_5x7_ASCII[f] = ReadByte; break;
			case 'E': E_5x7_ASCII[f] = ReadByte; break;
			case 'F': F_5x7_ASCII[f] = ReadByte; break;
			case 'G': G_5x7_ASCII[f] = ReadByte; break;
			case 'H': H_5x7_ASCII[f] = ReadByte; break;
			case 'I': I_5x7_ASCII[f] = ReadByte; break;
			case 'J': J_5x7_ASCII[f] = ReadByte; break;
			case 'K': K_5x7_ASCII[f] = ReadByte; break;
			case 'L': L_5x7_ASCII[f] = ReadByte; break;
			case 'M': M_5x7_ASCII[f] = ReadByte; break;
			case 'N': N_5x7_ASCII[f] = ReadByte; break;
			case 'O': O_5x7_ASCII[f] = ReadByte; break;
			case 'P': P_5x7_ASCII[f] = ReadByte; break;
			case 'Q': Q_5x7_ASCII[f] = ReadByte; break;
			case 'R': R_5x7_ASCII[f] = ReadByte; break;
			case 'S': S_5x7_ASCII[f] = ReadByte; break;
			case 'T': T_5x7_ASCII[f] = ReadByte; break;
			case 'U': U_5x7_ASCII[f] = ReadByte; break;
			case 'V': V_5x7_ASCII[f] = ReadByte; break;
			case 'W': W_5x7_ASCII[f] = ReadByte; break;
			case 'X': X_5x7_ASCII[f] = ReadByte; break;
			case 'Y': Y_5x7_ASCII[f] = ReadByte; break;
			case 'Z': Z_5x7_ASCII[f] = ReadByte; break;
			case '<': left_5x7_ASCII[f] = ReadByte; break;
			case '>': right_5x7_ASCII[f] = ReadByte; break;
			case '\"': quote_5x7_ASCII[f] = ReadByte; break;
			case ',': zpt_5x7_ASCII[f] = ReadByte; break;
		}		
	}		
}
*/
void read_7x8_ASCII_CharFromDisplay(){	
	SFRPAGE = 0x0F;
	//MF_SCLK = 0;	
	for(f = 0;f < 8;f++){
		ReadByte = 0x00;
		MF_SCLK = 0;
		for(r = 7; r >= 0; r--){	
			
			MF_SCLK = 0;
			//delay100mkS();
			
			if(MF_SO == 1)
				ReadByte |= 1<<r;
			else 					
				ReadByte |= 0<<r;			
			
			MF_SCLK = 1;
			//delay100mkS();			
	  }
		font7x8[f] = ReadByte;	
	}		
}
/*
void read_8x16_ASCII_CharFromDisplay(unsigned char ch){	
	SFRPAGE = 0x0F;
	//MF_SCLK = 0;	
	for(f = 0;f < 16;f++){
		ReadByte = 0x00;
		MF_SCLK = 0;
		delay10mkS();
		for(r = 7; r >= 0; r--){	
			
			MF_SCLK = 0;
			//delay100mkS();
			
			if(MF_SO == 1)
				ReadByte |= 1<<r;
			else 					
				ReadByte |= 0<<r;			
			
			MF_SCLK = 1;
			delay10mkS();
			//delay100mkS();			
	  }
		//font8x16[f] = ReadByte;	
		switch(ch){
			case 'A': A_8x16_ASCII[f] = ReadByte; break;
			case 'B': B_8x16_ASCII[f] = ReadByte; break;
			case 'C': C_8x16_ASCII[f] = ReadByte; break;
			case 'D': D_8x16_ASCII[f] = ReadByte; break;
			case 'E': E_8x16_ASCII[f] = ReadByte; break;
			case 'F': F_8x16_ASCII[f] = ReadByte; break;
			case 'G': G_8x16_ASCII[f] = ReadByte; break;
			case 'H': H_8x16_ASCII[f] = ReadByte; break;
			case 'I': I_8x16_ASCII[f] = ReadByte; break;
			case 'J': J_8x16_ASCII[f] = ReadByte; break;
			case 'K': K_8x16_ASCII[f] = ReadByte; break;
			case 'L': L_8x16_ASCII[f] = ReadByte; break;
			case 'M': M_8x16_ASCII[f] = ReadByte; break;
			case 'N': N_8x16_ASCII[f] = ReadByte; break;
			case 'O': O_8x16_ASCII[f] = ReadByte; break;
			case 'P': P_8x16_ASCII[f] = ReadByte; break;
			case 'Q': Q_8x16_ASCII[f] = ReadByte; break;
			case 'R': R_8x16_ASCII[f] = ReadByte; break;
			case 'S': S_8x16_ASCII[f] = ReadByte; break;
			case 'T': T_8x16_ASCII[f] = ReadByte; break;
 			case 'U': U_8x16_ASCII[f] = ReadByte; break;
			case 'V': V_8x16_ASCII[f] = ReadByte; break;
			case 'W': W_8x16_ASCII[f] = ReadByte; break;
			case 'X': X_8x16_ASCII[f] = ReadByte; break;
			case 'Y': Y_8x16_ASCII[f] = ReadByte; break;
			case 'Z': Z_8x16_ASCII[f] = ReadByte; break;
			case 'a': a__8x16_ASCII[f] = ReadByte; break;
			case 'b': b__8x16_ASCII[f] = ReadByte; break;
			case 'c': c__8x16_ASCII[f] = ReadByte; break;
			case 'd': d__8x16_ASCII[f] = ReadByte; break;
			case 'e': e__8x16_ASCII[f] = ReadByte; break;
			case 'f': f__8x16_ASCII[f] = ReadByte; break;
			case 'g': g__8x16_ASCII[f] = ReadByte; break;
			case 'h': h__8x16_ASCII[f] = ReadByte; break;
			case 'i': i__8x16_ASCII[f] = ReadByte; break;
			case 'j': j__8x16_ASCII[f] = ReadByte; break;
			case 'k': k__8x16_ASCII[f] = ReadByte; break;
			case 'l': l__8x16_ASCII[f] = ReadByte; break;
			case 'm': m__8x16_ASCII[f] = ReadByte; break;
			case 'n': n__8x16_ASCII[f] = ReadByte; break;
			case 'o': o__8x16_ASCII[f] = ReadByte; break;
			case 'p': p__8x16_ASCII[f] = ReadByte; break;
			case 'q': q__8x16_ASCII[f] = ReadByte; break;
			case 'r': r__8x16_ASCII[f] = ReadByte; break;
			case 's': s__8x16_ASCII[f] = ReadByte; break;
			case 't': t__8x16_ASCII[f] = ReadByte; break;
			case 'u': u__8x16_ASCII[f] = ReadByte; break;
			case 'v': v__8x16_ASCII[f] = ReadByte; break;
			case 'w': w__8x16_ASCII[f] = ReadByte; break;
			case 'x': x__8x16_ASCII[f] = ReadByte; break;
			case 'y': y__8x16_ASCII[f] = ReadByte; break;
			case 'z': z__8x16_ASCII[f] = ReadByte; break;
			case '0': number_0_8x16_ASCII[f] = ReadByte; break;
			case '1': number_1_8x16_ASCII[f] = ReadByte; break;
			case '2': number_2_8x16_ASCII[f] = ReadByte; break;
			case '3': number_3_8x16_ASCII[f] = ReadByte; break;
			case '4': number_4_8x16_ASCII[f] = ReadByte; break;
			case '5': number_5_8x16_ASCII[f] = ReadByte; break;
			case '6': number_6_8x16_ASCII[f] = ReadByte; break;
			case '7': number_7_8x16_ASCII[f] = ReadByte; break;			
			case '8': number_8_8x16_ASCII[f] = ReadByte; break;
			case '9': number_9_8x16_ASCII[f] = ReadByte; break;
			case ',': zpt_8x16_ASCII[f] = ReadByte; break;
			case '.': tck_8x16_ASCII[f] = ReadByte; break;
			case ':': double_tck_8x16_ASCII[f] = ReadByte; break;
			case '-': minus_8x16_ASCII[f] = ReadByte; break;
			case '=': ravno_8x16_ASCII[f] = ReadByte; break;
			case '+': plus_8x16_ASCII[f] = ReadByte; break;
			case '(': skobka1_8x16_ASCII[f] = ReadByte; break;
			case ')': skobka2_8x16_ASCII[f] = ReadByte; break;
			case '/': kosaya_right_8x16_ASCII[f] = ReadByte; break;
			case '%': procent_8x16_ASCII[f] = ReadByte; break;
		}	
	}		
}
*/
/*
void read_16x16_Font_CharFromDisplay(unsigned char ch){	
	SFRPAGE = 0x0F;
	//MF_SCLK = 0;	
	for(f = 0;f < 34;f++){
		ReadByte = 0x00;
		MF_SCLK = 0;
		for(r = 7; r >= 0; r--){	
			
			MF_SCLK = 0;
			//delay100mkS();
			delay10mkS();
			if(MF_SO == 1)
				ReadByte |= 1<<r;
			else 					
				ReadByte |= 0<<r;			
			
			MF_SCLK = 1;
			delay10mkS();
			//delay100mkS();			
	  }
		//font16x16[f] = ReadByte;
		switch(ch){
			case 'A': A_WidthAdjusted_Arial_ASCII[f] = ReadByte; break;
			case 'B': B_WidthAdjusted_Arial_ASCII[f] = ReadByte; break;
			case 'C': C_WidthAdjusted_Arial_ASCII[f] = ReadByte; break;
			case 'D': D_WidthAdjusted_Arial_ASCII[f] = ReadByte; break;
			case 'E': E_WidthAdjusted_Arial_ASCII[f] = ReadByte; break;
			case 'F': F_WidthAdjusted_Arial_ASCII[f] = ReadByte; break;
			case 'G': G_WidthAdjusted_Arial_ASCII[f] = ReadByte; break;
			case 'H': H_WidthAdjusted_Arial_ASCII[f] = ReadByte; break;
			case 'I': I_WidthAdjusted_Arial_ASCII[f] = ReadByte; break;
			case 'J': J_WidthAdjusted_Arial_ASCII[f] = ReadByte; break;
			case 'K': K_WidthAdjusted_Arial_ASCII[f] = ReadByte; break;
			case 'L': L_WidthAdjusted_Arial_ASCII[f] = ReadByte; break;
			case 'M': M_WidthAdjusted_Arial_ASCII[f] = ReadByte; break;
			case 'N': N_WidthAdjusted_Arial_ASCII[f] = ReadByte; break;
			case 'O': O_WidthAdjusted_Arial_ASCII[f] = ReadByte; break;
			case 'P': P_WidthAdjusted_Arial_ASCII[f] = ReadByte; break;
			case 'Q': Q_WidthAdjusted_Arial_ASCII[f] = ReadByte; break;
			case 'R': R_WidthAdjusted_Arial_ASCII[f] = ReadByte; break;
			case 'S': S_WidthAdjusted_Arial_ASCII[f] = ReadByte; break;
			case 'T': T_WidthAdjusted_Arial_ASCII[f] = ReadByte; break;
 			case 'U': U_WidthAdjusted_Arial_ASCII[f] = ReadByte; break;
			case 'V': V_WidthAdjusted_Arial_ASCII[f] = ReadByte; break;
			case 'W': W_WidthAdjusted_Arial_ASCII[f] = ReadByte; break;
			case 'X': X_WidthAdjusted_Arial_ASCII[f] = ReadByte; break;
			case 'Y': Y_WidthAdjusted_Arial_ASCII[f] = ReadByte; break;
			case 'Z': Z_WidthAdjusted_Arial_ASCII[f] = ReadByte; break;
			case 'a': a__WidthAdjusted_Arial_ASCII[f] = ReadByte; break;
			case 'b': b__WidthAdjusted_Arial_ASCII[f] = ReadByte; break;
			case 'c': c__WidthAdjusted_Arial_ASCII[f] = ReadByte; break;
			case 'd': d__WidthAdjusted_Arial_ASCII[f] = ReadByte; break;
			case 'e': e__WidthAdjusted_Arial_ASCII[f] = ReadByte; break;
			case 'f': f__WidthAdjusted_Arial_ASCII[f] = ReadByte; break;
			case 'g': g__WidthAdjusted_Arial_ASCII[f] = ReadByte; break;
			case 'h': h__WidthAdjusted_Arial_ASCII[f] = ReadByte; break;
			case 'i': i__WidthAdjusted_Arial_ASCII[f] = ReadByte; break;
			case 'j': j__WidthAdjusted_Arial_ASCII[f] = ReadByte; break;
			case 'k': k__WidthAdjusted_Arial_ASCII[f] = ReadByte; break;
			case 'l': l__WidthAdjusted_Arial_ASCII[f] = ReadByte; break;
			case 'm': m__WidthAdjusted_Arial_ASCII[f] = ReadByte; break;
			case 'n': n__WidthAdjusted_Arial_ASCII[f] = ReadByte; break;
			case 'o': o__WidthAdjusted_Arial_ASCII[f] = ReadByte; break;
			case 'p': p__WidthAdjusted_Arial_ASCII[f] = ReadByte; break;
			case 'q': q__WidthAdjusted_Arial_ASCII[f] = ReadByte; break;
			case 'r': r__WidthAdjusted_Arial_ASCII[f] = ReadByte; break;
			case 's': s__WidthAdjusted_Arial_ASCII[f] = ReadByte; break;
			case 't': t__WidthAdjusted_Arial_ASCII[f] = ReadByte; break;
			case 'u': u__WidthAdjusted_Arial_ASCII[f] = ReadByte; break;
			case 'v': v__WidthAdjusted_Arial_ASCII[f] = ReadByte; break;
			case 'w': w__WidthAdjusted_Arial_ASCII[f] = ReadByte; break;
			case 'x': x__WidthAdjusted_Arial_ASCII[f] = ReadByte; break;
			case 'y': y__WidthAdjusted_Arial_ASCII[f] = ReadByte; break;
			case 'z': z__WidthAdjusted_Arial_ASCII[f] = ReadByte; break;
			case '0': number_0_WidthAdjusted_Arial_ASCII[f] = ReadByte; break;
			case '1': number_1_WidthAdjusted_Arial_ASCII[f] = ReadByte; break;
			case '2': number_2_WidthAdjusted_Arial_ASCII[f] = ReadByte; break;
			case '3': number_3_WidthAdjusted_Arial_ASCII[f] = ReadByte; break;
			case '4': number_4_WidthAdjusted_Arial_ASCII[f] = ReadByte; break;
			case '5': number_5_WidthAdjusted_Arial_ASCII[f] = ReadByte; break;
			case '6': number_6_WidthAdjusted_Arial_ASCII[f] = ReadByte; break;
			case '7': number_7_WidthAdjusted_Arial_ASCII[f] = ReadByte; break;			
			case '8': number_8_WidthAdjusted_Arial_ASCII[f] = ReadByte; break;
			case '9': number_9_WidthAdjusted_Arial_ASCII[f] = ReadByte; break;
			case ',': zpt_WidthAdjusted_Arial_ASCII[f] = ReadByte; break;
			case '.': tck_WidthAdjusted_Arial_ASCII[f] = ReadByte; break;			
		}	
	}		
}
*/

/*void read_GB2312_CharFromDisplay(){	
	SFRPAGE = 0x0F;
	//MF_SCLK = 0;	
	for(f = 0;f < 32;f++){
		ReadByte = 0x00;
		MF_SCLK = 0;
		for(r = 7; r >= 0; r--){	
			
			MF_SCLK = 0;
			delay100mkS();
			
			if(MF_SO == 1)
				ReadByte |= 1<<r;
			else 					
				ReadByte |= 0<<r;			
			
			MF_SCLK = 1;
			delay100mkS();			
	  }
		fontGB2312[f] = ReadByte;	
	}		
}*/

/*void drawChar5x7ASCII_toDisplay(unsigned char col, unsigned char row){
	Set_Column_Address(col, col+1);
	Set_Row_Address(row,row+7);
	writeRAMCommand();
  for(dr = 0;dr < 8;dr++){
			draw_byte = 0x00;
			if(test_bit(font5x7[0], dr) > 0)
				draw_byte+=0xF0;
			if(test_bit(font5x7[1], dr) > 0)
				draw_byte+=0x0F;
			oled_Data(draw_byte);
			
			draw_byte = 0x00;
			if(test_bit(font5x7[2], dr) > 0)
				draw_byte+=0xF0;
			if(test_bit(font5x7[3], dr) > 0)
				draw_byte+=0x0F;
			oled_Data(draw_byte);
	}
	for(dr = 0;dr < 8;dr++){
			draw_byte = 0x00;
			if(test_bit(font5x7[4], dr) > 0)
				draw_byte+=0xF0;
			if(test_bit(font5x7[5], dr) > 0)
				draw_byte+=0x0F;
			oled_Data(draw_byte);
			
			draw_byte = 0x00;
			if(test_bit(font5x7[6], dr) > 0)
				draw_byte+=0xF0;
			if(test_bit(font5x7[7], dr) > 0)
				draw_byte+=0x0F;
			oled_Data(draw_byte);
	}	
}*/
void drawChar5x7ASCII_toDisplay_Array(const unsigned char* arr5x7, unsigned char col, unsigned char row, unsigned char brightness){
	Set_Column_Address(col, col+1);
	Set_Row_Address(row,row+7);
	writeRAMCommand();
  for(dr = 0;dr < 8;dr++){
			draw_byte = 0x00;
			if(test_bit(arr5x7[0], dr) > 0)
				draw_byte+=(brightness << 4);
			if(test_bit(arr5x7[1], dr) > 0)
				draw_byte+=brightness;
			oled_Data(draw_byte);
			
			draw_byte = 0x00;
			if(test_bit(arr5x7[2], dr) > 0)
				draw_byte+=(brightness << 4);
			if(test_bit(arr5x7[3], dr) > 0)
				draw_byte+=brightness;
			oled_Data(draw_byte);
	}
	for(dr = 0;dr < 8;dr++){
			draw_byte = 0x00;
			if(test_bit(arr5x7[4], dr) > 0)
				draw_byte+=(brightness << 4);
			if(test_bit(arr5x7[5], dr) > 0)
				draw_byte+=brightness;
			oled_Data(draw_byte);
			
			draw_byte = 0x00;
			if(test_bit(arr5x7[6], dr) > 0)
				draw_byte+=(brightness << 4);
			if(test_bit(arr5x7[7], dr) > 0)
				draw_byte+=brightness;
			oled_Data(draw_byte);
	}	
}
void drawChar7x8ASCII_toDisplay(unsigned char col, unsigned char row){
	Set_Column_Address(col, col+1);
	Set_Row_Address(row,row+7);
	writeRAMCommand();
  for(dr = 0;dr < 8;dr++){
			draw_byte = 0x00;
			if(test_bit(font7x8[0], dr) > 0)
				draw_byte+=0xF0;
			if(test_bit(font7x8[1], dr) > 0)
				draw_byte+=0x0F;
			oled_Data(draw_byte);
			
			draw_byte = 0x00;
			if(test_bit(font7x8[2], dr) > 0)
				draw_byte+=0xF0;
			if(test_bit(font7x8[3], dr) > 0)
				draw_byte+=0x0F;
			oled_Data(draw_byte);
	}
	for(dr = 0;dr < 8;dr++){
			draw_byte = 0x00;
			if(test_bit(font7x8[4], dr) > 0)
				draw_byte+=0xF0;
			if(test_bit(font7x8[5], dr) > 0)
				draw_byte+=0x0F;
			oled_Data(draw_byte);
			
			draw_byte = 0x00;
			if(test_bit(font7x8[6], dr) > 0)
				draw_byte+=0xF0;
			if(test_bit(font7x8[7], dr) > 0)
				draw_byte+=0x0F;
			oled_Data(draw_byte);
	}	
}

void drawChar8x16_toDisplay(unsigned char col, unsigned char row){
	Set_Column_Address(col, col+1);
	Set_Row_Address(row,row+15);
	writeRAMCommand();	
	for(dr = 0; dr < 8;dr++){
			draw_byte = 0x00;
			if(test_bit(font8x16[0], dr) > 0)
				draw_byte+=0xF0;
			if(test_bit(font8x16[1], dr) > 0)
				draw_byte+=0x0F;
			oled_Data(draw_byte);
			
			draw_byte = 0x00;
			if(test_bit(font8x16[2], dr) > 0)
				draw_byte+=0xF0;
			if(test_bit(font8x16[3], dr) > 0)
				draw_byte+=0x0F;
			oled_Data(draw_byte);		
	}
	for(dr = 0; dr < 8;dr++){
			draw_byte = 0x00;
			if(test_bit(font8x16[8], dr) > 0)
				draw_byte+=0xF0;
			if(test_bit(font8x16[9], dr) > 0)
				draw_byte+=0x0F;
			oled_Data(draw_byte);
			
			draw_byte = 0x00;
			if(test_bit(font8x16[10], dr) > 0)
				draw_byte+=0xF0;
			if(test_bit(font8x16[11], dr) > 0)
				draw_byte+=0x0F;
			oled_Data(draw_byte);		
	}
	for(dr = 0; dr < 8;dr++){
			draw_byte = 0x00;
			if(test_bit(font8x16[4], dr) > 0)
				draw_byte+=0xF0;
			if(test_bit(font8x16[5], dr) > 0)
				draw_byte+=0x0F;
			oled_Data(draw_byte);
			
			draw_byte = 0x00;
			if(test_bit(font8x16[6], dr) > 0)
				draw_byte+=0xF0;
			if(test_bit(font8x16[7], dr) > 0)
				draw_byte+=0x0F;
			oled_Data(draw_byte);		
	}
	for(dr = 0; dr < 8;dr++){
			draw_byte = 0x00;
			if(test_bit(font8x16[12], dr) > 0)
				draw_byte+=0xF0;
			if(test_bit(font8x16[13], dr) > 0)
				draw_byte+=0x0F;
			oled_Data(draw_byte);
			
			draw_byte = 0x00;
			if(test_bit(font8x16[14], dr) > 0)
				draw_byte+=0xF0;
			if(test_bit(font8x16[15], dr) > 0)
				draw_byte+=0x0F;
			oled_Data(draw_byte);		
	}
}

void drawChar8x16_toDisplay_Array(const unsigned char* arr8x16, unsigned char col, unsigned char row, unsigned char brightness){
	Set_Column_Address(col, col+1);
	Set_Row_Address(row,row+15);
	writeRAMCommand();	
	for(dr = 0; dr < 8;dr++){
			draw_byte = 0x00;
			if(test_bit(arr8x16[0], dr) > 0)
				draw_byte+=(brightness << 4);
			if(test_bit(arr8x16[1], dr) > 0)
				draw_byte+=brightness;
			oled_Data(draw_byte);
			
			draw_byte = 0x00;
			if(test_bit(arr8x16[2], dr) > 0)
				draw_byte+=(brightness << 4);
			if(test_bit(arr8x16[3], dr) > 0)
				draw_byte+=brightness;
			oled_Data(draw_byte);		
	}
	for(dr = 0; dr < 8;dr++){
			draw_byte = 0x00;
			if(test_bit(arr8x16[8], dr) > 0)
				draw_byte+=(brightness << 4);
			if(test_bit(arr8x16[9], dr) > 0)
				draw_byte+=brightness;
			oled_Data(draw_byte);
			
			draw_byte = 0x00;
			if(test_bit(arr8x16[10], dr) > 0)
				draw_byte+=(brightness << 4);
			if(test_bit(arr8x16[11], dr) > 0)
				draw_byte+=brightness;
			oled_Data(draw_byte);		
	}
	for(dr = 0; dr < 8;dr++){
			draw_byte = 0x00;
			if(test_bit(arr8x16[4], dr) > 0)
				draw_byte+=(brightness << 4);
			if(test_bit(arr8x16[5], dr) > 0)
				draw_byte+=brightness;
			oled_Data(draw_byte);
			
			draw_byte = 0x00;
			if(test_bit(arr8x16[6], dr) > 0)
				draw_byte+=(brightness << 4);
			if(test_bit(arr8x16[7], dr) > 0)
				draw_byte+=brightness;
			oled_Data(draw_byte);		
	}
	for(dr = 0; dr < 8;dr++){
			draw_byte = 0x00;
			if(test_bit(arr8x16[12], dr) > 0)
				draw_byte+=(brightness << 4);
			if(test_bit(arr8x16[13], dr) > 0)
				draw_byte+=brightness;
			oled_Data(draw_byte);
			
			draw_byte = 0x00;
			if(test_bit(arr8x16[14], dr) > 0)
				draw_byte+=(brightness << 4);
			if(test_bit(arr8x16[15], dr) > 0)
				draw_byte+=brightness;
			oled_Data(draw_byte);	
		}	
}

void drawChar16x16_toDisplay(unsigned char col, unsigned char row){
	Set_Column_Address(col, col+3);
	Set_Row_Address(row,row+15);
	writeRAMCommand();	
	for(dr = 0; dr < 8;dr++){
			draw_byte = 0x00;
			if(test_bit(font16x16[2], dr) > 0)
				draw_byte+=0xF0;
			if(test_bit(font16x16[3], dr) > 0)
				draw_byte+=0x0F;
			oled_Data(draw_byte);
			
			draw_byte = 0x00;
			if(test_bit(font16x16[4], dr) > 0)
				draw_byte+=0xF0;
			if(test_bit(font16x16[5], dr) > 0)
				draw_byte+=0x0F;
			oled_Data(draw_byte);		
	}
	for(dr = 0; dr < 8;dr++){
			draw_byte = 0x00;
			if(test_bit(font16x16[18], dr) > 0)
				draw_byte+=0xF0;
			if(test_bit(font16x16[19], dr) > 0)
				draw_byte+=0x0F;
			oled_Data(draw_byte);
			
			draw_byte = 0x00;
			if(test_bit(font16x16[20], dr) > 0)
				draw_byte+=0xF0;
			if(test_bit(font16x16[21], dr) > 0)
				draw_byte+=0x0F;
			oled_Data(draw_byte);		
	}
	for(dr = 0; dr < 8;dr++){
			draw_byte = 0x00;
			if(test_bit(font16x16[6], dr) > 0)
				draw_byte+=0xF0;
			if(test_bit(font16x16[7], dr) > 0)
				draw_byte+=0x0F;
			oled_Data(draw_byte);
			
			draw_byte = 0x00;
			if(test_bit(font16x16[8], dr) > 0)
				draw_byte+=0xF0;
			if(test_bit(font16x16[9], dr) > 0)
				draw_byte+=0x0F;
			oled_Data(draw_byte);		
	}
	for(dr = 0; dr < 8;dr++){
			draw_byte = 0x00;
			if(test_bit(font16x16[22], dr) > 0)
				draw_byte+=0xF0;
			if(test_bit(font16x16[23], dr) > 0)
				draw_byte+=0x0F;
			oled_Data(draw_byte);
			
			draw_byte = 0x00;
			if(test_bit(font16x16[24], dr) > 0)
				draw_byte+=0xF0;
			if(test_bit(font16x16[25], dr) > 0)
				draw_byte+=0x0F;
			oled_Data(draw_byte);		
	}
	for(dr = 0; dr < 8;dr++){
			draw_byte = 0x00;
			if(test_bit(font16x16[10], dr) > 0)
				draw_byte+=0xF0;
			if(test_bit(font16x16[11], dr) > 0)
				draw_byte+=0x0F;
			oled_Data(draw_byte);
			
			draw_byte = 0x00;
			if(test_bit(font16x16[12], dr) > 0)
				draw_byte+=0xF0;
			if(test_bit(font16x16[13], dr) > 0)
				draw_byte+=0x0F;
			oled_Data(draw_byte);		
	}
	for(dr = 0; dr < 8;dr++){
			draw_byte = 0x00;
			if(test_bit(font16x16[26], dr) > 0)
				draw_byte+=0xF0;
			if(test_bit(font16x16[27], dr) > 0)
				draw_byte+=0x0F;
			oled_Data(draw_byte);
			
			draw_byte = 0x00;
			if(test_bit(font16x16[28], dr) > 0)
				draw_byte+=0xF0;
			if(test_bit(font16x16[29], dr) > 0)
				draw_byte+=0x0F;
			oled_Data(draw_byte);		
	}
	for(dr = 0; dr < 8;dr++){
			draw_byte = 0x00;
			if(test_bit(font16x16[14], dr) > 0)
				draw_byte+=0xF0;
			if(test_bit(font16x16[15], dr) > 0)
				draw_byte+=0x0F;
			oled_Data(draw_byte);
			
			draw_byte = 0x00;
			if(test_bit(font16x16[16], dr) > 0)
				draw_byte+=0xF0;
			if(test_bit(font16x16[17], dr) > 0)
				draw_byte+=0x0F;
			oled_Data(draw_byte);		
	}
	for(dr = 0; dr < 8;dr++){
			draw_byte = 0x00;
			if(test_bit(font16x16[30], dr) > 0)
				draw_byte+=0xF0;
			if(test_bit(font16x16[31], dr) > 0)
				draw_byte+=0x0F;
			oled_Data(draw_byte);
			
			draw_byte = 0x00;
			if(test_bit(font16x16[32], dr) > 0)
				draw_byte+=0xF0;
			if(test_bit(font16x16[33], dr) > 0)
				draw_byte+=0x0F;
			oled_Data(draw_byte);		
	}
}
void drawChar16x16_toDisplay_Array(const unsigned char* arr16x16, unsigned char col, unsigned char row){
	Set_Column_Address(col, col+3);
	Set_Row_Address(row,row+15);
	writeRAMCommand();	
	for(dr = 0; dr < 8;dr++){
			draw_byte = 0x00;
			if(test_bit(arr16x16[2], dr) > 0)
				draw_byte+=0xF0;
			if(test_bit(arr16x16[3], dr) > 0)
				draw_byte+=0x0F;
			oled_Data(draw_byte);
			
			draw_byte = 0x00;
			if(test_bit(arr16x16[4], dr) > 0)
				draw_byte+=0xF0;
			if(test_bit(arr16x16[5], dr) > 0)
				draw_byte+=0x0F;
			oled_Data(draw_byte);		
	}
	for(dr = 0; dr < 8;dr++){
			draw_byte = 0x00;
			if(test_bit(arr16x16[18], dr) > 0)
				draw_byte+=0xF0;
			if(test_bit(arr16x16[19], dr) > 0)
				draw_byte+=0x0F;
			oled_Data(draw_byte);
			
			draw_byte = 0x00;
			if(test_bit(arr16x16[20], dr) > 0)
				draw_byte+=0xF0;
			if(test_bit(arr16x16[21], dr) > 0)
				draw_byte+=0x0F;
			oled_Data(draw_byte);		
	}
	for(dr = 0; dr < 8;dr++){
			draw_byte = 0x00;
			if(test_bit(arr16x16[6], dr) > 0)
				draw_byte+=0xF0;
			if(test_bit(arr16x16[7], dr) > 0)
				draw_byte+=0x0F;
			oled_Data(draw_byte);
			
			draw_byte = 0x00;
			if(test_bit(arr16x16[8], dr) > 0)
				draw_byte+=0xF0;
			if(test_bit(arr16x16[9], dr) > 0)
				draw_byte+=0x0F;
			oled_Data(draw_byte);		
	}
	for(dr = 0; dr < 8;dr++){
			draw_byte = 0x00;
			if(test_bit(arr16x16[22], dr) > 0)
				draw_byte+=0xF0;
			if(test_bit(arr16x16[23], dr) > 0)
				draw_byte+=0x0F;
			oled_Data(draw_byte);
			
			draw_byte = 0x00;
			if(test_bit(arr16x16[24], dr) > 0)
				draw_byte+=0xF0;
			if(test_bit(arr16x16[25], dr) > 0)
				draw_byte+=0x0F;
			oled_Data(draw_byte);		
	}
	for(dr = 0; dr < 8;dr++){
			draw_byte = 0x00;
			if(test_bit(arr16x16[10], dr) > 0)
				draw_byte+=0xF0;
			if(test_bit(arr16x16[11], dr) > 0)
				draw_byte+=0x0F;
			oled_Data(draw_byte);
			
			draw_byte = 0x00;
			if(test_bit(arr16x16[12], dr) > 0)
				draw_byte+=0xF0;
			if(test_bit(arr16x16[13], dr) > 0)
				draw_byte+=0x0F;
			oled_Data(draw_byte);		
	}
	for(dr = 0; dr < 8;dr++){
			draw_byte = 0x00;
			if(test_bit(arr16x16[26], dr) > 0)
				draw_byte+=0xF0;
			if(test_bit(arr16x16[27], dr) > 0)
				draw_byte+=0x0F;
			oled_Data(draw_byte);
			
			draw_byte = 0x00;
			if(test_bit(arr16x16[28], dr) > 0)
				draw_byte+=0xF0;
			if(test_bit(arr16x16[29], dr) > 0)
				draw_byte+=0x0F;
			oled_Data(draw_byte);		
	}
	for(dr = 0; dr < 8;dr++){
			draw_byte = 0x00;
			if(test_bit(arr16x16[14], dr) > 0)
				draw_byte+=0xF0;
			if(test_bit(arr16x16[15], dr) > 0)
				draw_byte+=0x0F;
			oled_Data(draw_byte);
			
			draw_byte = 0x00;
			if(test_bit(arr16x16[16], dr) > 0)
				draw_byte+=0xF0;
			if(test_bit(arr16x16[17], dr) > 0)
				draw_byte+=0x0F;
			oled_Data(draw_byte);		
	}
	for(dr = 0; dr < 8;dr++){
			draw_byte = 0x00;
			if(test_bit(arr16x16[30], dr) > 0)
				draw_byte+=0xF0;
			if(test_bit(arr16x16[31], dr) > 0)
				draw_byte+=0x0F;
			oled_Data(draw_byte);
			
			draw_byte = 0x00;
			if(test_bit(arr16x16[32], dr) > 0)
				draw_byte+=0xF0;
			if(test_bit(arr16x16[33], dr) > 0)
				draw_byte+=0x0F;
			oled_Data(draw_byte);		
	}
}
/*void drawCharGB2312_toDisplay(unsigned char col, unsigned char row){
	
	Set_Column_Address(col, col+3);
	Set_Row_Address(row,row+15);
	writeRAMCommand();
	for(dr = 0;dr < 8;dr++){
			draw_byte = 0x00;
			if(test_bit(fontGB2312[0], dr) > 0)
				draw_byte+=0xF0;
			if(test_bit(fontGB2312[1], dr) > 0)
				draw_byte+=0x0F;
			oled_Data(draw_byte);
			
			draw_byte = 0x00;
			if(test_bit(fontGB2312[2], dr) > 0)
				draw_byte+=0xF0;
			if(test_bit(fontGB2312[3], dr) > 0)
				draw_byte+=0x0F;
			oled_Data(draw_byte);
	}
	for(dr = 0;dr < 8;dr++){
			draw_byte = 0x00;
			if(test_bit(fontGB2312[16], dr) > 0)
				draw_byte+=0xF0;
			if(test_bit(fontGB2312[17], dr) > 0)
				draw_byte+=0x0F;
			oled_Data(draw_byte);
			
			draw_byte = 0x00;
			if(test_bit(fontGB2312[18], dr) > 0)
				draw_byte+=0xF0;
			if(test_bit(fontGB2312[19], dr) > 0)
				draw_byte+=0x0F;
			oled_Data(draw_byte);
	}	
	for(dr = 0;dr < 8;dr++){
			draw_byte = 0x00;
			if(test_bit(fontGB2312[4], dr) > 0)
				draw_byte+=0xF0;
			if(test_bit(fontGB2312[5], dr) > 0)
				draw_byte+=0x0F;
			oled_Data(draw_byte);
			
			draw_byte = 0x00;
			if(test_bit(fontGB2312[6], dr) > 0)
				draw_byte+=0xF0;
			if(test_bit(fontGB2312[7], dr) > 0)
				draw_byte+=0x0F;
			oled_Data(draw_byte);
	}
	for(dr = 0;dr < 8;dr++){
			draw_byte = 0x00;
			if(test_bit(fontGB2312[20], dr) > 0)
				draw_byte+=0xF0;
			if(test_bit(fontGB2312[21], dr) > 0)
				draw_byte+=0x0F;
			oled_Data(draw_byte);
			
			draw_byte = 0x00;
			if(test_bit(fontGB2312[22], dr) > 0)
				draw_byte+=0xF0;
			if(test_bit(fontGB2312[23], dr) > 0)
				draw_byte+=0x0F;
			oled_Data(draw_byte);
	}
	for(dr = 0;dr < 8;dr++){
			draw_byte = 0x00;
			if(test_bit(fontGB2312[8], dr) > 0)
				draw_byte+=0xF0;
			if(test_bit(fontGB2312[9], dr) > 0)
				draw_byte+=0x0F;
			oled_Data(draw_byte);
			
			draw_byte = 0x00;
			if(test_bit(fontGB2312[10], dr) > 0)
				draw_byte+=0xF0;
			if(test_bit(fontGB2312[11], dr) > 0)
				draw_byte+=0x0F;
			oled_Data(draw_byte);
	}
	for(dr = 0;dr < 8;dr++){
			draw_byte = 0x00;
			if(test_bit(fontGB2312[24], dr) > 0)
				draw_byte+=0xF0;
			if(test_bit(fontGB2312[25], dr) > 0)
				draw_byte+=0x0F;
			oled_Data(draw_byte);
			
			draw_byte = 0x00;
			if(test_bit(fontGB2312[26], dr) > 0)
				draw_byte+=0xF0;
			if(test_bit(fontGB2312[27], dr) > 0)
				draw_byte+=0x0F;
			oled_Data(draw_byte);
	}
	for(dr = 0;dr < 8;dr++){
			draw_byte = 0x00;
			if(test_bit(fontGB2312[12], dr) > 0)
				draw_byte+=0xF0;
			if(test_bit(fontGB2312[13], dr) > 0)
				draw_byte+=0x0F;
			oled_Data(draw_byte);
			
			draw_byte = 0x00;
			if(test_bit(fontGB2312[14], dr) > 0)
				draw_byte+=0xF0;
			if(test_bit(fontGB2312[15], dr) > 0)
				draw_byte+=0x0F;
			oled_Data(draw_byte);
	}
	for(dr = 0;dr < 8;dr++){
			draw_byte = 0x00;
			if(test_bit(fontGB2312[28], dr) > 0)
				draw_byte+=0xF0;
			if(test_bit(fontGB2312[29], dr) > 0)
				draw_byte+=0x0F;
			oled_Data(draw_byte);
			
			draw_byte = 0x00;
			if(test_bit(fontGB2312[30], dr) > 0)
				draw_byte+=0xF0;
			if(test_bit(fontGB2312[31], dr) > 0)
				draw_byte+=0x0F;
			oled_Data(draw_byte);
	}
}*/


void put_5x7ASCII_CharOnDisplay(unsigned char ch, unsigned char col, unsigned char row, unsigned char brightness){
	/*
	Address = getAddress_5x7_ASCII(ch);	
	
	address_BYTE1_MSB = (Address & 0x00FF0000)>>16;
	address_BYTE2     = (Address & 0x0000FF00)>>8;
	address_BYTE3_LSB = (Address & 0x000000FF);
	
	SFRPAGE = SPI0_PAGE;
	SPIEN   = 0;	
	SFRPAGE = 0x0F;	
	//MF_SO 	= 1;      //P3^3 = 1;	
	MF_CS2  = 0;	
	*/
	/*
	sendMF_byte_to_Display(0x0B);
	send24bitAddressToDisplay();
	sendMF_byte_to_Display(0x00);
	
	read_5x7_ASCII_CharFromDisplay();
	*/
	/*
	MF_CS2 = 1;
	SFRPAGE = SPI0_PAGE;
	SPIEN = 1;
	SFRPAGE = 0x0F;	
	*/
	//drawChar5x7ASCII_toDisplay(col, row);	
	switch(ch){
		  case ' ': drawChar5x7ASCII_toDisplay_Array(space_5x7_ASCII, col, row, brightness); break;
			case 'A': drawChar5x7ASCII_toDisplay_Array(A_5x7_ASCII, col, row, brightness); break;
			case 'B': drawChar5x7ASCII_toDisplay_Array(B_5x7_ASCII, col, row, brightness); break;
			case 'C': drawChar5x7ASCII_toDisplay_Array(C_5x7_ASCII, col, row, brightness); break;
			case 'D': drawChar5x7ASCII_toDisplay_Array(D_5x7_ASCII, col, row, brightness); break;
			case 'E': drawChar5x7ASCII_toDisplay_Array(E_5x7_ASCII, col, row, brightness); break;
			case 'F': drawChar5x7ASCII_toDisplay_Array(F_5x7_ASCII, col, row, brightness); break;
			case 'G': drawChar5x7ASCII_toDisplay_Array(G_5x7_ASCII, col, row, brightness); break;
			case 'H': drawChar5x7ASCII_toDisplay_Array(H_5x7_ASCII, col, row, brightness); break;
			case 'I': drawChar5x7ASCII_toDisplay_Array(I_5x7_ASCII, col, row, brightness); break;
			case 'J': drawChar5x7ASCII_toDisplay_Array(J_5x7_ASCII, col, row, brightness); break;
			case 'K': drawChar5x7ASCII_toDisplay_Array(K_5x7_ASCII, col, row, brightness); break;
			case 'L': drawChar5x7ASCII_toDisplay_Array(L_5x7_ASCII, col, row, brightness); break;
			case 'M': drawChar5x7ASCII_toDisplay_Array(M_5x7_ASCII, col, row, brightness); break;
			case 'N': drawChar5x7ASCII_toDisplay_Array(N_5x7_ASCII, col, row, brightness); break;
			case 'O': drawChar5x7ASCII_toDisplay_Array(O_5x7_ASCII, col, row, brightness); break;
			case 'P': drawChar5x7ASCII_toDisplay_Array(P_5x7_ASCII, col, row, brightness); break;
			case 'Q': drawChar5x7ASCII_toDisplay_Array(Q_5x7_ASCII, col, row, brightness); break;
			case 'R': drawChar5x7ASCII_toDisplay_Array(R_5x7_ASCII, col, row, brightness); break;
			case 'S': drawChar5x7ASCII_toDisplay_Array(S_5x7_ASCII, col, row, brightness); break;
			case 'T': drawChar5x7ASCII_toDisplay_Array(T_5x7_ASCII, col, row, brightness); break;
			case 'U': drawChar5x7ASCII_toDisplay_Array(U_5x7_ASCII, col, row, brightness); break;
			case 'V': drawChar5x7ASCII_toDisplay_Array(V_5x7_ASCII, col, row, brightness); break;
			case 'W': drawChar5x7ASCII_toDisplay_Array(W_5x7_ASCII, col, row, brightness); break;
			case 'X': drawChar5x7ASCII_toDisplay_Array(X_5x7_ASCII, col, row, brightness); break;
			case 'Y': drawChar5x7ASCII_toDisplay_Array(Y_5x7_ASCII, col, row, brightness); break;
			case 'Z': drawChar5x7ASCII_toDisplay_Array(Z_5x7_ASCII, col, row, brightness); break;
			case '<': drawChar5x7ASCII_toDisplay_Array(left_5x7_ASCII, col, row, brightness); break;
			case '>': drawChar5x7ASCII_toDisplay_Array(right_5x7_ASCII, col, row, brightness); break;
			case '\"': drawChar5x7ASCII_toDisplay_Array(quote_5x7_ASCII, col, row, brightness); break;
			case ',': drawChar5x7ASCII_toDisplay_Array(zpt_5x7_ASCII, col, row, brightness); break;
		}	
	
}

void put_7x8ASCII_CharOnDisplay(unsigned char ch, unsigned char col, unsigned char row){

	Address = getAddress_7x8_ASCII(ch);
	
	address_BYTE1_MSB = (Address & 0x00FF0000)>>16;
	address_BYTE2     = (Address & 0x0000FF00)>>8;
	address_BYTE3_LSB = (Address & 0x000000FF);
	
	SFRPAGE = SPI0_PAGE;
	SPIEN   = 0;	
	SFRPAGE = 0x0F;	
	//MF_SO 	= 1;      //P3^3 = 1;	
	MF_CS2  = 0;	
	
	sendMF_byte_to_Display(0x0B);
	send24bitAddressToDisplay();
	sendMF_byte_to_Display(0x00);
	
	read_7x8_ASCII_CharFromDisplay();
	
	MF_CS2 = 1;
	SFRPAGE = SPI0_PAGE;
	SPIEN = 1;
	SFRPAGE = 0x0F;
	
	drawChar7x8ASCII_toDisplay(col, row);	
}

void put_8x16ASCII_CharOnDisplay(unsigned char ch, unsigned char col, unsigned char row, unsigned char brightness){
	/*
	Address = getAddress_8x16_ASCII(ch);
	
	address_BYTE1_MSB = (Address & 0x00FF0000)>>16;
	address_BYTE2     = (Address & 0x0000FF00)>>8;
	address_BYTE3_LSB = (Address & 0x000000FF);	
	
	SFRPAGE = SPI0_PAGE;
	SPIEN   = 0;	
	SFRPAGE = 0x0F;	
	//MF_SO 	= 1;      //P3^3 = 1;	
	MF_CS2  = 0;	
	
	sendMF_byte_to_Display(0x0B);
	send24bitAddressToDisplay();
	sendMF_byte_to_Display(0x00);
	
	read_8x16_ASCII_CharFromDisplay();
	
	MF_CS2 = 1;
	SFRPAGE = SPI0_PAGE;
	SPIEN = 1;
	SFRPAGE = 0x0F;
	
	drawChar8x16_toDisplay(col, row);	
	*/
	switch(ch){
		  case ' ': drawChar8x16_toDisplay_Array(space_8x16_ASCII, col, row, brightness); break;
			case 'A': drawChar8x16_toDisplay_Array(A_8x16_ASCII, col, row, brightness); break;
			case 'B': drawChar8x16_toDisplay_Array(B_8x16_ASCII, col, row, brightness); break;
			case 'C': drawChar8x16_toDisplay_Array(C_8x16_ASCII, col, row, brightness); break;
			case 'D': drawChar8x16_toDisplay_Array(D_8x16_ASCII, col, row, brightness); break;
			case 'E': drawChar8x16_toDisplay_Array(E_8x16_ASCII, col, row, brightness); break;
			case 'F': drawChar8x16_toDisplay_Array(F_8x16_ASCII, col, row, brightness); break;
			case 'G': drawChar8x16_toDisplay_Array(G_8x16_ASCII, col, row, brightness); break;
			case 'H': drawChar8x16_toDisplay_Array(H_8x16_ASCII, col, row, brightness); break;
			case 'I': drawChar8x16_toDisplay_Array(I_8x16_ASCII, col, row, brightness); break;
			case 'J': drawChar8x16_toDisplay_Array(J_8x16_ASCII, col, row, brightness); break;
			case 'K': drawChar8x16_toDisplay_Array(K_8x16_ASCII, col, row, brightness); break;
			case 'L': drawChar8x16_toDisplay_Array(L_8x16_ASCII, col, row, brightness); break;
			case 'M': drawChar8x16_toDisplay_Array(M_8x16_ASCII, col, row, brightness); break;
			case 'N': drawChar8x16_toDisplay_Array(N_8x16_ASCII, col, row, brightness); break;
			case 'O': drawChar8x16_toDisplay_Array(O_8x16_ASCII, col, row, brightness); break;
			case 'P': drawChar8x16_toDisplay_Array(P_8x16_ASCII, col, row, brightness); break;
			case 'Q': drawChar8x16_toDisplay_Array(Q_8x16_ASCII, col, row, brightness); break;
			case 'R': drawChar8x16_toDisplay_Array(R_8x16_ASCII, col, row, brightness); break;
			case 'S': drawChar8x16_toDisplay_Array(S_8x16_ASCII, col, row, brightness); break;
			case 'T': drawChar8x16_toDisplay_Array(T_8x16_ASCII, col, row, brightness); break;
			case 'U': drawChar8x16_toDisplay_Array(U_8x16_ASCII, col, row, brightness); break;
			case 'V': drawChar8x16_toDisplay_Array(V_8x16_ASCII, col, row, brightness); break;
			case 'W': drawChar8x16_toDisplay_Array(W_8x16_ASCII, col, row, brightness); break;
			case 'X': drawChar8x16_toDisplay_Array(X_8x16_ASCII, col, row, brightness); break;
			case 'Y': drawChar8x16_toDisplay_Array(Y_8x16_ASCII, col, row, brightness); break;
			case 'Z': drawChar8x16_toDisplay_Array(Z_8x16_ASCII, col, row, brightness); break;
			case 'a': drawChar8x16_toDisplay_Array(a__8x16_ASCII, col, row, brightness); break;
			case 'b': drawChar8x16_toDisplay_Array(b__8x16_ASCII, col, row, brightness); break;	
			case 'c': drawChar8x16_toDisplay_Array(c__8x16_ASCII, col, row, brightness); break;
			case 'd': drawChar8x16_toDisplay_Array(d__8x16_ASCII, col, row, brightness); break;	
			case 'e': drawChar8x16_toDisplay_Array(e__8x16_ASCII, col, row, brightness); break;	
			case 'f': drawChar8x16_toDisplay_Array(f__8x16_ASCII, col, row, brightness); break;	
			case 'g': drawChar8x16_toDisplay_Array(g__8x16_ASCII, col, row, brightness); break;
			case 'h': drawChar8x16_toDisplay_Array(h__8x16_ASCII, col, row, brightness); break;	
			case 'i': drawChar8x16_toDisplay_Array(i__8x16_ASCII, col, row, brightness); break;	
			case 'j': drawChar8x16_toDisplay_Array(j__8x16_ASCII, col, row, brightness); break;
			case 'k': drawChar8x16_toDisplay_Array(k__8x16_ASCII, col, row, brightness); break;
			case 'l': drawChar8x16_toDisplay_Array(l__8x16_ASCII, col, row, brightness); break;				
			case 'm': drawChar8x16_toDisplay_Array(m__8x16_ASCII, col, row, brightness); break;
			case 'n': drawChar8x16_toDisplay_Array(n__8x16_ASCII, col, row, brightness); break;
			case 'o': drawChar8x16_toDisplay_Array(o__8x16_ASCII, col, row, brightness); break;
			case 'p': drawChar8x16_toDisplay_Array(p__8x16_ASCII, col, row, brightness); break;	
			case 'q': drawChar8x16_toDisplay_Array(q__8x16_ASCII, col, row, brightness); break;	
			case 'r': drawChar8x16_toDisplay_Array(r__8x16_ASCII, col, row, brightness); break;
			case 's': drawChar8x16_toDisplay_Array(s__8x16_ASCII, col, row, brightness); break;	
			case 't': drawChar8x16_toDisplay_Array(t__8x16_ASCII, col, row, brightness); break;				
			case 'u': drawChar8x16_toDisplay_Array(u__8x16_ASCII, col, row, brightness); break;
			case 'v': drawChar8x16_toDisplay_Array(v__8x16_ASCII, col, row, brightness); break;
			case 'w': drawChar8x16_toDisplay_Array(w__8x16_ASCII, col, row, brightness); break;
			case 'x': drawChar8x16_toDisplay_Array(x__8x16_ASCII, col, row, brightness); break;	
			case 'y': drawChar8x16_toDisplay_Array(y__8x16_ASCII, col, row, brightness); break;	
			case 'z': drawChar8x16_toDisplay_Array(z__8x16_ASCII, col, row, brightness); break;				
			case '0': drawChar8x16_toDisplay_Array(number_0_8x16_ASCII, col, row, brightness); break;			
			case '1': drawChar8x16_toDisplay_Array(number_1_8x16_ASCII, col, row, brightness); break;			
			case '2': drawChar8x16_toDisplay_Array(number_2_8x16_ASCII, col, row, brightness); break;
			case '3': drawChar8x16_toDisplay_Array(number_3_8x16_ASCII, col, row, brightness); break;
			case '4': drawChar8x16_toDisplay_Array(number_4_8x16_ASCII, col, row, brightness); break;
			case '5': drawChar8x16_toDisplay_Array(number_5_8x16_ASCII, col, row, brightness); break;
			case '6': drawChar8x16_toDisplay_Array(number_6_8x16_ASCII, col, row, brightness); break;
			case '7': drawChar8x16_toDisplay_Array(number_7_8x16_ASCII, col, row, brightness); break;
			case '8': drawChar8x16_toDisplay_Array(number_8_8x16_ASCII, col, row, brightness); break;
			case '9': drawChar8x16_toDisplay_Array(number_9_8x16_ASCII, col, row, brightness); break;
			case 0x00: drawChar8x16_toDisplay_Array(number_0_8x16_ASCII, col, row, brightness); break;
			case 0x01: drawChar8x16_toDisplay_Array(number_1_8x16_ASCII, col, row, brightness); break;
			case 0x02: drawChar8x16_toDisplay_Array(number_2_8x16_ASCII, col, row, brightness); break;
			case 0x03: drawChar8x16_toDisplay_Array(number_3_8x16_ASCII, col, row, brightness); break;
			case 0x04: drawChar8x16_toDisplay_Array(number_4_8x16_ASCII, col, row, brightness); break;
			case 0x05: drawChar8x16_toDisplay_Array(number_5_8x16_ASCII, col, row, brightness); break;
			case 0x06: drawChar8x16_toDisplay_Array(number_6_8x16_ASCII, col, row, brightness); break;
			case 0x07: drawChar8x16_toDisplay_Array(number_7_8x16_ASCII, col, row, brightness); break;
			case 0x08: drawChar8x16_toDisplay_Array(number_8_8x16_ASCII, col, row, brightness); break;
			case 0x09: drawChar8x16_toDisplay_Array(number_9_8x16_ASCII, col, row, brightness); break;
			case ',': drawChar8x16_toDisplay_Array(zpt_8x16_ASCII, col, row, brightness); break;
			case '.': drawChar8x16_toDisplay_Array(tck_8x16_ASCII, col, row, brightness); break;
			case ':': drawChar8x16_toDisplay_Array(double_tck_8x16_ASCII, col, row, brightness); break;
			case '-': drawChar8x16_toDisplay_Array(minus_8x16_ASCII, col, row, brightness); break;
			case '=': drawChar8x16_toDisplay_Array(ravno_8x16_ASCII, col, row, brightness); break;
			case '+': drawChar8x16_toDisplay_Array(plus_8x16_ASCII, col, row, brightness); break;
			case '(': drawChar8x16_toDisplay_Array(skobka1_8x16_ASCII, col, row, brightness); break;
			case ')': drawChar8x16_toDisplay_Array(skobka2_8x16_ASCII, col, row, brightness); break;
			case '/': drawChar8x16_toDisplay_Array(kosaya_right_8x16_ASCII, col, row, brightness); break;
			case '%': drawChar8x16_toDisplay_Array(procent_8x16_ASCII, col, row, brightness); break;
		}	
}
void put_WidthAdjusted_Arial_ASCII_CharOnDisplay(unsigned char ch, unsigned char col, unsigned char row){
  /*
	Address = getAddress_WidthAdjusted_Arial_ASCII(ch);
	
	address_BYTE1_MSB = (Address & 0x00FF0000)>>16;
	address_BYTE2     = (Address & 0x0000FF00)>>8;
	address_BYTE3_LSB = (Address & 0x000000FF);	
	
	SFRPAGE = SPI0_PAGE;
	SPIEN   = 0;	
	SFRPAGE = 0x0F;	
	MF_CS2  = 0;	
	
	sendMF_byte_to_Display(0x0B);
	send24bitAddressToDisplay();
	sendMF_byte_to_Display(0x00);
	
	read_16x16_Font_CharFromDisplay();
	
	MF_CS2 = 1;
	SFRPAGE = SPI0_PAGE;
	SPIEN = 1;
	SFRPAGE = 0x0F;
	
	drawChar16x16_toDisplay(col, row);	
	*/
		switch(ch){
		  case ' ': drawChar16x16_toDisplay_Array(space_WidthAdjusted_Arial_ASCII, col, row); break;
			case 'A': drawChar16x16_toDisplay_Array(A_WidthAdjusted_Arial_ASCII, col, row); break;
			case 'B': drawChar16x16_toDisplay_Array(B_WidthAdjusted_Arial_ASCII, col, row); break;
			case 'C': drawChar16x16_toDisplay_Array(C_WidthAdjusted_Arial_ASCII, col, row); break;
			case 'D': drawChar16x16_toDisplay_Array(D_WidthAdjusted_Arial_ASCII, col, row); break;
			case 'E': drawChar16x16_toDisplay_Array(E_WidthAdjusted_Arial_ASCII, col, row); break;
			case 'F': drawChar16x16_toDisplay_Array(F_WidthAdjusted_Arial_ASCII, col, row); break;
			case 'G': drawChar16x16_toDisplay_Array(G_WidthAdjusted_Arial_ASCII, col, row); break;
			case 'H': drawChar16x16_toDisplay_Array(H_WidthAdjusted_Arial_ASCII, col, row); break;
			case 'I': drawChar16x16_toDisplay_Array(I_WidthAdjusted_Arial_ASCII, col, row); break;
			case 'J': drawChar16x16_toDisplay_Array(J_WidthAdjusted_Arial_ASCII, col, row); break;
			case 'K': drawChar16x16_toDisplay_Array(K_WidthAdjusted_Arial_ASCII, col, row); break;
			case 'L': drawChar16x16_toDisplay_Array(L_WidthAdjusted_Arial_ASCII, col, row); break;
			case 'M': drawChar16x16_toDisplay_Array(M_WidthAdjusted_Arial_ASCII, col, row); break;
			case 'N': drawChar16x16_toDisplay_Array(N_WidthAdjusted_Arial_ASCII, col, row); break;
			case 'O': drawChar16x16_toDisplay_Array(O_WidthAdjusted_Arial_ASCII, col, row); break;
			case 'P': drawChar16x16_toDisplay_Array(P_WidthAdjusted_Arial_ASCII, col, row); break;
			case 'Q': drawChar16x16_toDisplay_Array(Q_WidthAdjusted_Arial_ASCII, col, row); break;
			case 'R': drawChar16x16_toDisplay_Array(R_WidthAdjusted_Arial_ASCII, col, row); break;
			case 'S': drawChar16x16_toDisplay_Array(S_WidthAdjusted_Arial_ASCII, col, row); break;
			case 'T': drawChar16x16_toDisplay_Array(T_WidthAdjusted_Arial_ASCII, col, row); break;
			case 'U': drawChar16x16_toDisplay_Array(U_WidthAdjusted_Arial_ASCII, col, row); break;
			case 'V': drawChar16x16_toDisplay_Array(V_WidthAdjusted_Arial_ASCII, col, row); break;
			case 'W': drawChar16x16_toDisplay_Array(W_WidthAdjusted_Arial_ASCII, col, row); break;
			case 'X': drawChar16x16_toDisplay_Array(X_WidthAdjusted_Arial_ASCII, col, row); break;
			case 'Y': drawChar16x16_toDisplay_Array(Y_WidthAdjusted_Arial_ASCII, col, row); break;
			case 'Z': drawChar16x16_toDisplay_Array(Z_WidthAdjusted_Arial_ASCII, col, row); break;	
			case 'a': drawChar16x16_toDisplay_Array(a__WidthAdjusted_Arial_ASCII, col, row); break;
			case 'b': drawChar16x16_toDisplay_Array(b__WidthAdjusted_Arial_ASCII, col, row); break;
			case 'c': drawChar16x16_toDisplay_Array(c__WidthAdjusted_Arial_ASCII, col, row); break;
			case 'd': drawChar16x16_toDisplay_Array(d__WidthAdjusted_Arial_ASCII, col, row); break;
			case 'e': drawChar16x16_toDisplay_Array(e__WidthAdjusted_Arial_ASCII, col, row); break;
			case 'f': drawChar16x16_toDisplay_Array(f__WidthAdjusted_Arial_ASCII, col, row); break;
			case 'g': drawChar16x16_toDisplay_Array(g__WidthAdjusted_Arial_ASCII, col, row); break;
			case 'h': drawChar16x16_toDisplay_Array(h__WidthAdjusted_Arial_ASCII, col, row); break;
			case 'i': drawChar16x16_toDisplay_Array(i__WidthAdjusted_Arial_ASCII, col, row); break;
			case 'j': drawChar16x16_toDisplay_Array(j__WidthAdjusted_Arial_ASCII, col, row); break;
			case 'k': drawChar16x16_toDisplay_Array(k__WidthAdjusted_Arial_ASCII, col, row); break;
			case 'l': drawChar16x16_toDisplay_Array(l__WidthAdjusted_Arial_ASCII, col, row); break;
			case 'm': drawChar16x16_toDisplay_Array(m__WidthAdjusted_Arial_ASCII, col, row); break;
			case 'n': drawChar16x16_toDisplay_Array(n__WidthAdjusted_Arial_ASCII, col, row); break;
			case 'o': drawChar16x16_toDisplay_Array(o__WidthAdjusted_Arial_ASCII, col, row); break;
			case 'p': drawChar16x16_toDisplay_Array(p__WidthAdjusted_Arial_ASCII, col, row); break;
			case 'q': drawChar16x16_toDisplay_Array(q__WidthAdjusted_Arial_ASCII, col, row); break;
			case 'r': drawChar16x16_toDisplay_Array(r__WidthAdjusted_Arial_ASCII, col, row); break;
			case 's': drawChar16x16_toDisplay_Array(s__WidthAdjusted_Arial_ASCII, col, row); break;
			case 't': drawChar16x16_toDisplay_Array(t__WidthAdjusted_Arial_ASCII, col, row); break;
			case 'u': drawChar16x16_toDisplay_Array(u__WidthAdjusted_Arial_ASCII, col, row); break;
			case 'v': drawChar16x16_toDisplay_Array(v__WidthAdjusted_Arial_ASCII, col, row); break;
			case 'w': drawChar16x16_toDisplay_Array(w__WidthAdjusted_Arial_ASCII, col, row); break;
			case 'x': drawChar16x16_toDisplay_Array(x__WidthAdjusted_Arial_ASCII, col, row); break;
			case 'y': drawChar16x16_toDisplay_Array(y__WidthAdjusted_Arial_ASCII, col, row); break;
			case 'z': drawChar16x16_toDisplay_Array(z__WidthAdjusted_Arial_ASCII, col, row); break;
			case '0': drawChar16x16_toDisplay_Array(number_0_WidthAdjusted_Arial_ASCII, col, row); break;
			case '1': drawChar16x16_toDisplay_Array(number_1_WidthAdjusted_Arial_ASCII, col, row); break;
			case '2': drawChar16x16_toDisplay_Array(number_2_WidthAdjusted_Arial_ASCII, col, row); break;
			case '3': drawChar16x16_toDisplay_Array(number_3_WidthAdjusted_Arial_ASCII, col, row); break;
			case '4': drawChar16x16_toDisplay_Array(number_4_WidthAdjusted_Arial_ASCII, col, row); break;
			case '5': drawChar16x16_toDisplay_Array(number_5_WidthAdjusted_Arial_ASCII, col, row); break;
			case '6': drawChar16x16_toDisplay_Array(number_6_WidthAdjusted_Arial_ASCII, col, row); break;
			case '7': drawChar16x16_toDisplay_Array(number_7_WidthAdjusted_Arial_ASCII, col, row); break;
			case '8': drawChar16x16_toDisplay_Array(number_8_WidthAdjusted_Arial_ASCII, col, row); break;
			case '9': drawChar16x16_toDisplay_Array(number_9_WidthAdjusted_Arial_ASCII, col, row); break;
			case ',': drawChar16x16_toDisplay_Array(zpt_WidthAdjusted_Arial_ASCII, col, row); break;
			case '.': drawChar16x16_toDisplay_Array(tck_WidthAdjusted_Arial_ASCII, col, row); break;
		}	
}

/*void put_8x16Latin_CharOnDisplay(unsigned char ch, unsigned char col, unsigned char row){
	
	Address = getAddress_8x16_Latin(ch);
	
	address_BYTE1_MSB = (Address & 0x00FF0000)>>16;
	address_BYTE2     = (Address & 0x0000FF00)>>8;
	address_BYTE3_LSB = (Address & 0x000000FF);
	
	SFRPAGE = SPI0_PAGE;
	SPIEN   = 0;	
	SFRPAGE = 0x0F;	
	//MF_SO 	= 1;      //P3^3 = 1;	
	MF_CS2  = 0;	
	
	sendMF_byte_to_Display(0x0B);	
	send24bitAddressToDisplay();
	sendMF_byte_to_Display(0x00);	
	
	read_8x16_ASCII_CharFromDisplay('A');
	
	MF_CS2 = 1;
	SFRPAGE = SPI0_PAGE;
	SPIEN = 1;
	SFRPAGE = 0x0F;
	
	drawChar8x16_toDisplay(col, row);
}*/
/*void put_WidthAdjusted_Latin_CharOnDisplay(unsigned char ch, unsigned char col, unsigned char row){

	Address = getAddress_WidthAdjusted_Latin(ch);
	
	address_BYTE1_MSB = (Address & 0x00FF0000)>>16;
	address_BYTE2     = (Address & 0x0000FF00)>>8;
	address_BYTE3_LSB = (Address & 0x000000FF);	
	
	SFRPAGE = SPI0_PAGE;
	SPIEN   = 0;	
	SFRPAGE = 0x0F;	
	MF_CS2  = 0;	
	
	sendMF_byte_to_Display(0x0B);
	send24bitAddressToDisplay();
	sendMF_byte_to_Display(0x00);
	
	read_16x16_Font_CharFromDisplay(ch);
	
	MF_CS2 = 1;
	SFRPAGE = SPI0_PAGE;
	SPIEN = 1;
	SFRPAGE = 0x0F;
	
	drawChar16x16_toDisplay(col, row);	
}*/
/*void put_GB2312_CharOnDisplay(unsigned int ch, unsigned char col, unsigned char row){
	
	Address = getAddress_GB2312(ch);
	
	address_BYTE1_MSB = (Address & 0x00FF0000)>>16;
	address_BYTE2     = (Address & 0x0000FF00)>>8;
	address_BYTE3_LSB = (Address & 0x000000FF);
	
	SFRPAGE = SPI0_PAGE;
	SPIEN   = 0;	
	SFRPAGE = 0x0F;	
	//MF_SO 	= 1;      //P3^3 = 1;	
	MF_CS2  = 0;		
	
	sendMF_byte_to_Display(0x0B);	
	send24bitAddressToDisplay();
	sendMF_byte_to_Display(0x00);		
	read_GB2312_CharFromDisplay();
	
	MF_CS2 = 1;	
	SFRPAGE = SPI0_PAGE;
	SPIEN = 1;		
	SFRPAGE = 0x0F;	
	
	drawCharGB2312_toDisplay(col, row);
}*/

/*void put_5x10LCM_1_CharOnDisplay(unsigned int ch, unsigned char col, unsigned char row){
	
	Address = getAddress_5x10_LCM_1(ch);
	
	address_BYTE1_MSB = (Address & 0x00FF0000)>>16;
	address_BYTE2     = (Address & 0x0000FF00)>>8;
	address_BYTE3_LSB = (Address & 0x000000FF);
	
	SFRPAGE = SPI0_PAGE;
	SPIEN   = 0;	
	SFRPAGE = 0x0F;	
	//MF_SO 	= 1;      //P3^3 = 1;	
	MF_CS2  = 0;		
	
	sendMF_byte_to_Display(0x0B);	
	send24bitAddressToDisplay();
	sendMF_byte_to_Display(0x00);		
	read_7x8_ASCII_CharFromDisplay();
	
	MF_CS2 = 1;	
	SFRPAGE = SPI0_PAGE;
	SPIEN = 1;		
	SFRPAGE = 0x0F;	
	
	drawChar7x8ASCII_toDisplay(col, row);
}
*/
/*void put_5x10LCM_2_CharOnDisplay(unsigned int ch, unsigned char col, unsigned char row){
	
	Address = getAddress_5x10_LCM_2(ch);
	
	address_BYTE1_MSB = (Address & 0x00FF0000)>>16;
	address_BYTE2     = (Address & 0x0000FF00)>>8;
	address_BYTE3_LSB = (Address & 0x000000FF);
	
	SFRPAGE = SPI0_PAGE;
	SPIEN   = 0;	
	SFRPAGE = 0x0F;	
	//MF_SO 	= 1;      //P3^3 = 1;	
	MF_CS2  = 0;		
	
	sendMF_byte_to_Display(0x0B);	
	send24bitAddressToDisplay();
	sendMF_byte_to_Display(0x00);		
	read_7x8_ASCII_CharFromDisplay();
	
	MF_CS2 = 1;	
	SFRPAGE = SPI0_PAGE;
	SPIEN = 1;		
	SFRPAGE = 0x0F;	
	
	drawChar7x8ASCII_toDisplay(col, row);
}*/
/*void put_5x10LCM_3_CharOnDisplay(unsigned int ch, unsigned char col, unsigned char row){
	
	Address = getAddress_5x10_LCM_3(ch);
	
	address_BYTE1_MSB = (Address & 0x00FF0000)>>16;
	address_BYTE2     = (Address & 0x0000FF00)>>8;
	address_BYTE3_LSB = (Address & 0x000000FF);
	
	SFRPAGE = SPI0_PAGE;
	SPIEN   = 0;	
	SFRPAGE = 0x0F;	
	//MF_SO 	= 1;      //P3^3 = 1;	
	MF_CS2  = 0;		
	
	sendMF_byte_to_Display(0x0B);	
	send24bitAddressToDisplay();
	sendMF_byte_to_Display(0x00);		
	read_7x8_ASCII_CharFromDisplay();
	
	MF_CS2 = 1;	
	SFRPAGE = SPI0_PAGE;
	SPIEN = 1;		
	SFRPAGE = 0x0F;	
	
	drawChar7x8ASCII_toDisplay(col, row);
}*/
/*void put_5x10LCM_8_CharOnDisplay(unsigned int ch, unsigned char col, unsigned char row){
	
	Address = getAddress_5x10_LCM_8(ch);
	
	address_BYTE1_MSB = (Address & 0x00FF0000)>>16;
	address_BYTE2     = (Address & 0x0000FF00)>>8;
	address_BYTE3_LSB = (Address & 0x000000FF);
	
	SFRPAGE = SPI0_PAGE;
	SPIEN   = 0;	
	SFRPAGE = 0x0F;	
	//MF_SO 	= 1;      //P3^3 = 1;	
	MF_CS2  = 0;		
	
	sendMF_byte_to_Display(0x0B);	
	send24bitAddressToDisplay();
	sendMF_byte_to_Display(0x00);		
	read_7x8_ASCII_CharFromDisplay();
	
	MF_CS2 = 1;	
	SFRPAGE = SPI0_PAGE;
	SPIEN = 1;		
	SFRPAGE = 0x0F;	
	
	drawChar7x8ASCII_toDisplay(col, row);
}*/
/*void put_5x10LCM_11_CharOnDisplay(unsigned int ch, unsigned char col, unsigned char row){
	
	Address = getAddress_5x10_LCM_11(ch);
	
	address_BYTE1_MSB = (Address & 0x00FF0000)>>16;
	address_BYTE2     = (Address & 0x0000FF00)>>8;
	address_BYTE3_LSB = (Address & 0x000000FF);
	
	SFRPAGE = SPI0_PAGE;
	SPIEN   = 0;	
	SFRPAGE = 0x0F;	
	//MF_SO 	= 1;      //P3^3 = 1;	
	MF_CS2  = 0;		
	
	sendMF_byte_to_Display(0x0B);	
	send24bitAddressToDisplay();
	sendMF_byte_to_Display(0x00);		
	read_7x8_ASCII_CharFromDisplay();
	
	MF_CS2 = 1;	
	SFRPAGE = SPI0_PAGE;
	SPIEN = 1;		
	SFRPAGE = 0x0F;	
	
	drawChar7x8ASCII_toDisplay(col, row);
}*/
/*void put_5x10LCM_12_CharOnDisplay(unsigned int ch, unsigned char col, unsigned char row){
	
	Address = getAddress_5x10_LCM_12(ch);
	
	address_BYTE1_MSB = (Address & 0x00FF0000)>>16;
	address_BYTE2     = (Address & 0x0000FF00)>>8;
	address_BYTE3_LSB = (Address & 0x000000FF);
	
	SFRPAGE = SPI0_PAGE;
	SPIEN   = 0;	
	SFRPAGE = 0x0F;	
	//MF_SO 	= 1;      //P3^3 = 1;	
	MF_CS2  = 0;		
	
	sendMF_byte_to_Display(0x0B);	
	send24bitAddressToDisplay();
	sendMF_byte_to_Display(0x00);		
	read_7x8_ASCII_CharFromDisplay();
	
	MF_CS2 = 1;	
	SFRPAGE = SPI0_PAGE;
	SPIEN = 1;		
	SFRPAGE = 0x0F;	
	
	drawChar7x8ASCII_toDisplay(col, row);
}*/
/*void put_5x10LCM_13_CharOnDisplay(unsigned int ch, unsigned char col, unsigned char row){
	
	Address = getAddress_5x10_LCM_13(ch);
	
	address_BYTE1_MSB = (Address & 0x00FF0000)>>16;
	address_BYTE2     = (Address & 0x0000FF00)>>8;
	address_BYTE3_LSB = (Address & 0x000000FF);
	
	SFRPAGE = SPI0_PAGE;
	SPIEN   = 0;	
	SFRPAGE = 0x0F;	
	//MF_SO 	= 1;      //P3^3 = 1;	
	MF_CS2  = 0;		
	
	sendMF_byte_to_Display(0x0B);	
	send24bitAddressToDisplay();
	sendMF_byte_to_Display(0x00);		
	read_7x8_ASCII_CharFromDisplay();
	
	MF_CS2 = 1;	
	SFRPAGE = SPI0_PAGE;
	SPIEN = 1;		
	SFRPAGE = 0x0F;	
	
	drawChar7x8ASCII_toDisplay(col, row);
}*/
void draw4Pixels(unsigned char fData,unsigned char sData,unsigned char row, unsigned char col){	
	Set_Column_Address(col,col); //
	Set_Row_Address(row,row);
	writeRAMCommand();
  oled_Data(fData);
	oled_Data(sData);
}

void init_5x7_ASCII(unsigned char ch){
	Address = getAddress_5x7_ASCII(ch);	
	
	address_BYTE1_MSB = (Address & 0x00FF0000)>>16;
	address_BYTE2     = (Address & 0x0000FF00)>>8;
	address_BYTE3_LSB = (Address & 0x000000FF);
	
	SFRPAGE = SPI0_PAGE;
	SPIEN   = 0;	
	SFRPAGE = 0x0F;	
	//MF_SO 	= 1;      //P3^3 = 1;	
	MF_CS2  = 0;	
	
	sendMF_byte_to_Display(0x0B);
	send24bitAddressToDisplay();
	sendMF_byte_to_Display(0x00);
	
	//read_5x7_ASCII_CharFromDisplay(ch);	
	
	MF_CS2 = 1;
	SFRPAGE = SPI0_PAGE;
	SPIEN = 1;
	SFRPAGE = 0x0F;	
}

void init_8x16_ASCII(unsigned char ch){
	Address = getAddress_8x16_ASCII(ch);	
	
	address_BYTE1_MSB = (Address & 0x00FF0000)>>16;
	address_BYTE2     = (Address & 0x0000FF00)>>8;
	address_BYTE3_LSB = (Address & 0x000000FF);
	
	SFRPAGE = SPI0_PAGE;
	SPIEN   = 0;	
	SFRPAGE = 0x0F;	
	//MF_SO 	= 1;      //P3^3 = 1;	
	MF_CS2  = 0;	
	
	sendMF_byte_to_Display(0x0B);
	send24bitAddressToDisplay();
	sendMF_byte_to_Display(0x00);
	
	//read_8x16_ASCII_CharFromDisplay(ch);	
	
	MF_CS2 = 1;
	SFRPAGE = SPI0_PAGE;
	SPIEN = 1;
	SFRPAGE = 0x0F;	
}
void init_WidthAdjusted_Arial_ASCII(unsigned char ch){
	Address = getAddress_WidthAdjusted_Arial_ASCII(ch);
	
	address_BYTE1_MSB = (Address & 0x00FF0000)>>16;
	address_BYTE2     = (Address & 0x0000FF00)>>8;
	address_BYTE3_LSB = (Address & 0x000000FF);	
	
	SFRPAGE = SPI0_PAGE;
	SPIEN   = 0;	
	SFRPAGE = 0x0F;	
	MF_CS2  = 0;	
	
	sendMF_byte_to_Display(0x0B);
	send24bitAddressToDisplay();
	sendMF_byte_to_Display(0x00);
	
	//read_16x16_Font_CharFromDisplay(ch);
	
	MF_CS2 = 1;
	SFRPAGE = SPI0_PAGE;
	SPIEN = 1;
	SFRPAGE = 0x0F;	
}
void init_Alphabet_5x7_ASCII(){
	//init_5x7_ASCII('A');
	//init_5x7_ASCII('B');
	//init_5x7_ASCII('C');
	//init_5x7_ASCII('D');
	//init_5x7_ASCII('E');
	//init_5x7_ASCII('F');
	//init_5x7_ASCII('G');
	//init_5x7_ASCII('H');
	//init_5x7_ASCII('I');
	//init_5x7_ASCII('J');
	//init_5x7_ASCII('K');
	//init_5x7_ASCII('L');
	//init_5x7_ASCII('M');
	//init_5x7_ASCII('N');
	//init_5x7_ASCII('O');
	//init_5x7_ASCII('P');
	//init_5x7_ASCII('Q');
	//init_5x7_ASCII('R');
	//init_5x7_ASCII('S');
	//init_5x7_ASCII('T');
	//init_5x7_ASCII('U');
	//init_5x7_ASCII('V');
	//init_5x7_ASCII('W');
	//init_5x7_ASCII('X');
	//init_5x7_ASCII('Y');
	//init_5x7_ASCII('Z');
	//init_5x7_ASCII('<');
	//init_5x7_ASCII('>');
	//init_5x7_ASCII('\"');
	//init_5x7_ASCII(',');
}
void init_Alphabet_8x16_ASCII(){
	//init_8x16_ASCII('A');
	//init_8x16_ASCII('B');
	//init_8x16_ASCII('C');
	//init_8x16_ASCII('D');
	//init_8x16_ASCII('E');
	//init_8x16_ASCII('F');
	//init_8x16_ASCII('G');
	//init_8x16_ASCII('H');
	//init_8x16_ASCII('I');
	//init_8x16_ASCII('J');
	//init_8x16_ASCII('K');
	//init_8x16_ASCII('L');
	//init_8x16_ASCII('M');
	//init_8x16_ASCII('N');
	//init_8x16_ASCII('O');
	//init_8x16_ASCII('P');
	//init_8x16_ASCII('Q');
	//init_8x16_ASCII('R');
	//init_8x16_ASCII('S');
	//init_8x16_ASCII('T');
	//init_8x16_ASCII('U');
	//init_8x16_ASCII('V');
	//init_8x16_ASCII('W');
	//init_8x16_ASCII('X');
	//init_8x16_ASCII('Y');
	//init_8x16_ASCII('Z');
	//init_8x16_ASCII('a');
	//init_8x16_ASCII('b');
	//init_8x16_ASCII('c');
	//init_8x16_ASCII('d');
	//init_8x16_ASCII('e');
	//init_8x16_ASCII('f');
	//init_8x16_ASCII('g');
	//init_8x16_ASCII('h');
	//init_8x16_ASCII('i');
	//init_8x16_ASCII('j');
	//init_8x16_ASCII('k');
	//init_8x16_ASCII('l');
	//init_8x16_ASCII('m');
	//init_8x16_ASCII('n');
	//init_8x16_ASCII('o');
	//init_8x16_ASCII('p');
	//init_8x16_ASCII('q');
	//init_8x16_ASCII('r');
	//init_8x16_ASCII('s');
	//init_8x16_ASCII('t');
	//init_8x16_ASCII('u');
	//init_8x16_ASCII('v');
	//init_8x16_ASCII('w');
	//init_8x16_ASCII('x');
	//init_8x16_ASCII('y');
	//init_8x16_ASCII('z');
	//init_8x16_ASCII('0');
	//init_8x16_ASCII('1');
	//init_8x16_ASCII('2');
	//init_8x16_ASCII('3');
  //init_8x16_ASCII('4');
	//init_8x16_ASCII('5');
	//init_8x16_ASCII('6');
	//init_8x16_ASCII('7');
	//init_8x16_ASCII('8');
	//init_8x16_ASCII('9');
	//init_8x16_ASCII(',');
	//init_8x16_ASCII('.');
	//init_8x16_ASCII(':');
	//init_8x16_ASCII('-');
	//init_8x16_ASCII('=');
	//init_8x16_ASCII('+');
	//init_8x16_ASCII('(');
	//init_8x16_ASCII(')');
	//init_8x16_ASCII('/');
	//init_8x16_ASCII('%');
}
void init_Alphabet_WidthAdjusted_Arial_ASCII(){
	//init_WidthAdjusted_Arial_ASCII('A');
	//init_WidthAdjusted_Arial_ASCII('B');
	//init_WidthAdjusted_Arial_ASCII('C');
	//init_WidthAdjusted_Arial_ASCII('D');
	//init_WidthAdjusted_Arial_ASCII('E');
	//init_WidthAdjusted_Arial_ASCII('F');
	//init_WidthAdjusted_Arial_ASCII('G');
	//init_WidthAdjusted_Arial_ASCII('H');
	//init_WidthAdjusted_Arial_ASCII('I');
	//init_WidthAdjusted_Arial_ASCII('J');
	//init_WidthAdjusted_Arial_ASCII('K');
	//init_WidthAdjusted_Arial_ASCII('L');
	//init_WidthAdjusted_Arial_ASCII('M');
	//init_WidthAdjusted_Arial_ASCII('N');
	//init_WidthAdjusted_Arial_ASCII('O');
	//init_WidthAdjusted_Arial_ASCII('P');
	//init_WidthAdjusted_Arial_ASCII('Q');
	//init_WidthAdjusted_Arial_ASCII('R');
	//init_WidthAdjusted_Arial_ASCII('S');
	//init_WidthAdjusted_Arial_ASCII('T');
	//init_WidthAdjusted_Arial_ASCII('U');
	//init_WidthAdjusted_Arial_ASCII('V');
	//init_WidthAdjusted_Arial_ASCII('W');
	//init_WidthAdjusted_Arial_ASCII('X');
	//init_WidthAdjusted_Arial_ASCII('Y');	
	//init_WidthAdjusted_Arial_ASCII('Z');
  //init_WidthAdjusted_Arial_ASCII('a');
	//init_WidthAdjusted_Arial_ASCII('b');
	//init_WidthAdjusted_Arial_ASCII('c');
	//init_WidthAdjusted_Arial_ASCII('d');
	//init_WidthAdjusted_Arial_ASCII('e');
	//init_WidthAdjusted_Arial_ASCII('f');
	//init_WidthAdjusted_Arial_ASCII('g');
	//init_WidthAdjusted_Arial_ASCII('h');
	//init_WidthAdjusted_Arial_ASCII('i');
	//init_WidthAdjusted_Arial_ASCII('j');
	//init_WidthAdjusted_Arial_ASCII('k');
	//init_WidthAdjusted_Arial_ASCII('l');
	//init_WidthAdjusted_Arial_ASCII('m');
	//init_WidthAdjusted_Arial_ASCII('n');
	//init_WidthAdjusted_Arial_ASCII('o');
	//init_WidthAdjusted_Arial_ASCII('p');
  //init_WidthAdjusted_Arial_ASCII('q');
	//init_WidthAdjusted_Arial_ASCII('r');
	//init_WidthAdjusted_Arial_ASCII('s');
	//init_WidthAdjusted_Arial_ASCII('t');
	//init_WidthAdjusted_Arial_ASCII('u');
	//init_WidthAdjusted_Arial_ASCII('v');
	//init_WidthAdjusted_Arial_ASCII('w');
	//init_WidthAdjusted_Arial_ASCII('x');
	//init_WidthAdjusted_Arial_ASCII('y');
	//init_WidthAdjusted_Arial_ASCII('z');
	//init_WidthAdjusted_Arial_ASCII('0');
	//init_WidthAdjusted_Arial_ASCII('1');
	//init_WidthAdjusted_Arial_ASCII('2');
	//init_WidthAdjusted_Arial_ASCII('3');
	//init_WidthAdjusted_Arial_ASCII('4');
	//init_WidthAdjusted_Arial_ASCII('5');
	//init_WidthAdjusted_Arial_ASCII('6');
	//init_WidthAdjusted_Arial_ASCII('7');
	//init_WidthAdjusted_Arial_ASCII('8');
	//init_WidthAdjusted_Arial_ASCII('9');
	//init_WidthAdjusted_Arial_ASCII(',');
	//init_WidthAdjusted_Arial_ASCII('.');	
}


