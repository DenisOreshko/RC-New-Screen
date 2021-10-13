#include <C8051F120.h>
#include <Timer0.h>
#include <Timer1.h>
#include <Timer2.h>
#include <Timer3.h>
#include <Timer4.h>
#include <UART0.h>

sbit MF_SO   = P3^3;
sbit T0      = P3^4;
sbit T1      = P3^5;

void WDT_Disabled(){
	EA = 0;//Disable All interupt //SFRPAGE = All pages;
	WDTCN = 0xDE;                 //SFRPAGE = All pages;
	WDTCN = 0xAD;                 //SFRPAGE = All pages;
	EA = 1;//Enable All interupt  //SFRPAGE = All pages;
}
void Ports_Init(){
	SFRPAGE = 0x0F;	
	P0MDOUT = 0xF5;   //Push-pull out UART0_TX0(P0_0) SPI_SCK(P0_2) SPI_MOSI(P0_4)
	P1MDOUT = 0xFF;	  //D/C, /CS, /RES  and diod P1.6
	P3MDOUT = 0xF7;	  /*P3^4, P3^5 sound T0, T1*/
	P4MDOUT = 0xFF;   /*D0-D7*/  //SFRPAGE = 0x0F;
	P5MDOUT = 0xFF;	  /*A8-A15*/ //SFRPAGE = 0x0F;
	P6MDOUT = 0xF0;	  /*P0-P3 inputs*/ /*P4-P7 oututs*/ //SFRPAGE = 0x0F;
	P6 = 0xFF;        /*P0-P3 inputs*/                  //SFRPAGE = 0x0F;
	XBR0    = 0x06;		//allow  UART0, SPI
	XBR1    = 0x80;   //SYSCLK P1.7 enable
	XBR2    = 0x40;
	MF_SO 	= 1;      //P3^3 = 1;	
	T0 = 1;
	T1 = 1;
}
void SYSCLK_Init(){
  SFRPAGE = 0x0F;	
	OSCICN = 0xC3;          // Select internal generator. Enabled frequency chek SYSCLK	
}
void SPI_Init(){
	SFRPAGE = SPI0_PAGE;	
	SPI0CFG = 0x40;
	SPI0CKR = 0x00 ; //Max Freq SPI
	SPI0CN  = 0x01;	//SPI enable		
}

void initCPU(){
	WDT_Disabled();	
	Ports_Init();	
	SYSCLK_Init();
	SPI_Init();
	UART0_Init();	
	Timer0_Init();
	Timer1_Init();
	Timer2_Init();
	Timer3_Init();
  Timer4_Init();	
}