#include <C8051F120.h>
#include <Timer0.h>
#include <Timer3.h>
#include <Delays.h>
#include <SFRPage.h>
                             /*   C   		C#       D      D#      E       F       F#      G(405 Hz)G#      A       A#      B  */
xdata unsigned int nota[] =  {0 ,0xFF0B ,0xFF18 ,0xFF25 ,0xFF32 ,0xFF3D ,0xFF48 ,0xFF53 ,0xFF5C ,0xFF65 ,0xFF6B ,0xFF76 ,0xFF7E      /*3oct*/
                                ,0xFE16 ,0xFE31 ,0xFE4B ,0xFE64 ,0xFE7B ,0xFE91 ,0xFEA5 ,0xFEB9 ,0xFECB ,0xFEDD ,0xFEED ,0xFEFC      /*2oct*/
						                                                            ,0xB892 ,0xFD4A ,0xFD87 ,0xFD97 ,0xFDBA ,0xFDDA ,0xF98C};    /*1oct*/
						               /*1/64 1/32  1/16  1/8    1/4    1/2     1  */
xdata unsigned int delay[] = {14,  32,   64,  128,   256,   512,   1024, 340};
	                         /*  0    1     2    3      4      5      6      7 */

xdata unsigned char startMelody[] = {128,253,143,255};
xdata unsigned char buttonPressSound[] = {59,255};
xdata unsigned char buttonStopSound[] = {16,26,16,26,16,128,185,255};
	
xdata unsigned char gamma_1[] = {128,217,218,219,220,221,222,223,205,206,207,208,209,210,211,212,213,214,215,216,193,194,195,196,197,198,199,200,201,202,203,204,64,255};
	
xdata unsigned char gamma_1_32[] = {128,57,58,59,60,61,62,63,45,46,47,48,49,50,51,52,53,54,55,56,33,34,35,36,37,38,39,40,41,42,43,44,64,255};

/*

B  1 3oct = 204  B  1/2 3oct = 172  B  1/4 3oct = 140		B  1/8 3oct = 108		B  1/16 3oct = 76		B  1/32 3oct = 44			
A# 1 3oct = 203  A# 1/2 3oct = 171	A# 1/4 3oct = 139		A# 1/8 3oct = 107		A# 1/16 3oct = 75		A# 1/32 3oct = 43			
A  1 3oct = 202  A  1/2 3oct = 170	A  1/4 3oct = 138		A  1/8 3oct = 106		A  1/16 3oct = 74		A  1/32 3oct = 42			
G# 1 3oct = 201	 G# 1/2 3oct = 169	G# 1/4 3oct = 137		G# 1/8 3oct = 105		G# 1/16 3oct = 73		G# 1/32 3oct = 41			
G  1 3oct = 200	 G  1/2 3oct = 168	G  1/4 3oct = 136		G  1/8 3oct = 104		G  1/16 3oct = 72		G  1/32 3oct = 40			
F# 1 3oct = 199	 F# 1/2 3oct = 167	F# 1/4 3oct = 135		F# 1/8 3oct = 103		F# 1/16 3oct = 71		F# 1/32 3oct = 39		
F  1 3oct = 198	 F  1/2 3oct = 166	F  1/4 3oct = 134		F  1/8 3oct = 102		F  1/16 3oct = 70 	F  1/32 3oct = 38			
E  1 3oct = 197	 E  1/2 3oct = 165	E  1/4 3oct = 133		E  1/8 3oct = 101		E  1/16 3oct = 69		E  1/32 3oct = 37			
D# 1 3oct = 196	 D# 1/2 3oct = 164	D# 1/4 3oct = 132		D# 1/8 3oct = 100		D# 1/16 3oct = 68		D# 1/32 3oct = 36			
D  1 3oct = 195	 D  1/2 3oct = 163	D  1/4 3oct = 131		D  1/8 3oct = 99		D  1/16 3oct = 67		D  1/32 3oct = 35			
C# 1 3oct = 194	 C# 1/2 3oct = 162	C# 1/4 3oct = 130		C# 1/8 3oct = 98		C# 1/16 3oct = 66		C# 1/32 3oct = 34			
C  1 3oct = 193	 C  1/2 3oct = 161	C  1/4 3oct = 129		C  1/8 3oct = 97		C  1/16 3oct = 65		C  1/32 3oct = 33		

B  1 2oct = 216  B  1/2 2oct = 184  B  1/4 2oct = 152		B  1/8 2oct = 120		B  1/16 2oct = 88		B  1/32 2oct = 56			
A# 1 2oct = 215  A# 1/2 2oct = 183	A# 1/4 2oct = 151		A# 1/8 2oct = 119		A# 1/16 2oct = 87		A# 1/32 2oct = 55			
A  1 2oct = 214  A  1/2 2oct = 182	A  1/4 2oct = 150		A  1/8 2oct = 118		A  1/16 2oct = 86		A  1/32 2oct = 54			
G# 1 2oct = 213	 G# 1/2 2oct = 181	G# 1/4 2oct = 149		G# 1/8 2oct = 117		G# 1/16 2oct = 85		G# 1/32 2oct = 53			
G  1 2oct = 212	 G  1/2 2oct = 180	G  1/4 2oct = 148		G  1/8 2oct = 116		G  1/16 2oct = 84		G  1/32 2oct = 52			
F# 1 2oct = 211	 F# 1/2 2oct = 179	F# 1/4 2oct = 147		F# 1/8 2oct = 115		F# 1/16 2oct = 83		F# 1/32 2oct = 51		
F  1 2oct = 210	 F  1/2 2oct = 178	F  1/4 2oct = 146		F  1/8 2oct = 114		F  1/16 2oct = 82 	F  1/32 2oct = 50			
E  1 2oct = 209	 E  1/2 2oct = 177	E  1/4 2oct = 145		E  1/8 2oct = 113		E  1/16 2oct = 81		E  1/32 2oct = 49			
D# 1 2oct = 208	 D# 1/2 2oct = 176	D# 1/4 2oct = 144		D# 1/8 2oct = 112		D# 1/16 2oct = 80		D# 1/32 2oct = 48			D# 1/64 2oct = 16
D  1 2oct = 207	 D  1/2 2oct = 175	D  1/4 2oct = 143		D  1/8 2oct = 111		D  1/16 2oct = 79		D  1/32 2oct = 47			D  1/64 2oct = 15
C# 1 2oct = 206	 C# 1/2 2oct = 174	C# 1/4 2oct = 142		C# 1/8 2oct = 110		C# 1/16 2oct = 78		C# 1/32 2oct = 46			C# 1/64 2oct = 14
C  1 2oct = 205	 C  1/2 2oct = 173	C  1/4 2oct = 141		C  1/8 2oct = 109		C  1/16 2oct = 77		C  1/32 2oct = 45			C  1/64 2oct = 13			 	
	
B  1 1oct = 223	 B  1/2 1oct = 191	B  1/4 1oct = 159		B  1/8 1oct = 127		B  1/16 1oct = 95		B  1/32 1oct = 63                            <---155Hz									
A# 1 1oct = 222	 A# 1/2 1oct = 190	A# 1/4 1oct = 158		A# 1/8 1oct = 126		A# 1/16 1oct = 94		A# 1/32 1oct = 62		
A  1 1oct = 221	 A  1/2 1oct = 189	A  1/4 1oct = 157		A  1/8 1oct = 125		A  1/16 1oct = 93		A  1/32 1oct = 61		
G# 1 1oct = 220	 G# 1/2 1oct = 188	G# 1/4 1oct = 156		G# 1/8 1oct = 124		G# 1/16 1oct = 92		G# 1/32 1oct = 60		
G  1 1oct = 219	 G  1/2 1oct = 187	G  1/4 1oct = 155		G  1/8 1oct = 123		G  1/16 1oct = 91		G  1/32 1oct = 59	    G  1/64 1oct = 27      <---(405Hz)						
F# 1 1oct = 218	 F# 1/2 1oct = 186	F# 1/4 1oct = 154		F# 1/8 1oct = 122		F# 1/16 1oct = 90		F# 1/32 1oct = 58	    F# 1/64 1oct = 26

F  1 1oct = 217	 F  1/2 1oct = 185	F  1/4 1oct = 153		F  1/8 1oct = 121		F  1/16 1oct = 89		F  1/32 1oct = 57     F  1/64 1oct = 25		   <---(10 Hz)																																																																																																															
																																																																																																																																																																																																													
*/

xdata unsigned char Pirates_Of_The_Caribbean[] = {143,64,111,64,143,64,111,64,143,64,111,64,111,64,111,64,111,64,
																									143,64,111,64,143,64,111,64,143,64,111,64,111,64,111,64,111,64,
																									143,64,111,64,143,64,111,64,143,64,111,64,111,64,125,64,109,64,
																									143,64,143,64,111,64,113,64,146,64,146,64,114,64,116,64,145,64,
																									145,64,111,64,109,64,109,64,143,64,125,64,109,64,143,64,143,64,
																									111,64,113,64,146,64,146,64,114,64,116,64,145,64,145,64,111,64,
																									109,64,111,64,125,64,109,64,143,64,143,64,111,64,114,64,148,64,
																									148,64,116,64,118,64,151,64,151,64,118,64,116,64,118,64,143,128,
																									111,64,113,64,146,64,146,64,148,64,118,64,143,64,128,111,64,114,64,
																									145,64,145,64,114,64,111,64,177,64,118,64,97,64,131,64,131,64,99,64,
																									101,64,134,64,134,64,102,64,104,64,133,64,133,64,99,64,97,64,97,64,
																									131,64,118,64,97,64,131,64,131,64,99,64,101,64,134,64,134,64,102,64,
																									104,64,133,64,133,64,99,64,97,64,163,128,255};

xdata unsigned char Elochka[] = {150,64,115,64,115,64,150,64,115,64,115,64,118,64,116,64
                                ,115,64,113,64,175,64,152,64,99,64,108,64,150,64,115,64
						                    ,115,64,118,64,116,64,115,64,113,64,207,255};

xdata unsigned char Kuznechik[] = {150,64,145,64,150,64,145,64,150,64,149,64,181,64,149,64,	
																	 145,64,149,64,145,64,149,64,150,64,182,64,150,64,145,64,	
																	 150,64,145,64,150,64,149,64,181,64,149,64,145,64,149,64,	
																	 145,64,149,64,182,96,150,64,152,64,120,64,120,64,152,64,152,	
																	 64,129,64,97,64,97,64,129,64,129,64,129,64,152,64,150,64,	
																	 149,64,150,64,182,64,150,64,152,64,120,64,120,64,152,64,152,
																	 64,129,64,97,64,97,64,129,64,129,129,64,152,64,150,64,149,
																   64,182,128,255};

xdata unsigned char Happy_Birthday[] = {116,64,116,64,150,64,148,64,129,64,184,64,116,64,
																				116,64,150,64,148,64,131,64,161,64,116,64,116,64,
																				136,64,133,64,129,64,152,64,182,64,102,64,102,64,
																				133,64,129,64,131,64,193,128,255};

xdata unsigned char *melody[] = {&(startMelody),&(buttonPressSound),&(buttonStopSound),&(Kuznechik), &(Happy_Birthday),&(Pirates_Of_The_Caribbean)};
xdata unsigned char *sper;
xdata unsigned char kod_tona = 0;
xdata unsigned char kod_delay = 0;
xdata unsigned char byte = 0;
xdata unsigned char x = 0;

unsigned char pgm_read_byte(unsigned char *sper){
	return *sper;
}

int play(char melody[]){
	temp_SFR_page = SFRPAGE;	
	sper = melody;	
	while(byte != 255){		
    byte = pgm_read_byte(sper);			
		if (byte == 255){
			Timer0_Off();
			Timer3_Off();
			byte = 0x00;
			return 0;	
		}		
		kod_tona = (byte)&0x1F;
		kod_delay = ((byte)&0xE0)>>5;		
		byte_T0 = nota[kod_tona];	
		note_delay = delay[kod_delay];	
		Timer0_On();
		Timer3_On();
		SFRPAGE = temp_SFR_page;
	}	
}

void start_Sound(){
	temp_SFR_page = SFRPAGE;
	play(melody[0]);
	SFRPAGE = temp_SFR_page;
}
void button_Sound(){
	temp_SFR_page = SFRPAGE;
	play(melody[1]);
	SFRPAGE = temp_SFR_page;
}

void info_Message_Sound(){
	
}

void error_Message_Sound(){
	
}
void stop_Button_Sound(){	
		temp_SFR_page = SFRPAGE;  
		play(melody[2]);
		SFRPAGE = temp_SFR_page;	
}
