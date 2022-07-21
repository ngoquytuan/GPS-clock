#include "main.h"
#include <string.h>


uint8_t LEDintensity = 2;

#define decodeMode  0x09
#define intensity   0x0A
#define scanLimit   0x0B
#define shut_down   0x0C
#define disTest     0x0F

#ifdef SLAVE_CONSOLE
#endif
#ifdef SLAVE_WALL
#endif

#ifdef SLAVE_MATRIX
#endif

#ifdef SLAVE_WALL
	#define DAY1 1
	#define DAY2 2
	#define MON1 3
	#define MON2 4
	#define YEA1 5
	#define YEA2 6
	#define YEA3 7
	#define YEA4 8
#endif
#ifndef SLAVE_MATRIX
uint8_t blynk = 0;
unsigned char const LEDcode[]=
{
0x7e,0x30,0x6d,0x79,0x33,0x5b,0x5f,0x70,0x7f,0x7b, //0..9
0x77,0x1f,0x4e,0x3d,0x4f,0x47,0x67,0x3e,0xff //a..f,P,U,all_on
};

void MAX7219_SendByte (unsigned char dat)
{
unsigned char i;
	for (i=0;i<8;i++)
	{
		if((dat&0x80)==0x80) HAL_GPIO_WritePin(LED_DIN_GPIO_Port, LED_DIN_Pin, GPIO_PIN_SET);//GPIO_SetBits(GPIOA, MAX7219_DIN);
		else HAL_GPIO_WritePin(LED_DIN_GPIO_Port, LED_DIN_Pin, GPIO_PIN_RESET);//GPIO_ResetBits(GPIOA, MAX7219_DIN);

		HAL_GPIO_WritePin(LED_CLK_GPIO_Port, LED_CLK_Pin, GPIO_PIN_RESET);//GPIO_ResetBits(GPIOA, MAX7219_CLK);
		//HAL_Delay(1);
		HAL_GPIO_WritePin(LED_CLK_GPIO_Port, LED_CLK_Pin, GPIO_PIN_SET);//GPIO_SetBits(GPIOA, MAX7219_CLK);
		dat<<=1;
	}
}

void MAX7219_SendAddrDat (unsigned char addr,unsigned char dat)
{
HAL_GPIO_WritePin(LED_LOAD_GPIO_Port, LED_LOAD_Pin, GPIO_PIN_RESET);//GPIO_ResetBits(GPIOA, MAX7219_CS);
MAX7219_SendByte (addr);
MAX7219_SendByte (dat);
HAL_GPIO_WritePin(LED_LOAD_GPIO_Port, LED_LOAD_Pin, GPIO_PIN_SET);//GPIO_SetBits(GPIOA, MAX7219_CS);
}

void MAX7219_Init (void)
{
MAX7219_SendAddrDat (shut_down,0x01); //normal operation
#ifdef SLAVE_CONSOLE
	MAX7219_SendAddrDat (intensity,LEDintensity); //intensity
#endif
#ifdef SLAVE_WALL
	MAX7219_SendAddrDat (intensity,LEDintensity); //intensity
#endif
MAX7219_SendAddrDat (scanLimit,0x07); //all digits on
MAX7219_SendAddrDat (decodeMode,0x00); //decoding
MAX7219_SendAddrDat (disTest,0x00); //display test off
}

void MAX7219_Clear(void)
{
unsigned char i;
	for(i=8;i>0;i--)
	MAX7219_SendAddrDat(i,0x00);
}


void MAX7219_DisplayInt (long int val)
{
	long int a;
	a = val;
	MAX7219_SendAddrDat(0x08,LEDcode[1]);

	a=a%10000000;
	MAX7219_SendAddrDat(0x07,0x03);
	a=a%1000000;
	MAX7219_SendAddrDat(0x06,LEDcode[9]);
	a=a%100000;
	MAX7219_SendAddrDat(0x05,LEDcode[8]);
	a=a%10000;
	MAX7219_SendAddrDat(0x04,LEDcode[(a/1000)]);
	a=a%1000;
	MAX7219_SendAddrDat(0x03,LEDcode[(a/100)]);
	a=a%100;
	MAX7219_SendAddrDat(0x02,LEDcode[(a/10)]);
	a=a%10;
	MAX7219_SendAddrDat(0x01,LEDcode[a]);
}

#ifdef SLAVE_WALL
void MAX7219_SendByte2 (unsigned char dat)
{
unsigned char i;
	for (i=0;i<8;i++)
	{
		if((dat&0x80)==0x80) HAL_GPIO_WritePin(DIN2_GPIO_Port, DIN2_Pin, GPIO_PIN_SET);//GPIO_SetBits(GPIOA, MAX7219_DIN);
		else HAL_GPIO_WritePin(DIN2_GPIO_Port, DIN2_Pin, GPIO_PIN_RESET);//GPIO_ResetBits(GPIOA, MAX7219_DIN);

		HAL_GPIO_WritePin(CLK2_GPIO_Port, CLK2_Pin, GPIO_PIN_RESET);//GPIO_ResetBits(GPIOA, MAX7219_CLK);
		//HAL_Delay(1);
		HAL_GPIO_WritePin(CLK2_GPIO_Port, CLK2_Pin, GPIO_PIN_SET);//GPIO_SetBits(GPIOA, MAX7219_CLK);
		dat<<=1;
	}
}

void MAX7219_SendAddrDat2 (unsigned char addr,unsigned char dat)
{
		HAL_GPIO_WritePin(PC3_GPIO_Port, PC3_Pin, GPIO_PIN_RESET);//GPIO_ResetBits(GPIOA, MAX7219_CS);
		MAX7219_SendByte2 (addr);
		MAX7219_SendByte2 (dat);
		HAL_GPIO_WritePin(PC3_GPIO_Port, PC3_Pin, GPIO_PIN_SET);//GPIO_SetBits(GPIOA, MAX7219_CS);
}

void MAX7219_Init2 (void)
{
		MAX7219_SendAddrDat2 (shut_down,0x01); //normal operation
		MAX7219_SendAddrDat2 (intensity,LEDintensity); //intensity
		MAX7219_SendAddrDat2 (scanLimit,0x07); //all digits on
		MAX7219_SendAddrDat2 (decodeMode,0x00); //decoding
		MAX7219_SendAddrDat2 (disTest,0x00); //display test off
}
#endif


void display_init_check(void)
{
	MAX7219_Init();
	MAX7219_SendAddrDat(0x01,LEDcode[8]);
	MAX7219_SendAddrDat(0x02,LEDcode[8]);
	MAX7219_SendAddrDat(0x03,LEDcode[8]);
	MAX7219_SendAddrDat(0x04,LEDcode[8]);
	MAX7219_SendAddrDat(0x05,LEDcode[8]);
	MAX7219_SendAddrDat(0x06,LEDcode[8]);
	MAX7219_SendAddrDat(0x07,0xFF);//on all blink
	MAX7219_SendAddrDat(0x08,0x00);
	

	#ifdef SLAVE_WALL
	MAX7219_Init2();
	MAX7219_SendAddrDat2 (DAY1,LEDcode[8]);
	MAX7219_SendAddrDat2 (DAY2,LEDcode[8]);
	MAX7219_SendAddrDat2 (MON1,LEDcode[8]);
	MAX7219_SendAddrDat2 (MON2,LEDcode[8]);
	MAX7219_SendAddrDat2 (YEA1,LEDcode[8]);
	MAX7219_SendAddrDat2 (YEA2,LEDcode[8]);
	MAX7219_SendAddrDat2 (YEA3,LEDcode[8]);
	MAX7219_SendAddrDat2 (YEA4,LEDcode[8]);
	#endif
}

void console_display(void)
{
	uint8_t temp;
	//laythoigian();
	temp = hours/10;
	MAX7219_SendAddrDat(0x01,LEDcode[temp]);
	temp = hours%10;
	MAX7219_SendAddrDat(0x02,LEDcode[temp]);
	
	temp = minutes/10;
	MAX7219_SendAddrDat(0x03,LEDcode[temp]);
	temp = minutes%10;
	MAX7219_SendAddrDat(0x04,LEDcode[temp]);
	
	temp = seconds/10;
	MAX7219_SendAddrDat(0x05,LEDcode[temp]);
	temp = seconds%10;
	MAX7219_SendAddrDat(0x06,LEDcode[temp]);
	
}
void console_blink(void)
{
	
	
	if(blynk == 0)
	{
		blynk = 1;
		#ifdef SLAVE_CONSOLE
		if     (slave_clock.sync_status == LOCAL) MAX7219_SendAddrDat(0x07,0xFB);//all + local
		else if(slave_clock.sync_status == GPS)   MAX7219_SendAddrDat(0x07,0xFD);// all + gps
		else MAX7219_SendAddrDat(0x07,0xFF);// all + gps + local
		#endif
		#ifdef SLAVE_WALL
		if(slave_clock.sync_status == LOCAL) MAX7219_SendAddrDat(0x07,0xF6);//all + local
		else if(slave_clock.sync_status == GPS)   MAX7219_SendAddrDat(0x07,0xF8);//all + gps
		else MAX7219_SendAddrDat(0x07,0xFE);// all + gps + local
		#endif
	}
	else
	{
		blynk =0;
		#ifdef SLAVE_CONSOLE
		if     (slave_clock.sync_status == LOCAL) MAX7219_SendAddrDat(0x07,0x03);// off all + local
		else if(slave_clock.sync_status == GPS)   MAX7219_SendAddrDat(0x07,0x05);// off all + gps
		else MAX7219_SendAddrDat(0x07,0x07);// off all + gps + local
		#endif
		#ifdef SLAVE_WALL
		if(slave_clock.sync_status == LOCAL) MAX7219_SendAddrDat(0x07,0x06);//off all + local
		else if(slave_clock.sync_status == GPS)   MAX7219_SendAddrDat(0x07,0x08);//off all + gps
		else MAX7219_SendAddrDat(0x07,0x0E);// off all + gps + local
		#endif
	}
}
#ifdef SLAVE_WALL

void day_display(void)
{
	uint8_t temp;
	//laythoigian();
	temp = days/10;
	MAX7219_SendAddrDat2(DAY1,LEDcode[temp]);
	temp = days%10;
	MAX7219_SendAddrDat2(DAY2,LEDcode[temp]);
	
	temp = months/10;
	MAX7219_SendAddrDat2(MON1,LEDcode[temp]);
	temp = months%10;
	MAX7219_SendAddrDat2(MON2,LEDcode[temp]);
	
	MAX7219_SendAddrDat2 (YEA1,LEDcode[2]);
	MAX7219_SendAddrDat2 (YEA2,LEDcode[0]);
	temp = years/10;
	MAX7219_SendAddrDat2(YEA3,LEDcode[temp]);
	temp = years%10;
	MAX7219_SendAddrDat2(YEA4,LEDcode[temp]);
	
}
#endif
void chinhdosang(void)
{
	uint32_t i;
	LEDintensity++;
	if(LEDintensity > 15) LEDintensity =1;
	if(LEDintensity < 0) LEDintensity =1;
	MAX7219_SendAddrDat (intensity,LEDintensity); //intensity
	#ifdef SLAVE_WALL
	MAX7219_SendAddrDat2 (intensity,LEDintensity); //intensity
	#endif
	for (i=0;i<1000;i++);
}

//END of led 7 seg
#endif
/*----------------------------------------------------------------------------------------------------------------*/
#ifdef SLAVE_MATRIX


#define LOAD2_LOW    HAL_GPIO_WritePin(LOAD2_GPIO_Port, LOAD2_Pin, GPIO_PIN_RESET);
#define LOAD2_HIGH   HAL_GPIO_WritePin(LOAD2_GPIO_Port, LOAD2_Pin, GPIO_PIN_SET);

#define line1 0
#define line2 1
#define line3 2
#define line4 3
#define line5 4
#define line6 5
#define line7 6
#define line8 7

#define num1 0
#define num2 1
#define num3 2
#define num4 3
#define num5 4
#define num6 5
#define num7 6

#define mtrix1 4
#define mtrix2 3
#define mtrix3 2
#define mtrix4 1
#define mtrix5 0

#define mtrix6 6
#define mtrix7 5
#define mtrix8 4
#define mtrix9 3
#define mtrix10 2
#define mtrix11 1
#define mtrix12 0

#define BIT8 0x80
#define BIT7 0x40
#define BIT6 0x20
#define BIT5 0x10
#define BIT4 0x8
#define BIT3 0x4
#define BIT2 0x2
#define BIT1 0x1

unsigned char Buffer_7up[8][7];
unsigned char Buffer_5down[8][5];

// Standard font - variable spacing
unsigned char  _sysfont[] =
{

  'F', 2, 0, 0, 0, 255, 8,
  0,		// 										0 - 'Empty Cell'; 7
  5, 62, 91, 79, 91, 62,		// 1 - 'Sad Smiley';8
  5, 62, 107, 79, 107, 62,		// 2 - 'Happy Smiley';14
  5, 28, 62, 124, 62, 28,		// 3 - 'Heart';20
  5, 24, 60, 126, 60, 24,		// 4 - 'Diamond';26
  5, 28, 87, 125, 87, 28,		// 5 - 'Clubs';32
  5, 28, 94, 127, 94, 28,		// 6 - 'Spades';38
  4, 0, 24, 60, 24,		// 7 - 'Bullet Point';44
  5, 255, 231, 195, 231, 255,		// 8 - 'Rev Bullet Point';49
  4, 0, 24, 36, 24,		// 9 - 'Hollow Bullet Point';55
  5, 255, 231, 219, 231, 255,		// 10 - 'Rev Hollow BP';60
  5, 48, 72, 58, 6, 14,		// 11 - 'Male';66
  5, 38, 41, 121, 41, 38,		// 12 - 'Female';72
  5, 64, 127, 5, 5, 7,		// 13 - 'Music Note 1';78
  5, 64, 127, 5, 37, 63,		// 14 - 'Music Note 2';84
  5, 90, 60, 231, 60, 90,		// 15 - 'Snowflake';90
  5, 127, 62, 28, 28, 8,		// 16 - 'Right Pointer';96
  5, 8, 28, 28, 62, 127,		// 17 - 'Left Pointer';102
  5, 20, 34, 127, 34, 20,		// 18 - 'UpDown Arrows';108
  5, 255, 255, 255, 255, 255,		// 19 - 'Full Block';114
  5, 240, 240, 240, 240, 240,		// 20 - 'Half Block Bottom';120
  3, 255, 255, 255,		// 21 - 'Half Block LHS';126
  5, 0, 0, 0, 255, 255,		// 22 - 'Half Block RHS';130
  5, 15, 15, 15, 15, 15,		// 23 - 'Half Block Top';136
  5, 8, 4, 126, 4, 8,		// 24 - 'Up Arrow';142
  5, 16, 32, 126, 32, 16,		// 25 - 'Down Arrow';148
  5, 8, 8, 42, 28, 8,		// 26 - 'Right Arrow';154
  5, 8, 28, 42, 8, 8,		// 27 - 'Left Arrow';160
  5, 170, 0, 85, 0, 170,		// 28 - '30% shading';166
  5, 170, 85, 170, 85, 170,		// 29 - '50% shading';172
  5, 48, 56, 62, 56, 48,		// 30 - 'Up Pointer';178
  5, 6, 14, 62, 14, 6,		// 31 - 'Down Pointer';184
  2, 0, 0, 		// 32 - 'Space';190
  1, 95,		// 33 - '!';193
  3, 7, 0, 7,		// 34 - '""'/195
  5, 20, 127, 20, 127, 20,		// 35 - '#';199
  5, 68, 74, 255, 74, 50,		// 36 - '$';205
  5, 99, 19, 8, 100, 99,		// 37 - '%';211
  5, 54, 73, 73, 54, 72,		// 38 - '&';217
  1, 7,		// 39 - ''';223
  3, 62, 65, 65,		// 40 - '(';225
  3, 65, 65, 62,		// 41 - ')';229
  5, 8, 42, 28, 42, 8,		// 42 - '*';233
  5, 8, 8, 62, 8, 8,		// 43 - '+';239
  2, 96, 224,		// 44 - ',';245
  4, 8, 8, 8, 8,		// 45 - '-';248
  2, 96, 96,		// 46 - '.';253
  5, 96, 16, 8, 4, 3,		// 47 - '/';256
  5, 62, 81, 73, 69, 62,		// 					48 - '0'       262
  3, 0,4, 2, 127,0,		// 										49 - '1'                 268
//  3, 66, 127, 64,		// 49 - serifed '1'
  5, 113, 73, 73, 73, 70,							// 50 - '2'       272
  5, 65, 73, 73, 73, 54,							// 51 - '3'       278
  5, 15, 8, 8, 8, 127,								// 52 - '4'         284
  5, 79, 73, 73, 73, 49,							// 53 - '5'       290
  5, 62, 73, 73, 73, 48,							// 54 - '6'       296
  5, 3, 1, 1, 1, 127,									// 55 - '7'           302
//  5, 1, 1, 121, 5, 3,		// 55 - european style '7'
  5, 54, 73, 73, 73, 54,							// 56 - '8'       311
  5, 6, 73, 73, 73, 62,								// 57 - '9'         317 
  2, 108, 108,												// 58 - ':'                 318
  2, 108, 236,		// 59 - ';'
  3, 8, 20, 34,		// 60 - '<'
  4, 20, 20, 20, 20,		// 61 - '='
  3, 34, 20, 8,		// 62 - '>'
  5, 1, 89, 9, 9, 6,		// 63 - '?'
  5, 62, 65, 93, 89, 78,		// 64 - '@'
  5, 126, 9, 9, 9, 126,		// 65 - 'A'
  5, 127, 73, 73, 73, 54,		// 66 - 'B'
  5, 62, 65, 65, 65, 65,		// 67 - 'C'
  5, 127, 65, 65, 65, 62,		// 68 - 'D'
  5, 127, 73, 73, 73, 65,		// 69 - 'E'
  5, 127, 9, 9, 9, 1,		// 70 - 'F'
  5, 62, 65, 65, 73, 121,		// 71 - 'G'
  5, 127, 8, 8, 8, 127,		// 72 - 'H'
  3, 65, 127, 65,		// 73 - 'I'
  5, 48, 65, 65, 65, 63,		// 74 - 'J'
  5, 127, 8, 20, 34, 65,		// 75 - 'K'
  5, 127, 64, 64, 64, 64,		// 76 - 'L'
  5, 127, 2, 12, 2, 127,		// 77 - 'M'
  5, 127, 4, 8, 16, 127,		// 78 - 'N'
  5, 62, 65, 65, 65, 62,		// 79 - 'O'
  5, 127, 9, 9, 9, 6,		// 80 - 'P'
  5, 62, 65, 65, 97, 126,		// 81 - 'Q'
  5, 127, 9, 25, 41, 70,		// 82 - 'R'
  5, 70, 73, 73, 73, 49,		// 83 - 'S'
  5, 1, 1, 127, 1, 1,		// 84 - 'T'
  5, 63, 64, 64, 64, 63,		// 85 - 'U'
  5, 31, 32, 64, 32, 31,		// 86 - 'V'
  5, 63, 64, 56, 64, 63,		// 87 - 'W'
  5, 99, 20, 8, 20, 99,		// 88 - 'X'
  5, 3, 4, 120, 4, 3,		// 89 - 'Y'
  5, 97, 81, 73, 69, 67,		// 90 - 'Z'
  3, 127, 65, 65,		// 91 - '['
  5, 3, 4, 8, 16, 96,		// 92 - '\'
  3, 65, 65, 127,		// 93 - ']'
  5, 4, 2, 1, 2, 4,		// 94 - '^'
  4, 128, 128, 128, 128,		// 95 - '_'
  3, 1, 2, 4,		// 96 - '`'
  4, 56, 68, 68, 124,		// 97 - 'a'
  4, 127, 68, 68, 56,		// 98 - 'b'
  4, 56, 68, 68, 68,		// 99 - 'c'
  4, 56, 68, 68, 127,		// 100 - 'd'
  4, 56, 84, 84, 88,		// 101 - 'e'
  4, 4, 126, 5, 1,		// 102 - 'f'
  4, 24, 164, 164, 124,		// 103 - 'g'
  4, 127, 4, 4, 120,		// 104 - 'h'
  1, 125,		// 105 - 'i'
  3, 132, 133, 124,		// 106 - 'j'
  4, 127, 16, 40, 68,		// 107 - 'k'
  1, 127,		// 108 - 'l'
  5, 124, 4, 120, 4, 120,		// 109 - 'm'
  4, 124, 4, 4, 120,		// 110 - 'n'
  4, 56, 68, 68, 56,		// 111 - 'o'
  4, 252, 36, 36, 24,		// 112 - 'p'
  4, 24, 36, 36, 252,		// 113 - 'q'
  4, 124, 4, 4, 8,		// 114 - 'r'
  4, 88, 84, 84, 52,		// 115 - 's'
  3, 4, 127, 4,		// 116 - 't'
  4, 60, 64, 64, 124,		// 117 - 'u'
  4, 28, 32, 64, 124,		// 118 - 'v'
  5, 60, 64, 48, 64, 60,		// 119 - 'w'
  4, 108, 16, 16, 108,		// 120 - 'x'
  4, 28, 160, 160, 124,		// 121 - 'y'
  4, 100, 84, 84, 76,		// 122 - 'z'
  4, 8, 54, 65, 65,		// 123 - '{'
  1, 127,		// 124 - '|'
  4, 65, 65, 54, 8,		// 125 - '}'
  4, 2, 1, 2, 1,		// 126 - '~'
  5, 127, 65, 65, 65, 127,		// 127 - 'Hollow Block'
  5, 62, 85, 85, 85, 65,		// 128 - 'Euro symbol'
  5, 56, 68, 68, 56, 68,		// 129 - 'Alpha'
  5, 124, 42, 42, 62, 20,		// 130 - 'Beta'
  5, 126, 2, 2, 6, 6,		// 131 - 'Gamma'
  5, 2, 126, 2, 126, 2,		// 132 - 'Pi'
  5, 99, 85, 73, 65, 99,		// 133 - 'Sigma'
  5, 56, 68, 68, 60, 4,		// 134 - 'Theta'
  5, 64, 126, 32, 30, 32,		// 135 - 'mu'
  5, 6, 2, 126, 2, 2,		// 136 - 'Tau'
  8, 99, 19, 8, 100, 99, 0, 96, 96,		// 137 - 'Percent 00'
  5, 42, 42, 42, 42, 42,		// 138 - '3 Bar Equals'
  3, 81, 74, 68,		// 139 - '>='
  3, 68, 74, 81,		// 140 - '<='
  5, 0, 0, 255, 1, 3,		// 141 - 'Top of Integral'
  3, 224, 128, 255,		// 142 - 'Bot of Integral'
  5, 54, 18, 54, 36, 54,		// 143 - 'Wavy ='
  3, 2, 5, 2,		// 144 - 'Degree'
  2, 24, 24,		// 145 - 'Math Product'
  2, 16, 16,		// 146 - 'Short Dash'
  5, 48, 64, 255, 1, 1,		// 147 - 'Square Root'
  4, 31, 1, 1, 30,		// 148 - 'Superscript n'
  0,		// 149
  0,		// 150  
  0,		// 151  
  0,		// 152
  9, 1, 1, 127, 1, 127, 2, 12, 2, 127,		// 153 - 'Trademark'
  0,		// 154
  0,		// 155
  0,		// 156
  0,		// 157
  0,		// 158
  0,		// 159
  1, 0,		// 160 - ' Non-breaking space'
  1, 125,		// 161 - '¡ Inverted Exclamation Mark'
  4, 24, 36, 126, 36,		// 162 - '¢ Cent sign'
  4, 68, 126, 69, 65,		// 163 - '£ Pound sign'
  5, 34, 28, 20, 28, 34,		// 164 - '¤ Currency sign'
  5, 1, 42, 124, 42, 1,		// 165 - '¥ Yen sign'
  1, 119,		// 166 - '¦ Broken bar'
  4, 78, 85, 85, 57,		// 167 - '§ Section sign'
  3, 2, 0, 2,		// 168 - '¨ Diaeresis (Umlaut)'
  7, 126, 129, 189, 165, 165, 129, 126,		// 169 - '© Copyright sign'
  3, 38, 41, 47,		// 170 - 'ª Feminine Ordinal Indicator'
  5, 8, 20, 42, 20, 34,		// 171 - '« Left-pointing double angle quotation mark'
  4, 8, 8, 8, 24,		// 172 - '¬ Not sign'
  3, 8, 8, 8,		// 173 - ' Soft hyphen'
  7, 126, 129, 189, 149, 169, 129, 126,		// 174 - '® Registered sign'
  5, 1, 1, 1, 1, 1,		// 175 - '¯ macron'
  3, 2, 5, 2,		// 176 - '° Degree symbol'
  3, 36, 46, 36,		// 177 - '± Plus-minus sign'
  3, 25, 21, 18,		// 178 - '² Superscript two'
  3, 21, 21, 10,		// 179 - '³ Superscript three'
  2, 2, 1,		// 180 - '´ Acute accent'
  4, 248, 32, 64, 120,		// 181 - 'µ Micro sign'
  5, 6, 9, 127, 1, 127,		// 182 - 'Pilcrow sign'
  2, 24, 24,		// 183 - '· Middle dot'
  4, 0, 128, 160, 192,		// 184 - '¸ Cedilla'
  3, 18, 31, 16,		// 185 - '¹ Superscript one'
  3, 38, 41, 38,		// 186 - 'º Masculine ordinal indicator'
  5, 34, 20, 42, 20, 8,		// 187 - '» Right-pointing double angle quotation mark'
  8, 64, 47, 16, 8, 4, 30, 17, 124,		// 188 - '¼ Vulgar fraction one quarter'
  8, 64, 47, 16, 8, 4, 98, 85, 76,		// 189 - '½ Vulgar fraction one half'
  9, 21, 85, 63, 16, 8, 4, 30, 17, 124,		// 190 - '¾ Vulgar fraction three quarters'
  5, 48, 72, 72, 77, 64,		// 191 - '¿ Inverted Question Mark'
  5, 120, 21, 22, 20, 120,		// 192 - 'À Latin Capital Letter A with grave'
  5, 120, 20, 22, 21, 120,		// 193 - 'Á Latin Capital letter A with acute'
  5, 122, 21, 20, 21, 122,		// 194 - 'Â Latin Capital letter A with circumflex'
  5, 120, 22, 21, 22, 121,		// 195 - 'Ã Latin Capital letter A with tilde'
  5, 120, 21, 20, 21, 120,		// 196 - 'Ä Latin Capital letter A with diaeresis'
  5, 120, 20, 21, 20, 120,		// 197 - 'Å Latin Capital letter A with ring above'
  7, 126, 9, 9, 127, 73, 73, 65,		// 198 - 'Æ Latin Capital letter Æ'
  5, 158, 161, 97, 33, 33,		// 199 - 'Ç Latin Capital letter C with cedilla'
  5, 124, 84, 85, 70, 68,		// 200 - 'È Latin Capital letter E with grave'
  5, 124, 84, 86, 69, 68,		// 201 - 'É Latin Capital letter E with acute'
  5, 126, 85, 84, 69, 70,		// 202 - 'Ê Latin Capital letter E with circumflex'
  5, 124, 85, 84, 69, 68,		// 203 - 'Ë Latin Capital letter E with diaeresis'
  3, 69, 126, 68,		// 204 - 'Ì Latin Capital letter I with grave'
  3, 68, 126, 69,		// 205 - 'Í Latin Capital letter I with acute'
  3, 70, 125, 70,		// 206 - 'Î Latin Capital letter I with circumflex'
  3, 69, 124, 69,		// 207 - 'Ï Latin Capital letter I with diaeresis'
  5, 8, 127, 73, 65, 62,		// 208 - 'Ð Latin Capital letter Eth'
  5, 124, 10, 17, 34, 125,		// 209 - 'Ñ Latin Capital letter N with tilde'
  5, 56, 68, 69, 70, 56,		// 210 - 'Ò Latin Capital letter O with grave'
  5, 56, 68, 70, 69, 56,		// 211 - 'Ó Latin Capital letter O with acute'
  5, 58, 69, 68, 69, 58,		// 212 - 'Ô Latin Capital letter O with circumflex'
  5, 56, 70, 69, 70, 57,		// 213 - 'Õ Latin Capital letter O with tilde'
  5, 56, 69, 68, 69, 56,		// 214 - 'Ö Latin Capital letter O with diaeresis'
  5, 34, 20, 8, 20, 34,		// 215 - '× Multiplication sign'
  5, 124, 98, 90, 70, 62,		// 216 - 'Ø Latin Capital letter O with stroke'
  5, 60, 64, 65, 66, 60,		// 217 - 'Ù Latin Capital letter U with grave'
  5, 60, 64, 66, 65, 60,		// 218 - 'Ú Latin Capital letter U with acute'
  5, 60, 66, 65, 66, 60,		// 219 - 'Û Latin Capital Letter U with circumflex'
  5, 60, 65, 64, 65, 60,		// 220 - 'Ü Latin Capital Letter U with diaeresis'
  5, 2, 4, 122, 5, 2,		// 221 - 'Ý Latin Capital Letter Y with acute'
  4, 63, 18, 18, 12,		// 222 - 'Þ Latin Capital Letter Thorn'
  5, 126, 73, 73, 78, 48,		// 223 - 'ß Latin Small Letter sharp S'
  4, 56, 69, 70, 124,		// 224 - 'à Latin Small Letter A with grave'
  4, 56, 68, 70, 125,		// 225 - 'á Latin Small Letter A with acute'
  4, 56, 70, 69, 126,		// 226 - 'â Latin Small Letter A with circumflex'
  4, 58, 69, 70, 125,		// 227 - 'ã Latin Small Letter A with tilde'
  4, 56, 69, 68, 125,		// 228 - 'ä Latin Small Letter A with diaeresis'
  4, 48, 74, 77, 122,		// 229 - 'å Latin Small Letter A with ring above'
  7, 32, 84, 84, 56, 84, 84, 88,		// 230 - 'æ Latin Small Letter Æ'
  4, 156, 162, 98, 34,		// 231 - 'ç Latin Small Letter C with cedilla'
  4, 56, 85, 86, 88,		// 232 - 'è Latin Small Letter E with grave'
  4, 56, 84, 86, 89,		// 233 - 'é Latin Small Letter E with acute'
  4, 56, 86, 85, 90,		// 234 - 'ê Latin Small Letter E with circumflex'
  4, 56, 85, 84, 89,		// 235 - 'ë Latin Small Letter E with diaeresis'
  2, 1, 122,		// 236 - 'ì Latin Small Letter I with grave'
  2, 122, 1,		// 237 - 'í Latin Small Letter I with acute'
  3, 2, 121, 2,		// 238 - 'î Latin Small Letter I with circumflex'
  3, 2, 120, 2,		// 239 - 'ï Latin Small Letter I with diaeresis'
  4, 48, 75, 75, 60,		// 240 - 'ð Latin Small Letter Eth'
  4, 122, 9, 10, 113,		// 241 - 'ñ Latin Small Letter N with tilde'
  4, 48, 73, 74, 48,		// 242 - 'ò Latin Small Letter O with grave'
  4, 48, 72, 74, 49,		// 243 - 'ó Latin Small Letter O with acute'
  4, 48, 74, 73, 50,		// 244 - 'ô Latin Small Letter O with circumflex'
  4, 50, 73, 74, 49,		// 245 - 'õ Latin Small Letter O with tilde'
  4, 57, 68, 68, 57,		// 246 - 'ö Latin Small Letter O with diaeresis'
  5, 8, 8, 42, 8, 8,		// 247 - '÷ Division sign'
  4, 56, 84, 76, 56,		// 248 - 'ø Latin Small Letter O with stroke'
  4, 56, 65, 66, 120,		// 249 - 'ù Latin Small Letter U with grave'
  4, 56, 64, 66, 121,		// 250 - 'ú Latin Small Letter U with acute'
  4, 56, 66, 65, 122,		// 251 - 'û Latin Small Letter U with circumflex'
  4, 58, 64, 64, 122,		// 252 - 'ü Latin Small Letter U with diaeresis'
  4, 24, 160, 162, 121,		// 253 - 'ý Latin Small Letter Y with acute'
  4, 252, 40, 40, 16,		// 254 - 'þ Latin Small Letter Thorn'
  4, 26, 160, 160, 122,		// 255 - 'ÿ Latin Small Letter Y with diaeresis'

};











uint16_t setOffset(uint8_t num)
{
	uint16_t offsetr; 
  switch (num)
	{
		case 9: offsetr = 317;
			break;
		case 8: offsetr = 311;
			break;
		case 7: offsetr = 305;
			break;
		case 6: offsetr = 299;
			break;
		case 5: offsetr = 293;
			break;
		case 4: offsetr = 287;
			break;
		case 3: offsetr = 281;
			break;
		case 2: offsetr = 275;
			break;
		case 1: offsetr = 269;
			break;
		case 0: offsetr = 263;
			break;
		default:
          // nothing to do, use the library defaults
          break;
	}
	return offsetr;
}

void load_line1(uint8_t dis_date,uint8_t dis_month,uint8_t dis_year)
{ 
	
	
	uint8_t *temp,*temp1;
	uint8_t *min,*min1;
	uint8_t *sec,*sec1;
	
	temp  = _sysfont + setOffset(dis_date/10);
	temp1 = _sysfont + setOffset(dis_date%10);
	
	min   = _sysfont + setOffset(dis_month/10);//(*(min+4))
	min1  = _sysfont + setOffset(dis_month%10);
	
	sec   = _sysfont + setOffset(2);//(*sec1) (*(min1+4)) (*(sec+4)) 
	sec1  = _sysfont + setOffset(0);//(*(sec1+4))
	
	Buffer_7up[line1][mtrix6] = (((*temp)&BIT1)<<7) + (((*(temp+1))&BIT1) << 6) +(((*(temp+2))&BIT1) << 5) +(((*(temp+3))&BIT1) << 4) + (((*(temp+4))&BIT1) << 3) +0+ (((*temp1)&BIT1) << 1) +  ((*(temp1+1))&BIT1);
	Buffer_7up[line2][mtrix6] = (((*temp)&BIT2)<<6) + (((*(temp+1))&BIT2) << 5) +(((*(temp+2))&BIT2) << 4) +(((*(temp+3))&BIT2) << 3) + (((*(temp+4))&BIT2) << 2) +0+ (((*temp1)&BIT2) << 0) + (((*(temp1+1))&BIT2)>>1);
	Buffer_7up[line3][mtrix6] = (((*temp)&BIT3)<<5) + (((*(temp+1))&BIT3) << 4) +(((*(temp+2))&BIT3) << 3) +(((*(temp+3))&BIT3) << 2) + (((*(temp+4))&BIT3) << 1) +0+ (((*temp1)&BIT3) >> 1) + (((*(temp1+1))&BIT3)>>2);
	Buffer_7up[line4][mtrix6] = (((*temp)&BIT4)<<4) + (((*(temp+1))&BIT4) << 3) +(((*(temp+2))&BIT4) << 2) +(((*(temp+3))&BIT4) << 1) + (((*(temp+4))&BIT4) << 0) +0+ (((*temp1)&BIT4) >> 2) + (((*(temp1+1))&BIT4)>>3);
	Buffer_7up[line5][mtrix6] = (((*temp)&BIT5)<<3) + (((*(temp+1))&BIT5) << 2) +(((*(temp+2))&BIT5) << 1) +(((*(temp+3))&BIT5) << 0) + (((*(temp+4))&BIT5) >> 1) +0+ (((*temp1)&BIT5) >> 3) + (((*(temp1+1))&BIT5)>>4);
	Buffer_7up[line6][mtrix6] = (((*temp)&BIT6)<<2) + (((*(temp+1))&BIT6) << 1) +(((*(temp+2))&BIT6) << 0) +(((*(temp+3))&BIT6) >> 1) + (((*(temp+4))&BIT6) >> 2) +0+ (((*temp1)&BIT6) >> 4) + (((*(temp1+1))&BIT6)>>5);
	Buffer_7up[line7][mtrix6] = (((*temp)&BIT7)<<1) + (((*(temp+1))&BIT7) << 0) +(((*(temp+2))&BIT7) >> 1) +(((*(temp+3))&BIT7) >> 2) + (((*(temp+4))&BIT7) >> 3) +0+ (((*temp1)&BIT7) >> 5) + (((*(temp1+1))&BIT7)>>6);
	Buffer_7up[line8][mtrix6] = (((*temp)&BIT8)<<0) + (((*(temp+1))&BIT8) >> 1) +(((*(temp+2))&BIT8) >> 2) +(((*(temp+3))&BIT8) >> 3) + (((*(temp+4))&BIT8) >> 4) +0+ (((*temp1)&BIT8) >> 6) + (((*(temp1+1))&BIT8)>>7);
	
	Buffer_7up[line1][mtrix7] = (((*(temp1+2))&BIT1)<<7)+ (((*(temp1+3))&BIT1) << 6)+(((*(temp1+4))&BIT1) << 5)   +0							+0  						+0+ (((*min)&BIT1) << 1) +  ((*(min+1))&BIT1);
	Buffer_7up[line2][mtrix7] = (((*(temp1+2))&BIT2)<<6)+ (((*(temp1+3))&BIT2) << 5)+(((*(temp1+4))&BIT2) << 4)		+0							+0							+0+ (((*min)&BIT2) << 0) + (((*(min+1))&BIT2)>>1);
	Buffer_7up[line3][mtrix7] = (((*(temp1+2))&BIT3)<<5)+ (((*(temp1+3))&BIT3) << 4)+(((*(temp1+4))&BIT3) << 3)		+0							+0							+0+ (((*min)&BIT3) >> 1) + (((*(min+1))&BIT3)>>2);
	Buffer_7up[line4][mtrix7] = (((*(temp1+2))&BIT4)<<4)+ (((*(temp1+3))&BIT4) << 3)+(((*(temp1+4))&BIT4) << 2)		+0							+0							+0+ (((*min)&BIT4) >> 2) + (((*(min+1))&BIT4)>>3);
	Buffer_7up[line5][mtrix7] = (((*(temp1+2))&BIT5)<<3)+ (((*(temp1+3))&BIT5) << 2)+(((*(temp1+4))&BIT5) << 1)		+0							+0  					  +0+ (((*min)&BIT5) >> 3) + (((*(min+1))&BIT5)>>4);
	Buffer_7up[line6][mtrix7] = (((*(temp1+2))&BIT6)<<2)+ (((*(temp1+3))&BIT6) << 1)+(((*(temp1+4))&BIT6) << 0)		+0							+0							+0+ (((*min)&BIT6) >> 4) + (((*(min+1))&BIT6)>>5);
	Buffer_7up[line7][mtrix7] = (((*(temp1+2))&BIT7)<<1)+ (((*(temp1+3))&BIT7) << 0)+(((*(temp1+4))&BIT7) >> 1)		+0							+8  						+0+ (((*min)&BIT7) >> 5) + (((*(min+1))&BIT7)>>6);
	Buffer_7up[line8][mtrix7] = (((*(temp1+2))&BIT8)<<0)+ (((*(temp1+3))&BIT8) >> 1)+(((*(temp1+4))&BIT8) >> 2)		+0							+0  						+0+ (((*min)&BIT8) >> 6) + (((*(min+1))&BIT8)>>7);
	
	Buffer_7up[line1][mtrix8] = (((*(min+2))&0x01)<<7) + (((*(min+3))&0x01) << 6) +(((*(min+4))&0x01) << 5) +((0&0x01) << 4) + (((*min1)&0x01) << 3) +(((*(min1+1))&0x01) << 2)+ (((*(min1+2))&0x01) << 1) +  ((*(min1+3))&0x01);
	Buffer_7up[line2][mtrix8] = (((*(min+2))&0x02)<<6) + (((*(min+3))&0x02) << 5) +(((*(min+4))&0x02) << 4) +((0&0x02) << 3) + (((*min1)&0x02) << 2) +(((*(min1+1))&0x02) << 1)+ (((*(min1+2))&0x02) << 0) + (((*(min1+3))&0x02)>>1);
	Buffer_7up[line3][mtrix8] = (((*(min+2))&0x04)<<5) + (((*(min+3))&0x04) << 4) +(((*(min+4))&0x04) << 3) +((0&0x04) << 2) + (((*min1)&0x04) << 1) +(((*(min1+1))&0x04) << 0)+ (((*(min1+2))&0x04) >> 1) + (((*(min1+3))&0x04)>>2);
	Buffer_7up[line4][mtrix8] = (((*(min+2))&0x08)<<4) + (((*(min+3))&0x08) << 3) +(((*(min+4))&0x08) << 2) +((0&0x08) << 1) + (((*min1)&0x08) << 0) +(((*(min1+1))&0x08) >> 1)+ (((*(min1+2))&0x08) >> 2) + (((*(min1+3))&0x08)>>3);
	Buffer_7up[line5][mtrix8] = (((*(min+2))&0x10)<<3) + (((*(min+3))&0x10) << 2) +(((*(min+4))&0x10) << 1) +((0&0x10) << 0) + (((*min1)&0x10) >> 1) +(((*(min1+1))&0x10) >> 2)+ (((*(min1+2))&0x10) >> 3) + (((*(min1+3))&0x10)>>4);
	Buffer_7up[line6][mtrix8] = (((*(min+2))&0x20)<<2) + (((*(min+3))&0x20) << 1) +(((*(min+4))&0x20) << 0) +((0&0x20) >> 1) + (((*min1)&0x20) >> 2) +(((*(min1+1))&0x20) >> 3)+ (((*(min1+2))&0x20) >> 4) + (((*(min1+3))&0x20)>>5);
	Buffer_7up[line7][mtrix8] = (((*(min+2))&0x40)<<1) + (((*(min+3))&0x40) << 0) +(((*(min+4))&0x40) >> 1) +((0&0x40) >> 2) + (((*min1)&0x40) >> 3) +(((*(min1+1))&0x40) >> 4)+ (((*(min1+2))&0x40) >> 5) + (((*(min1+3))&0x40)>>6);
	Buffer_7up[line8][mtrix8] = (((*(min+2))&0x80)<<0) + (((*(min+3))&0x80) >> 1) +(((*(min+4))&0x80) >> 2) +((0&0x80) >> 3) + (((*min1)&0x80) >> 4) +(((*(min1+1))&0x80) >> 5)+ (((*(min1+2))&0x80) >> 6) + (((*(min1+3))&0x80)>>7);
	
	Buffer_7up[line1][mtrix9] = (((*(min1+4))&0x01)<<7) + ((0&0x01) << 6) +((0&0x01) << 5) +((0&0x01) << 4) + (((*sec)&0x01) << 3) +(((*(sec+1))&0x01) << 2)+ (((*(sec+2)) &0x01) << 1) +  ((*(sec+3))&0x01);
	Buffer_7up[line2][mtrix9] = (((*(min1+4))&0x02)<<6) + ((0&0x02) << 5) +((0&0x02) << 4) +((0&0x02) << 3) + (((*sec)&0x02) << 2) +(((*(sec+1))&0x02) << 1)+ (((*(sec+2)) &0x02) << 0) + (((*(sec+3))&0x02)>>1);
	Buffer_7up[line3][mtrix9] = (((*(min1+4))&0x04)<<5) + ((0&0x04) << 4) +00              +((0&0x04) << 2) + (((*sec)&0x04) << 1) +(((*(sec+1))&0x04) << 0)+ (((*(sec+2)) &0x04) >> 1) + (((*(sec+3))&0x04)>>2);
	Buffer_7up[line4][mtrix9] = (((*(min1+4))&0x08)<<4) + ((0&0x08) << 3) +((0&0x08) << 2) +((0&0x08) << 1) + (((*sec)&0x08) << 0) +(((*(sec+1))&0x08) >> 1)+ (((*(sec+2)) &0x08) >> 2) + (((*(sec+3))&0x08)>>3);
	Buffer_7up[line5][mtrix9] = (((*(min1+4))&0x10)<<3) + ((0&0x10) << 2) +((0&0x10) << 1) +((0&0x10) << 0) + (((*sec)&0x10) >> 1) +(((*(sec+1))&0x10) >> 2)+ (((*(sec+2)) &0x10) >> 3) + (((*(sec+3))&0x10)>>4);
	Buffer_7up[line6][mtrix9] = (((*(min1+4))&0x20)<<2) + ((0&0x20) << 1) +((0&0x20) << 0) +((0&0x20) >> 1)+ (((*sec)&0x20) >> 2) +(((*(sec+1))&0x20) >> 3)+ (((*(sec+2)) &0x20) >> 4) + (((*(sec+3))&0x20)>>5);
	Buffer_7up[line7][mtrix9] = (((*(min1+4))&0x40)<<1) + ((0&0x40) << 0) +32              +((0&0x40) >> 2) + (((*sec)&0x40) >> 3) +(((*(sec+1))&0x40) >> 4)+ (((*(sec+2)) &0x40) >> 5) + (((*(sec+3))&0x40)>>6);
	Buffer_7up[line8][mtrix9] = (((*(min1+4))&0x80)<<0) + ((0&0x80) >> 1) +((0&0x80) >> 2) +((0&0x80) >> 3) + (((*sec)&0x80) >> 4) +(((*(sec+1))&0x80) >> 5)+ (((*(sec+2)) &0x80) >> 6) + (((*(sec+3))&0x80)>>7);
	
	Buffer_7up[line1][mtrix10]= (((*(sec+4))&0x01)<<7) + ((0&0x01) << 6) +(((*sec1)&0x01) << 5) +(((*(sec1+1))&0x01) << 4) + (((*(sec1+2))&0x01) << 3) +(((*(sec1+3))&0x01) << 2)+ (((*(sec1+4))&0x01) << 1) + ( 0&0x01);
	Buffer_7up[line2][mtrix10]= (((*(sec+4))&0x02)<<6) + ((0&0x02) << 5) +(((*sec1)&0x02) << 4) +(((*(sec1+1))&0x02) << 3) + (((*(sec1+2))&0x02) << 2) +(((*(sec1+3))&0x02) << 1)+ (((*(sec1+4))&0x02) << 0) + ((0&0x02)>>1);
	Buffer_7up[line3][mtrix10]= (((*(sec+4))&0x04)<<5) + ((0&0x04) << 4) +(((*sec1)&0x04) << 3) +(((*(sec1+1))&0x04) << 2) + (((*(sec1+2))&0x04) << 1) +(((*(sec1+3))&0x04) << 0)+ (((*(sec1+4))&0x04) >> 1) + ((0&0x04)>>2);
	Buffer_7up[line4][mtrix10]= (((*(sec+4))&0x08)<<4) + ((0&0x08) << 3) +(((*sec1)&0x08) << 2) +(((*(sec1+1))&0x08) << 1) + (((*(sec1+2))&0x08) << 0) +(((*(sec1+3))&0x08) >> 1)+ (((*(sec1+4))&0x08) >> 2) + ((0&0x08)>>3);
	Buffer_7up[line5][mtrix10]= (((*(sec+4))&0x10)<<3) + ((0&0x10) << 2) +(((*sec1)&0x10) << 1) +(((*(sec1+1))&0x10) << 0) + (((*(sec1+2))&0x10) >> 1) +(((*(sec1+3))&0x10) >> 2)+ (((*(sec1+4))&0x10) >> 3) + ((0&0x10)>>4);
	Buffer_7up[line6][mtrix10]= (((*(sec+4))&0x20)<<2) + ((0&0x20) << 1) +(((*sec1)&0x20) << 0) +(((*(sec1+1))&0x20) >> 1) + (((*(sec1+2))&0x20) >> 2) +(((*(sec1+3))&0x20) >> 3)+ (((*(sec1+4))&0x20) >> 4) + ((0&0x20)>>5);
	Buffer_7up[line7][mtrix10]= (((*(sec+4))&0x40)<<1) + ((0&0x40) << 0) +(((*sec1)&0x40) >> 1) +(((*(sec1+1))&0x40) >> 2) + (((*(sec1+2))&0x40) >> 3) +(((*(sec1+3))&0x40) >> 4)+ (((*(sec1+4))&0x40) >> 5) + ((0&0x40)>>6);
	Buffer_7up[line8][mtrix10]= (((*(sec+4))&0x80)<<0) + ((0&0x80) >> 1) +(((*sec1)&0x80) >> 2) +(((*(sec1+1))&0x80) >> 3) + (((*(sec1+2))&0x80) >> 4) +(((*(sec1+3))&0x80) >> 5)+ (((*(sec1+4))&0x80) >> 6) + ((0&0x80)>>7);
	
	sec   = _sysfont + setOffset(dis_year/10);//(*sec1) (*(min1+4)) (*(sec+4)) 
	sec1  = _sysfont + setOffset(dis_year%10);//(*(sec1+4))  *(sec1+4)
	
	Buffer_7up[line1][mtrix11]= (((*(sec+0))&0x01)<<7) + (((*(sec+1))&0x01) << 6) +(((*(sec+2))&0x01) << 5) +(((*(sec+3))&0x01) << 4) + (((*(sec+4))&0x01) << 3) +((0&0x01) << 2)+ (((*(sec1+0))&0x01) << 1) + ( (*(sec1+1))&0x01);
	Buffer_7up[line2][mtrix11]= (((*(sec+0))&0x02)<<6) + (((*(sec+1))&0x02) << 5) +(((*(sec+2))&0x02) << 4) +(((*(sec+3))&0x02) << 3) + (((*(sec+4))&0x02) << 2) +((0&0x02) << 1)+ (((*(sec1+0))&0x02) << 0) + (((*(sec1+1))&0x02)>>1);
	Buffer_7up[line3][mtrix11]= (((*(sec+0))&0x04)<<5) + (((*(sec+1))&0x04) << 4) +(((*(sec+2))&0x04) << 3) +(((*(sec+3))&0x04) << 2) + (((*(sec+4))&0x04) << 1) +((0&0x04) << 0)+ (((*(sec1+0))&0x04) >> 1) + (((*(sec1+1))&0x04)>>2);
	Buffer_7up[line4][mtrix11]= (((*(sec+0))&0x08)<<4) + (((*(sec+1))&0x08) << 3) +(((*(sec+2))&0x08) << 2) +(((*(sec+3))&0x08) << 1) + (((*(sec+4))&0x08) << 0) +((0&0x08) >> 1)+ (((*(sec1+0))&0x08) >> 2) + (((*(sec1+1))&0x08)>>3);
	Buffer_7up[line5][mtrix11]= (((*(sec+0))&0x10)<<3) + (((*(sec+1))&0x10) << 2) +(((*(sec+2))&0x10) << 1) +(((*(sec+3))&0x10) << 0) + (((*(sec+4))&0x10) >> 1) +((0&0x10) >> 2)+ (((*(sec1+0))&0x10) >> 3) + (((*(sec1+1))&0x10)>>4);
	Buffer_7up[line6][mtrix11]= (((*(sec+0))&0x20)<<2) + (((*(sec+1))&0x20) << 1) +(((*(sec+2))&0x20) << 0) +(((*(sec+3))&0x20) >> 1) + (((*(sec+4))&0x20) >> 2) +((0&0x20) >> 3)+ (((*(sec1+0))&0x20) >> 4) + (((*(sec1+1))&0x20)>>5);
	Buffer_7up[line7][mtrix11]= (((*(sec+0))&0x40)<<1) + (((*(sec+1))&0x40) << 0) +(((*(sec+2))&0x40) >> 1) +(((*(sec+3))&0x40) >> 2) + (((*(sec+4))&0x40) >> 3) +((0&0x40) >> 4)+ (((*(sec1+0))&0x40) >> 5) + (((*(sec1+1))&0x40)>>6);
	Buffer_7up[line8][mtrix11]= (((*(sec+0))&0x80)<<0) + (((*(sec+1))&0x80) >> 1) +(((*(sec+2))&0x80) >> 2) +(((*(sec+3))&0x80) >> 3) + (((*(sec+4))&0x80) >> 4) +((0&0x80) >> 5)+ (((*(sec1+0))&0x80) >> 6) + (((*(sec1+1))&0x80)>>7);
	
	Buffer_7up[line1][mtrix12]= (((*(sec+2))&0x01)<<7) + (((*(sec1+3))&0x01) << 6) +(((*(sec1+4))&0x01) << 5);// +(((*(sec1+1))&0x01) << 4) + (((*(sec1+2))&0x01) << 3) +(((*(sec1+3))&0x01) << 2)+ (((*(sec1+4))&0x01) << 1) + ( 0&0x01);
	Buffer_7up[line2][mtrix12]= (((*(sec+2))&0x02)<<6) + (((*(sec1+3))&0x02) << 5) +(((*(sec1+4))&0x02) << 4);// +(((*(sec1+1))&0x02) << 3) + (((*(sec1+2))&0x02) << 2) +(((*(sec1+3))&0x02) << 1)+ (((*(sec1+4))&0x02) << 0) + ((0&0x02)>>1);
	Buffer_7up[line3][mtrix12]= (((*(sec+2))&0x04)<<5) + (((*(sec1+3))&0x04) << 4) +(((*(sec1+4))&0x04) << 3);// +(((*(sec1+1))&0x04) << 2) + (((*(sec1+2))&0x04) << 1) +(((*(sec1+3))&0x04) << 0)+ (((*(sec1+4))&0x04) >> 1) + ((0&0x04)>>2);
	Buffer_7up[line4][mtrix12]= (((*(sec+2))&0x08)<<4) + (((*(sec1+3))&0x08) << 3) +(((*(sec1+4))&0x08) << 2);// +(((*(sec1+1))&0x08) << 1) + (((*(sec1+2))&0x08) << 0) +(((*(sec1+3))&0x08) >> 1)+ (((*(sec1+4))&0x08) >> 2) + ((0&0x08)>>3);
	Buffer_7up[line5][mtrix12]= (((*(sec+2))&0x10)<<3) + (((*(sec1+3))&0x10) << 2) +(((*(sec1+4))&0x10) << 1);// +(((*(sec1+1))&0x10) << 0) + (((*(sec1+2))&0x10) >> 1) +(((*(sec1+3))&0x10) >> 2)+ (((*(sec1+4))&0x10) >> 3) + ((0&0x10)>>4);
	Buffer_7up[line6][mtrix12]= (((*(sec+2))&0x20)<<2) + (((*(sec1+3))&0x20) << 1) +(((*(sec1+4))&0x20) << 0);// +(((*(sec1+1))&0x20) >> 1) + (((*(sec1+2))&0x20) >> 2) +(((*(sec1+3))&0x20) >> 3)+ (((*(sec1+4))&0x20) >> 4) + ((0&0x20)>>5);
	Buffer_7up[line7][mtrix12]= (((*(sec+2))&0x40)<<1) + (((*(sec1+3))&0x40) << 0) +(((*(sec1+4))&0x40) >> 1);// +(((*(sec1+1))&0x40) >> 2) + (((*(sec1+2))&0x40) >> 3) +(((*(sec1+3))&0x40) >> 4)+ (((*(sec1+4))&0x40) >> 5) + ((0&0x40)>>6);
	Buffer_7up[line8][mtrix12]= (((*(sec+2))&0x80)<<0) + (((*(sec1+3))&0x80) >> 1) +(((*(sec1+4))&0x80) >> 2);// +(((*(sec1+1))&0x80) >> 3) + (((*(sec1+2))&0x80) >> 4) +(((*(sec1+3))&0x80) >> 5)+ (((*(sec1+4))&0x80) >> 6) + ((0&0x80)>>7);
}

void load_line2(uint8_t dis_hour,uint8_t dis_min,uint8_t dis_sec,uint8_t dot)
{ 

	
	uint8_t *temp,*temp1;
	uint8_t *min,*min1;
	uint8_t *sec,*sec1;
	
	
	temp  = _sysfont + setOffset(dis_hour/10);
	temp1 = _sysfont + setOffset(dis_hour%10);
	
	min   = _sysfont + setOffset(dis_min/10);//(*(min+4))
	min1  = _sysfont + setOffset(dis_min%10);
	
	sec   = _sysfont + setOffset(dis_sec/10);//(*sec1) (*(min1+4)) (*(sec+4)) 
	sec1  = _sysfont + setOffset(dis_sec%10);//(*(sec1+4))
	
	Buffer_5down[line1][mtrix1] = (((*temp)&0x01)<<7) + (((*(temp+1))&0x01) << 6) +(((*(temp+2))&0x01) << 5) +(((*(temp+3))&0x01) << 4) + (((*(temp+4))&0x01) << 3) +0+ (((*temp1)&0x01) << 1) + ((*(temp1+1))&0x01);
	Buffer_5down[line2][mtrix1] = (((*temp)&0x02)<<6) + (((*(temp+1))&0x02) << 5) +(((*(temp+2))&0x02) << 4) +(((*(temp+3))&0x02) << 3) + (((*(temp+4))&0x02) << 2) +0+ (((*temp1)&0x02) << 0) + (((*(temp1+1))&0x02)>>1);
	Buffer_5down[line3][mtrix1] = (((*temp)&0x04)<<5) + (((*(temp+1))&0x04) << 4) +(((*(temp+2))&0x04) << 3) +(((*(temp+3))&0x04) << 2) + (((*(temp+4))&0x04) << 1) +0+ (((*temp1)&0x04) >> 1) + (((*(temp1+1))&0x04)>>2);
	Buffer_5down[line4][mtrix1] = (((*temp)&0x08)<<4) + (((*(temp+1))&0x08) << 3) +(((*(temp+2))&0x08) << 2) +(((*(temp+3))&0x08) << 1) + (((*(temp+4))&0x08) << 0) +0+ (((*temp1)&0x08) >> 2) + (((*(temp1+1))&0x08)>>3);
	Buffer_5down[line5][mtrix1] = (((*temp)&0x10)<<3) + (((*(temp+1))&0x10) << 2) +(((*(temp+2))&0x10) << 1) +(((*(temp+3))&0x10) << 0) + (((*(temp+4))&0x10) >> 1) +0+ (((*temp1)&0x10) >> 3) + (((*(temp1+1))&0x10)>>4);
	Buffer_5down[line6][mtrix1] = (((*temp)&0x20)<<2) + (((*(temp+1))&0x20) << 1) +(((*(temp+2))&0x20) << 0) +(((*(temp+3))&0x20) >> 1) + (((*(temp+4))&0x20) >> 2) +0+ (((*temp1)&0x20) >> 4) + (((*(temp1+1))&0x20)>>5);
	Buffer_5down[line7][mtrix1] = (((*temp)&0x40)<<1) + (((*(temp+1))&0x40) << 0) +(((*(temp+2))&0x40) >> 1) +(((*(temp+3))&0x40) >> 2) + (((*(temp+4))&0x40) >> 3) +0+ (((*temp1)&0x40) >> 5) + (((*(temp1+1))&0x40)>>6);
	Buffer_5down[line8][mtrix1] = (((*temp)&0x80)<<0) + (((*(temp+1))&0x80) >> 1) +(((*(temp+2))&0x80) >> 2) +(((*(temp+3))&0x80) >> 3) + (((*(temp+4))&0x80) >> 4) +0+ (((*temp1)&0x80) >> 6) + (((*(temp1+1))&0x80)>>7);
	
	Buffer_5down[line1][mtrix2] = (((*(temp1+2))&0x01)<<7)+ (((*(temp1+3))&0x01) << 6)+(((*(temp1+4))&0x01) << 5)   +0								+0  						+0+ (((*min)&0x01) << 1) +  ((*(min+1))&0x01);
	Buffer_5down[line2][mtrix2] = (((*(temp1+2))&0x02)<<6)+ (((*(temp1+3))&0x02) << 5)+(((*(temp1+4))&0x02) << 4)		+0							+0							  +0+ (((*min)&0x02) << 0) + (((*(min+1))&0x02)>>1);
	Buffer_5down[line3][mtrix2] = (((*(temp1+2))&0x04)<<5)+ (((*(temp1+3))&0x04) << 4)+(((*(temp1+4))&0x04) << 3)		+0							+0x8							+0+ (((*min)&0x04) >> 1) + (((*(min+1))&0x04)>>2);
	Buffer_5down[line4][mtrix2] = (((*(temp1+2))&0x08)<<4)+ (((*(temp1+3))&0x08) << 3)+(((*(temp1+4))&0x08) << 2)		+0							+0							  +0+ (((*min)&0x08) >> 2) + (((*(min+1))&0x08)>>3);
	Buffer_5down[line5][mtrix2] = (((*(temp1+2))&0x10)<<3)+ (((*(temp1+3))&0x10) << 2)+(((*(temp1+4))&0x10) << 1)		+0							+0  							+0+ (((*min)&0x10) >> 3) + (((*(min+1))&0x10)>>4);
	Buffer_5down[line6][mtrix2] = (((*(temp1+2))&0x20)<<2)+ (((*(temp1+3))&0x20) << 1)+(((*(temp1+4))&0x20) << 0)		+0							+0x8							+0+ (((*min)&0x20) >> 4) + (((*(min+1))&0x20)>>5);
	Buffer_5down[line7][mtrix2] = (((*(temp1+2))&0x40)<<1)+ (((*(temp1+3))&0x40) << 0)+(((*(temp1+4))&0x40) >> 1)		+0							+0  							+0+ (((*min)&0x40) >> 5) + (((*(min+1))&0x40)>>6);
	Buffer_5down[line8][mtrix2] = (((*(temp1+2))&0x80)<<0)+ (((*(temp1+3))&0x80) >> 1)+(((*(temp1+4))&0x80) >> 2)		+0							+0  							+0+ (((*min)&0x80) >> 6) + (((*(min+1))&0x80)>>7);
	
	Buffer_5down[line1][mtrix3] = (((*(min+2))&0x01)<<7) + (((*(min+3))&0x01) << 6) +(((*(min+4))&0x01) << 5) +((0&0x01) << 4) + (((*min1)&0x01) << 3) +(((*(min1+1))&0x01) << 2)+ (((*(min1+2))&0x01) << 1) +  ((*(min1+3))&0x01);
	Buffer_5down[line2][mtrix3] = (((*(min+2))&0x02)<<6) + (((*(min+3))&0x02) << 5) +(((*(min+4))&0x02) << 4) +((0&0x02) << 3) + (((*min1)&0x02) << 2) +(((*(min1+1))&0x02) << 1)+ (((*(min1+2))&0x02) << 0) + (((*(min1+3))&0x02)>>1);
	Buffer_5down[line3][mtrix3] = (((*(min+2))&0x04)<<5) + (((*(min+3))&0x04) << 4) +(((*(min+4))&0x04) << 3) +((0&0x04) << 2) + (((*min1)&0x04) << 1) +(((*(min1+1))&0x04) << 0)+ (((*(min1+2))&0x04) >> 1) + (((*(min1+3))&0x04)>>2);
	Buffer_5down[line4][mtrix3] = (((*(min+2))&0x08)<<4) + (((*(min+3))&0x08) << 3) +(((*(min+4))&0x08) << 2) +((0&0x08) << 1) + (((*min1)&0x08) << 0) +(((*(min1+1))&0x08) >> 1)+ (((*(min1+2))&0x08) >> 2) + (((*(min1+3))&0x08)>>3);
	Buffer_5down[line5][mtrix3] = (((*(min+2))&0x10)<<3) + (((*(min+3))&0x10) << 2) +(((*(min+4))&0x10) << 1) +((0&0x10) << 0) + (((*min1)&0x10) >> 1) +(((*(min1+1))&0x10) >> 2)+ (((*(min1+2))&0x10) >> 3) + (((*(min1+3))&0x10)>>4);
	Buffer_5down[line6][mtrix3] = (((*(min+2))&0x20)<<2) + (((*(min+3))&0x20) << 1) +(((*(min+4))&0x20) << 0) +((0&0x20) >> 1) + (((*min1)&0x20) >> 2) +(((*(min1+1))&0x20) >> 3)+ (((*(min1+2))&0x20) >> 4) + (((*(min1+3))&0x20)>>5);
	Buffer_5down[line7][mtrix3] = (((*(min+2))&0x40)<<1) + (((*(min+3))&0x40) << 0) +(((*(min+4))&0x40) >> 1) +((0&0x40) >> 2) + (((*min1)&0x40) >> 3) +(((*(min1+1))&0x40) >> 4)+ (((*(min1+2))&0x40) >> 5) + (((*(min1+3))&0x40)>>6);
	Buffer_5down[line8][mtrix3] = (((*(min+2))&0x80)<<0) + (((*(min+3))&0x80) >> 1) +(((*(min+4))&0x80) >> 2) +((0&0x80) >> 3) + (((*min1)&0x80) >> 4) +(((*(min1+1))&0x80) >> 5)+ (((*(min1+2))&0x80) >> 6) + (((*(min1+3))&0x80)>>7);
	
	Buffer_5down[line1][mtrix4] = (((*(min1+4))&0x01)<<7) + ((0&0x01) << 6) +((0&0x01) << 5) +((0&0x01) << 4) + (((*sec)&0x01) << 3) +(((*(sec+1))&0x01) << 2)+ (((*(sec+2)) &0x01) << 1) +  ((*(sec+3))&0x01);
	Buffer_5down[line2][mtrix4] = (((*(min1+4))&0x02)<<6) + ((0&0x02) << 5) +((0&0x02) << 4) +((0&0x02) << 3) + (((*sec)&0x02) << 2) +(((*(sec+1))&0x02) << 1)+ (((*(sec+2)) &0x02) << 0) + (((*(sec+3))&0x02)>>1);
	Buffer_5down[line3][mtrix4] = (((*(min1+4))&0x04)<<5) + ((0&0x04) << 4) +32              +((0&0x04) << 2) + (((*sec)&0x04) << 1) +(((*(sec+1))&0x04) << 0)+ (((*(sec+2)) &0x04) >> 1) + (((*(sec+3))&0x04)>>2);
	Buffer_5down[line4][mtrix4] = (((*(min1+4))&0x08)<<4) + ((0&0x08) << 3) +((0&0x08) << 2) +((0&0x08) << 1) + (((*sec)&0x08) << 0) +(((*(sec+1))&0x08) >> 1)+ (((*(sec+2)) &0x08) >> 2) + (((*(sec+3))&0x08)>>3);
	Buffer_5down[line5][mtrix4] = (((*(min1+4))&0x10)<<3) + ((0&0x10) << 2) +((0&0x10) << 1) +((0&0x10) << 0) + (((*sec)&0x10) >> 1) +(((*(sec+1))&0x10) >> 2)+ (((*(sec+2)) &0x10) >> 3) + (((*(sec+3))&0x10)>>4);
	Buffer_5down[line6][mtrix4] = (((*(min1+4))&0x20)<<2) + ((0&0x20) << 1) +((32&0x20) << 0) +((0&0x20) >> 1)+ (((*sec)&0x20) >> 2) +(((*(sec+1))&0x20) >> 3)+ (((*(sec+2)) &0x20) >> 4) + (((*(sec+3))&0x20)>>5);
	Buffer_5down[line7][mtrix4] = (((*(min1+4))&0x40)<<1) + ((0&0x40) << 0) +((0&0x40) >> 1) +((0&0x40) >> 2) + (((*sec)&0x40) >> 3) +(((*(sec+1))&0x40) >> 4)+ (((*(sec+2)) &0x40) >> 5) + (((*(sec+3))&0x40)>>6);
	Buffer_5down[line8][mtrix4] = (((*(min1+4))&0x80)<<0) + ((0&0x80) >> 1) +((0&0x80) >> 2) +((0&0x80) >> 3) + (((*sec)&0x80) >> 4) +(((*(sec+1))&0x80) >> 5)+ (((*(sec+2)) &0x80) >> 6) + (((*(sec+3))&0x80)>>7);
	
	Buffer_5down[line1][mtrix5] = (((*(sec+4))&0x01)<<7) + ((0&0x01) << 6) +(((*sec1)&0x01) << 5) +(((*(sec1+1))&0x01) << 4) + (((*(sec1+2))&0x01) << 3) +(((*(sec1+3))&0x01) << 2)+ (((*(sec1+4))&0x01) << 1) + ( 0&0x01);
	Buffer_5down[line2][mtrix5] = (((*(sec+4))&0x02)<<6) + ((0&0x02) << 5) +(((*sec1)&0x02) << 4) +(((*(sec1+1))&0x02) << 3) + (((*(sec1+2))&0x02) << 2) +(((*(sec1+3))&0x02) << 1)+ (((*(sec1+4))&0x02) << 0) + ((0&0x02)>>1);
	Buffer_5down[line3][mtrix5] = (((*(sec+4))&0x04)<<5) + ((0&0x04) << 4) +(((*sec1)&0x04) << 3) +(((*(sec1+1))&0x04) << 2) + (((*(sec1+2))&0x04) << 1) +(((*(sec1+3))&0x04) << 0)+ (((*(sec1+4))&0x04) >> 1) + ((0&0x04)>>2);
	Buffer_5down[line4][mtrix5] = (((*(sec+4))&0x08)<<4) + ((0&0x08) << 3) +(((*sec1)&0x08) << 2) +(((*(sec1+1))&0x08) << 1) + (((*(sec1+2))&0x08) << 0) +(((*(sec1+3))&0x08) >> 1)+ (((*(sec1+4))&0x08) >> 2) + ((0&0x08)>>3);
	Buffer_5down[line5][mtrix5] = (((*(sec+4))&0x10)<<3) + ((0&0x10) << 2) +(((*sec1)&0x10) << 1) +(((*(sec1+1))&0x10) << 0) + (((*(sec1+2))&0x10) >> 1) +(((*(sec1+3))&0x10) >> 2)+ (((*(sec1+4))&0x10) >> 3) + ((0&0x10)>>4);
	Buffer_5down[line6][mtrix5] = (((*(sec+4))&0x20)<<2) + ((0&0x20) << 1) +(((*sec1)&0x20) << 0) +(((*(sec1+1))&0x20) >> 1) + (((*(sec1+2))&0x20) >> 2) +(((*(sec1+3))&0x20) >> 3)+ (((*(sec1+4))&0x20) >> 4) + ((0&0x20)>>5);
	Buffer_5down[line7][mtrix5] = (((*(sec+4))&0x40)<<1) + ((0&0x40) << 0) +(((*sec1)&0x40) >> 1) +(((*(sec1+1))&0x40) >> 2) + (((*(sec1+2))&0x40) >> 3) +(((*(sec1+3))&0x40) >> 4)+ (((*(sec1+4))&0x40) >> 5) + ((0&0x40)>>6);
	Buffer_5down[line8][mtrix5] = (((*(sec+4))&0x80)<<0) + ((0&0x80) >> 1) +(((*sec1)&0x80) >> 2) +(((*(sec1+1))&0x80) >> 3) + (((*(sec1+2))&0x80) >> 4) +(((*(sec1+3))&0x80) >> 5)+ (((*(sec1+4))&0x80) >> 6) + ((0&0x80)>>7);
}
	


void line2_matrix_SendByte (unsigned char dat)
{
unsigned char i;
	for (i=0;i<8;i++)
	{
		if((dat&0x80)==0x80) HAL_GPIO_WritePin(LED_DIN_GPIO_Port, LED_DIN_Pin, GPIO_PIN_SET);//GPIO_SetBits(GPIOA, MAX7219_DIN);
		else HAL_GPIO_WritePin(LED_DIN_GPIO_Port, LED_DIN_Pin, GPIO_PIN_RESET);//GPIO_ResetBits(GPIOA, MAX7219_DIN);
		HAL_GPIO_WritePin(LED_CLK_GPIO_Port, LED_CLK_Pin, GPIO_PIN_RESET);//GPIO_ResetBits(GPIOA, MAX7219_CLK);	
		HAL_GPIO_WritePin(LED_CLK_GPIO_Port, LED_CLK_Pin, GPIO_PIN_SET);//GPIO_SetBits(GPIOA, MAX7219_CLK);
		dat<<=1;
	}
}

void line2_matrix_SendAdd (unsigned char addr,unsigned char dat)
{
	line2_matrix_SendByte(addr);
	line2_matrix_SendByte(dat);
}

void scan_5down(void)
{
	unsigned char i;
	
	for (i=1;i<9;i++){
		HAL_GPIO_WritePin(LED_LOAD_GPIO_Port, LED_LOAD_Pin, GPIO_PIN_RESET);;
	  line2_matrix_SendAdd(i,Buffer_5down[i-1][num1]);
		line2_matrix_SendAdd(i,Buffer_5down[i-1][num2]);
		line2_matrix_SendAdd(i,Buffer_5down[i-1][num3]);
		line2_matrix_SendAdd(i,Buffer_5down[i-1][num4]);
		line2_matrix_SendAdd(i,Buffer_5down[i-1][num5]);
		HAL_GPIO_WritePin(LED_LOAD_GPIO_Port, LED_LOAD_Pin, GPIO_PIN_SET);;
	}
	
	
}
void line2_matrix_init (void)
{
	if(LEDintensity < 1)  LEDintensity =1;
	if(LEDintensity > 15) LEDintensity =15;
	HAL_GPIO_WritePin(LED_LOAD_GPIO_Port, LED_LOAD_Pin, GPIO_PIN_RESET);
	
	//HAL_Delay(1);
	line2_matrix_SendAdd(shut_down,0x01); //normal operation
	line2_matrix_SendAdd(shut_down,0x01); //normal operation
	line2_matrix_SendAdd(shut_down,0x01); //normal operation
	line2_matrix_SendAdd(shut_down,0x01); //normal operation
	line2_matrix_SendAdd(shut_down,0x01); //normal operation
	line2_matrix_SendAdd(shut_down,0x01); //normal operation
	HAL_GPIO_WritePin(LED_LOAD_GPIO_Port, LED_LOAD_Pin, GPIO_PIN_SET);
	//for (i=0;i<8;i++);
	//HAL_Delay(1);
	/*HAL_GPIO_WritePin(LED_LOAD_GPIO_Port, LED_LOAD_Pin, GPIO_PIN_RESET);HAL_Delay(1);
	line2_matrix_SendAdd(decodeMode,0x00); //decoding
	line2_matrix_SendAdd(decodeMode,0x00); //decoding
	line2_matrix_SendAdd(decodeMode,0x00); //decoding
	line2_matrix_SendAdd(decodeMode,0x00); //decoding
	line2_matrix_SendAdd(decodeMode,0x00); //decoding
	HAL_GPIO_WritePin(LED_LOAD_GPIO_Port, LED_LOAD_Pin, GPIO_PIN_SET);
	HAL_Delay(1);*/
	HAL_GPIO_WritePin(LED_LOAD_GPIO_Port, LED_LOAD_Pin, GPIO_PIN_RESET);
	//HAL_Delay(1);
	//for (i=0;i<8;i++);
	line2_matrix_SendAdd(intensity,LEDintensity); //intensity
	line2_matrix_SendAdd(intensity,LEDintensity); //intensity
	line2_matrix_SendAdd(intensity,LEDintensity); //intensity
	line2_matrix_SendAdd(intensity,LEDintensity); //intensity
	line2_matrix_SendAdd(intensity,LEDintensity); //intensity
	line2_matrix_SendAdd(intensity,LEDintensity); //intensity
	HAL_GPIO_WritePin(LED_LOAD_GPIO_Port, LED_LOAD_Pin, GPIO_PIN_SET);
	//for (i=0;i<8;i++);
	//HAL_Delay(1);
	HAL_GPIO_WritePin(LED_LOAD_GPIO_Port, LED_LOAD_Pin, GPIO_PIN_RESET);
	//HAL_Delay(1);
	//for (i=0;i<8;i++);
	line2_matrix_SendAdd(scanLimit,0x07); //all digits on
	line2_matrix_SendAdd(scanLimit,0x07); //all digits on
	line2_matrix_SendAdd(scanLimit,0x07); //all digits on
	line2_matrix_SendAdd(scanLimit,0x07); //all digits on
	line2_matrix_SendAdd(scanLimit,0x07); //all digits on
	line2_matrix_SendAdd(scanLimit,0x07); //all digits on
	HAL_GPIO_WritePin(LED_LOAD_GPIO_Port, LED_LOAD_Pin, GPIO_PIN_SET);
	//for (i=0;i<8;i++);
	//HAL_Delay(1);
	/*HAL_GPIO_WritePin(LED_LOAD_GPIO_Port, LED_LOAD_Pin, GPIO_PIN_RESET);HAL_Delay(1);
	line2_matrix_SendAdd(disTest,0x00); //display test off
	line2_matrix_SendAdd(disTest,0x00); //display test off
	line2_matrix_SendAdd(disTest,0x00); //display test off
	line2_matrix_SendAdd(disTest,0x00); //display test off
	line2_matrix_SendAdd(disTest,0x00); //display test off
	HAL_GPIO_WritePin(LED_LOAD_GPIO_Port, LED_LOAD_Pin, GPIO_PIN_SET);
	HAL_Delay(1);*/
	
}



void up7_matrix_SendByte (unsigned char dat)
{
unsigned char i;
	for (i=0;i<8;i++)
	{
		if((dat&0x80)==0x80) HAL_GPIO_WritePin(DIN2_GPIO_Port, DIN2_Pin, GPIO_PIN_SET);//GPIO_SetBits(GPIOA, MAX7219_DIN);
		else HAL_GPIO_WritePin(DIN2_GPIO_Port, DIN2_Pin, GPIO_PIN_RESET);//GPIO_ResetBits(GPIOA, MAX7219_DIN);
		HAL_GPIO_WritePin(CLK2_GPIO_Port, CLK2_Pin, GPIO_PIN_RESET);//GPIO_ResetBits(GPIOA, MAX7219_CLK);
		HAL_GPIO_WritePin(CLK2_GPIO_Port, CLK2_Pin, GPIO_PIN_SET);//GPIO_SetBits(GPIOA, MAX7219_CLK);
		dat<<=1;
	}
}

void up7_matrix_SendAdd (unsigned char addr,unsigned char dat)
{
	up7_matrix_SendByte(addr);
	up7_matrix_SendByte(dat);
}
//Buffer_7up[line][num]
void scan_7up(void)
{
	unsigned char i;
	
	for (i=1;i<9;i++){
		LOAD2_LOW;
	  up7_matrix_SendAdd(i,Buffer_7up[i-1][num1]);
		up7_matrix_SendAdd(i,Buffer_7up[i-1][num2]);
		up7_matrix_SendAdd(i,Buffer_7up[i-1][num3]);
		up7_matrix_SendAdd(i,Buffer_7up[i-1][num4]);
		up7_matrix_SendAdd(i,Buffer_7up[i-1][num5]);
		up7_matrix_SendAdd(i,Buffer_7up[i-1][num6]);
		up7_matrix_SendAdd(i,Buffer_7up[i-1][num7]);
		LOAD2_HIGH;
	}
	
	
}

void up7_matrix_init (void)
{
	if(LEDintensity < 1)  LEDintensity =1;
	if(LEDintensity > 15) LEDintensity =15;
	
	HAL_GPIO_WritePin(LOAD2_GPIO_Port, LOAD2_Pin, GPIO_PIN_RESET);
	up7_matrix_SendAdd(shut_down,0x01); //normal operation
	up7_matrix_SendAdd(shut_down,0x01); //normal operation
	up7_matrix_SendAdd(shut_down,0x01); //normal operation
	up7_matrix_SendAdd(shut_down,0x01); //normal operation
	up7_matrix_SendAdd(shut_down,0x01); //normal operation
	up7_matrix_SendAdd(shut_down,0x01); //normal operation
	up7_matrix_SendAdd(shut_down,0x01); //normal operation
	HAL_GPIO_WritePin(LOAD2_GPIO_Port, LOAD2_Pin, GPIO_PIN_SET);
	
	/*HAL_GPIO_WritePin(LOAD2_GPIO_Port, LOAD2_Pin, GPIO_PIN_RESET);HAL_Delay(1);
	up7_matrix_SendAdd(decodeMode,0x00); //decoding
	up7_matrix_SendAdd(decodeMode,0x00); //decoding
	up7_matrix_SendAdd(decodeMode,0x00); //decoding
	up7_matrix_SendAdd(decodeMode,0x00); //decoding
	up7_matrix_SendAdd(decodeMode,0x00); //decoding
	up7_matrix_SendAdd(decodeMode,0x00); //decoding
	up7_matrix_SendAdd(decodeMode,0x00); //decoding
	HAL_GPIO_WritePin(LOAD2_GPIO_Port, LOAD2_Pin, GPIO_PIN_SET);
	HAL_Delay(1);*/
	HAL_GPIO_WritePin(LOAD2_GPIO_Port, LOAD2_Pin, GPIO_PIN_RESET);
	up7_matrix_SendAdd(intensity,LEDintensity); //intensity
	up7_matrix_SendAdd(intensity,LEDintensity); //intensity
	up7_matrix_SendAdd(intensity,LEDintensity); //intensity
	up7_matrix_SendAdd(intensity,LEDintensity); //intensity
	up7_matrix_SendAdd(intensity,LEDintensity); //intensity
	up7_matrix_SendAdd(intensity,LEDintensity); //intensity
	up7_matrix_SendAdd(intensity,LEDintensity); //intensity
	HAL_GPIO_WritePin(LOAD2_GPIO_Port, LOAD2_Pin, GPIO_PIN_SET);
	
	HAL_GPIO_WritePin(LOAD2_GPIO_Port, LOAD2_Pin, GPIO_PIN_RESET);
	up7_matrix_SendAdd(scanLimit,0x07); //all digits on
	up7_matrix_SendAdd(scanLimit,0x07); //all digits on
	up7_matrix_SendAdd(scanLimit,0x07); //all digits on
	up7_matrix_SendAdd(scanLimit,0x07); //all digits on
	up7_matrix_SendAdd(scanLimit,0x07); //all digits on
	up7_matrix_SendAdd(scanLimit,0x07); //all digits on
	up7_matrix_SendAdd(scanLimit,0x07); //all digits on
	HAL_GPIO_WritePin(LOAD2_GPIO_Port, LOAD2_Pin, GPIO_PIN_SET);
	
	/*HAL_GPIO_WritePin(LOAD2_GPIO_Port, LOAD2_Pin, GPIO_PIN_RESET);HAL_Delay(1);
	up7_matrix_SendAdd(disTest,0x00); //display test off
	up7_matrix_SendAdd(disTest,0x00); //display test off
	up7_matrix_SendAdd(disTest,0x00); //display test off
	up7_matrix_SendAdd(disTest,0x00); //display test off
	up7_matrix_SendAdd(disTest,0x00); //display test off
	up7_matrix_SendAdd(disTest,0x00); //display test off
	up7_matrix_SendAdd(disTest,0x00); //display test off
	HAL_GPIO_WritePin(LOAD2_GPIO_Port, LOAD2_Pin, GPIO_PIN_SET);
	HAL_Delay(1);*/
	
	
	
}



void chinhdosang(void)
	{
		uint32_t i;
		
		
		HAL_GPIO_WritePin(LOAD2_GPIO_Port, LOAD2_Pin, GPIO_PIN_RESET);
		up7_matrix_SendAdd(intensity,LEDintensity); //intensity
		up7_matrix_SendAdd(intensity,LEDintensity); //intensity
		up7_matrix_SendAdd(intensity,LEDintensity); //intensity
		up7_matrix_SendAdd(intensity,LEDintensity); //intensity
		up7_matrix_SendAdd(intensity,LEDintensity); //intensity
		up7_matrix_SendAdd(intensity,LEDintensity); //intensity
		up7_matrix_SendAdd(intensity,LEDintensity); //intensity
		HAL_GPIO_WritePin(LOAD2_GPIO_Port, LOAD2_Pin, GPIO_PIN_SET);
		
		//line2_matrix_init();
		HAL_GPIO_WritePin(LED_LOAD_GPIO_Port, LED_LOAD_Pin, GPIO_PIN_RESET);
		line2_matrix_SendAdd(intensity,LEDintensity); //intensity
		line2_matrix_SendAdd(intensity,LEDintensity); //intensity
		line2_matrix_SendAdd(intensity,LEDintensity); //intensity
		line2_matrix_SendAdd(intensity,LEDintensity); //intensity
		line2_matrix_SendAdd(intensity,LEDintensity); //intensity
		line2_matrix_SendAdd(intensity,LEDintensity); //intensity
		HAL_GPIO_WritePin(LED_LOAD_GPIO_Port, LED_LOAD_Pin, GPIO_PIN_SET);
		for (i=0;i<1000;i++);
	}

#endif


