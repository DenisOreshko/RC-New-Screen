#ifndef _Menu_h_
#define _Menu_h_
extern int INDEX_MENU;
void updateMenu();
void write_5x7ASCII_String(const unsigned char* arr, int arr_size, unsigned char col, unsigned char row, unsigned char brightness);
void write_8x16ASCII_String(const unsigned char* arr, int arr_size, unsigned char col, unsigned char row, unsigned char brightness);
#endif //_Menu_h_