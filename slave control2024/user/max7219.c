#include "main.h"
#include <string.h>
#include "mydefines.h"

uint8_t LEDintensity = 15;

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
		//HAL_Delay(1);
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

//void MAX7219_Clear(void)
//{
//unsigned char i;
//	for(i=8;i>0;i--)
//	MAX7219_SendAddrDat(i,0x00);
//}


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
		if     (slave_clock.sync_status == RED) MAX7219_SendAddrDat(0x07,0xFB);//all + local
		else if(slave_clock.sync_status == GREEN)   MAX7219_SendAddrDat(0x07,0xFD);// all + GREEN
		else MAX7219_SendAddrDat(0x07,0xFF);// all + GREEN + local
		#endif
		#ifdef SLAVE_WALL
		if(slave_clock.sync_status == RED) MAX7219_SendAddrDat(0x07,0xF6);//all + local
		else if(slave_clock.sync_status == GREEN)   MAX7219_SendAddrDat(0x07,0xF8);//all + gps
		else MAX7219_SendAddrDat(0x07,0xFE);// all + gps + local
		#endif
	}
	else
	{
		blynk =0;
		#ifdef SLAVE_CONSOLE
		if     (slave_clock.sync_status == RED) MAX7219_SendAddrDat(0x07,0x03);// off all + local
		else if(slave_clock.sync_status == GREEN)   MAX7219_SendAddrDat(0x07,0x05);// off all + GREEN
		else MAX7219_SendAddrDat(0x07,0x07);// off all + GREEN + local
		#endif
		#ifdef SLAVE_WALL
		if(slave_clock.sync_status == RED) MAX7219_SendAddrDat(0x07,0x06);//off all + local
		else if(slave_clock.sync_status == GREEN)   MAX7219_SendAddrDat(0x07,0x08);//off all + GREEN
		else MAX7219_SendAddrDat(0x07,0x0E);// off all + GREEN + local
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
	
	LEDintensity++;
	if(LEDintensity > 15) LEDintensity = 3;
	if(LEDintensity < 3) LEDintensity = 3;
	MAX7219_SendAddrDat (intensity,LEDintensity); //intensity
	#ifdef SLAVE_WALL
	MAX7219_SendAddrDat2 (intensity,LEDintensity); //intensity
	#endif
	//HAL_Delay(1000);
}

//END of led 7 seg
#endif
/*----------------------------------------------------------------------------------------------------------------*/
#ifdef SLAVE_MATRIX


#define LOAD2_LOW    HAL_GPIO_WritePin(LOAD2_GPIO_Port, LOAD2_Pin, GPIO_PIN_RESET);
#define LOAD2_HIGH   HAL_GPIO_WritePin(LOAD2_GPIO_Port, LOAD2_Pin, GPIO_PIN_SET);


// Standard font - variable spacing
static const uint8_t _sysfont[] = {
    5, 62, 81, 73, 69, 62,  // 48 - '0'
    3, 0, 4, 2, 127, 0,  // 49 - '1'
    5, 113, 73, 73, 73, 70,  // 50 - '2'
    5, 65, 73, 73, 73, 54,  // 51 - '3'
    5, 15, 8, 8, 8, 127,  // 52 - '4'
    5, 79, 73, 73, 73, 49,  // 53 - '5'
    5, 62, 73, 73, 73, 48,  // 54 - '6'
    5, 3, 1, 1, 1, 127,  // 55 - '7'
    5, 54, 73, 73, 73, 54,  // 56 - '8'
    5, 6, 73, 73, 73, 62,  // 57 - '9'
};
static const uint16_t offsetTable[] = {1, 7, 13, 19, 25, 31, 37, 43, 49, 55};


uint16_t setOffset(uint8_t num) {
    return (num <= 9) ? offsetTable[num] : 0;
}




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

uint8_t Buffer_7up[8][7];
uint8_t Buffer_5down[8][5];








void load_line1(uint8_t dis_date,uint8_t dis_month,uint8_t dis_year)
{ 
	
	const uint8_t *DateTens,*DateOnes;
	
	DateTens  = _sysfont + setOffset(dis_date/10);
	DateOnes  = _sysfont + setOffset(dis_date%10);
	
	const uint8_t *MonthTens = _sysfont + setOffset(dis_month/10);
  const uint8_t *MonthOnes = _sysfont + setOffset(dis_month%10);
	
	const uint8_t *YearTens;
  const uint8_t *YearOnes;
	// to make year 20xx, make 2, 0 digit first!
  YearTens = _sysfont + setOffset(2);
  YearOnes = _sysfont + setOffset(0);
	

	Buffer_7up[line1][mtrix6] = (((*DateTens)&BIT1)<<7) + (((*(DateTens+1))&BIT1) << 6) +(((*(DateTens+2))&BIT1) << 5) +(((*(DateTens+3))&BIT1) << 4) + (((*(DateTens+4))&BIT1) << 3) +0+ (((*DateOnes)&BIT1) << 1) +  ((*(DateOnes+1))&BIT1);
	Buffer_7up[line2][mtrix6] = (((*DateTens)&BIT2)<<6) + (((*(DateTens+1))&BIT2) << 5) +(((*(DateTens+2))&BIT2) << 4) +(((*(DateTens+3))&BIT2) << 3) + (((*(DateTens+4))&BIT2) << 2) +0+ (((*DateOnes)&BIT2) << 0) + (((*(DateOnes+1))&BIT2)>>1);
	Buffer_7up[line3][mtrix6] = (((*DateTens)&BIT3)<<5) + (((*(DateTens+1))&BIT3) << 4) +(((*(DateTens+2))&BIT3) << 3) +(((*(DateTens+3))&BIT3) << 2) + (((*(DateTens+4))&BIT3) << 1) +0+ (((*DateOnes)&BIT3) >> 1) + (((*(DateOnes+1))&BIT3)>>2);
	Buffer_7up[line4][mtrix6] = (((*DateTens)&BIT4)<<4) + (((*(DateTens+1))&BIT4) << 3) +(((*(DateTens+2))&BIT4) << 2) +(((*(DateTens+3))&BIT4) << 1) + (((*(DateTens+4))&BIT4) << 0) +0+ (((*DateOnes)&BIT4) >> 2) + (((*(DateOnes+1))&BIT4)>>3);
	Buffer_7up[line5][mtrix6] = (((*DateTens)&BIT5)<<3) + (((*(DateTens+1))&BIT5) << 2) +(((*(DateTens+2))&BIT5) << 1) +(((*(DateTens+3))&BIT5) << 0) + (((*(DateTens+4))&BIT5) >> 1) +0+ (((*DateOnes)&BIT5) >> 3) + (((*(DateOnes+1))&BIT5)>>4);
	Buffer_7up[line6][mtrix6] = (((*DateTens)&BIT6)<<2) + (((*(DateTens+1))&BIT6) << 1) +(((*(DateTens+2))&BIT6) << 0) +(((*(DateTens+3))&BIT6) >> 1) + (((*(DateTens+4))&BIT6) >> 2) +0+ (((*DateOnes)&BIT6) >> 4) + (((*(DateOnes+1))&BIT6)>>5);
	Buffer_7up[line7][mtrix6] = (((*DateTens)&BIT7)<<1) + (((*(DateTens+1))&BIT7) << 0) +(((*(DateTens+2))&BIT7) >> 1) +(((*(DateTens+3))&BIT7) >> 2) + (((*(DateTens+4))&BIT7) >> 3) +0+ (((*DateOnes)&BIT7) >> 5) + (((*(DateOnes+1))&BIT7)>>6);
	Buffer_7up[line8][mtrix6] = (((*DateTens)&BIT8)<<0) + (((*(DateTens+1))&BIT8) >> 1) +(((*(DateTens+2))&BIT8) >> 2) +(((*(DateTens+3))&BIT8) >> 3) + (((*(DateTens+4))&BIT8) >> 4) +0+ (((*DateOnes)&BIT8) >> 6) + (((*(DateOnes+1))&BIT8)>>7);
	
	Buffer_7up[line1][mtrix7] = (((*(DateOnes+2))&BIT1)<<7)+ (((*(DateOnes+3))&BIT1) << 6)+(((*(DateOnes+4))&BIT1) << 5)    +0							+0  						+0+ (((*MonthTens)&BIT1) << 1) +  ((*(MonthTens+1))&BIT1);
	Buffer_7up[line2][mtrix7] = (((*(DateOnes+2))&BIT2)<<6)+ (((*(DateOnes+3))&BIT2) << 5)+(((*(DateOnes+4))&BIT2) << 4)		+0							+0							+0+ (((*MonthTens)&BIT2) << 0) + (((*(MonthTens+1))&BIT2)>>1);
	Buffer_7up[line3][mtrix7] = (((*(DateOnes+2))&BIT3)<<5)+ (((*(DateOnes+3))&BIT3) << 4)+(((*(DateOnes+4))&BIT3) << 3)		+0							+0							+0+ (((*MonthTens)&BIT3) >> 1) + (((*(MonthTens+1))&BIT3)>>2);
	Buffer_7up[line4][mtrix7] = (((*(DateOnes+2))&BIT4)<<4)+ (((*(DateOnes+3))&BIT4) << 3)+(((*(DateOnes+4))&BIT4) << 2)		+0							+0							+0+ (((*MonthTens)&BIT4) >> 2) + (((*(MonthTens+1))&BIT4)>>3);
	Buffer_7up[line5][mtrix7] = (((*(DateOnes+2))&BIT5)<<3)+ (((*(DateOnes+3))&BIT5) << 2)+(((*(DateOnes+4))&BIT5) << 1)		+0							+0  					  +0+ (((*MonthTens)&BIT5) >> 3) + (((*(MonthTens+1))&BIT5)>>4);
	Buffer_7up[line6][mtrix7] = (((*(DateOnes+2))&BIT6)<<2)+ (((*(DateOnes+3))&BIT6) << 1)+(((*(DateOnes+4))&BIT6) << 0)		+0							+0							+0+ (((*MonthTens)&BIT6) >> 4) + (((*(MonthTens+1))&BIT6)>>5);
	Buffer_7up[line7][mtrix7] = (((*(DateOnes+2))&BIT7)<<1)+ (((*(DateOnes+3))&BIT7) << 0)+(((*(DateOnes+4))&BIT7) >> 1)		+0							+8  						+0+ (((*MonthTens)&BIT7) >> 5) + (((*(MonthTens+1))&BIT7)>>6);
	Buffer_7up[line8][mtrix7] = (((*(DateOnes+2))&BIT8)<<0)+ (((*(DateOnes+3))&BIT8) >> 1)+(((*(DateOnes+4))&BIT8) >> 2)		+0							+0  						+0+ (((*MonthTens)&BIT8) >> 6) + (((*(MonthTens+1))&BIT8)>>7);
	
	Buffer_7up[line1][mtrix8] = (((*(MonthTens+2))&0x01)<<7) + (((*(MonthTens+3))&0x01) << 6) +(((*(MonthTens+4))&0x01) << 5) +((0&0x01) << 4) + (((*MonthOnes)&0x01) << 3) +(((*(MonthOnes+1))&0x01) << 2)+ (((*(MonthOnes+2))&0x01) << 1) +  ((*(MonthOnes+3))&0x01);
	Buffer_7up[line2][mtrix8] = (((*(MonthTens+2))&0x02)<<6) + (((*(MonthTens+3))&0x02) << 5) +(((*(MonthTens+4))&0x02) << 4) +((0&0x02) << 3) + (((*MonthOnes)&0x02) << 2) +(((*(MonthOnes+1))&0x02) << 1)+ (((*(MonthOnes+2))&0x02) << 0) + (((*(MonthOnes+3))&0x02)>>1);
	Buffer_7up[line3][mtrix8] = (((*(MonthTens+2))&0x04)<<5) + (((*(MonthTens+3))&0x04) << 4) +(((*(MonthTens+4))&0x04) << 3) +((0&0x04) << 2) + (((*MonthOnes)&0x04) << 1) +(((*(MonthOnes+1))&0x04) << 0)+ (((*(MonthOnes+2))&0x04) >> 1) + (((*(MonthOnes+3))&0x04)>>2);
	Buffer_7up[line4][mtrix8] = (((*(MonthTens+2))&0x08)<<4) + (((*(MonthTens+3))&0x08) << 3) +(((*(MonthTens+4))&0x08) << 2) +((0&0x08) << 1) + (((*MonthOnes)&0x08) << 0) +(((*(MonthOnes+1))&0x08) >> 1)+ (((*(MonthOnes+2))&0x08) >> 2) + (((*(MonthOnes+3))&0x08)>>3);
	Buffer_7up[line5][mtrix8] = (((*(MonthTens+2))&0x10)<<3) + (((*(MonthTens+3))&0x10) << 2) +(((*(MonthTens+4))&0x10) << 1) +((0&0x10) << 0) + (((*MonthOnes)&0x10) >> 1) +(((*(MonthOnes+1))&0x10) >> 2)+ (((*(MonthOnes+2))&0x10) >> 3) + (((*(MonthOnes+3))&0x10)>>4);
	Buffer_7up[line6][mtrix8] = (((*(MonthTens+2))&0x20)<<2) + (((*(MonthTens+3))&0x20) << 1) +(((*(MonthTens+4))&0x20) << 0) +((0&0x20) >> 1) + (((*MonthOnes)&0x20) >> 2) +(((*(MonthOnes+1))&0x20) >> 3)+ (((*(MonthOnes+2))&0x20) >> 4) + (((*(MonthOnes+3))&0x20)>>5);
	Buffer_7up[line7][mtrix8] = (((*(MonthTens+2))&0x40)<<1) + (((*(MonthTens+3))&0x40) << 0) +(((*(MonthTens+4))&0x40) >> 1) +((0&0x40) >> 2) + (((*MonthOnes)&0x40) >> 3) +(((*(MonthOnes+1))&0x40) >> 4)+ (((*(MonthOnes+2))&0x40) >> 5) + (((*(MonthOnes+3))&0x40)>>6);
	Buffer_7up[line8][mtrix8] = (((*(MonthTens+2))&0x80)<<0) + (((*(MonthTens+3))&0x80) >> 1) +(((*(MonthTens+4))&0x80) >> 2) +((0&0x80) >> 3) + (((*MonthOnes)&0x80) >> 4) +(((*(MonthOnes+1))&0x80) >> 5)+ (((*(MonthOnes+2))&0x80) >> 6) + (((*(MonthOnes+3))&0x80)>>7);
	
	Buffer_7up[line1][mtrix9] = (((*(MonthOnes+4))&0x01)<<7) + ((0&0x01) << 6) +((0&0x01) << 5) +((0&0x01) << 4) + (((*YearTens)&0x01) << 3) +(((*(YearTens+1))&0x01) << 2)+ (((*(YearTens+2)) &0x01) << 1) +  ((*(YearTens+3))&0x01);
	Buffer_7up[line2][mtrix9] = (((*(MonthOnes+4))&0x02)<<6) + ((0&0x02) << 5) +((0&0x02) << 4) +((0&0x02) << 3) + (((*YearTens)&0x02) << 2) +(((*(YearTens+1))&0x02) << 1)+ (((*(YearTens+2)) &0x02) << 0) + (((*(YearTens+3))&0x02)>>1);
	Buffer_7up[line3][mtrix9] = (((*(MonthOnes+4))&0x04)<<5) + ((0&0x04) << 4) +00              +((0&0x04) << 2) + (((*YearTens)&0x04) << 1) +(((*(YearTens+1))&0x04) << 0)+ (((*(YearTens+2)) &0x04) >> 1) + (((*(YearTens+3))&0x04)>>2);
	Buffer_7up[line4][mtrix9] = (((*(MonthOnes+4))&0x08)<<4) + ((0&0x08) << 3) +((0&0x08) << 2) +((0&0x08) << 1) + (((*YearTens)&0x08) << 0) +(((*(YearTens+1))&0x08) >> 1)+ (((*(YearTens+2)) &0x08) >> 2) + (((*(YearTens+3))&0x08)>>3);
	Buffer_7up[line5][mtrix9] = (((*(MonthOnes+4))&0x10)<<3) + ((0&0x10) << 2) +((0&0x10) << 1) +((0&0x10) << 0) + (((*YearTens)&0x10) >> 1) +(((*(YearTens+1))&0x10) >> 2)+ (((*(YearTens+2)) &0x10) >> 3) + (((*(YearTens+3))&0x10)>>4);
	Buffer_7up[line6][mtrix9] = (((*(MonthOnes+4))&0x20)<<2) + ((0&0x20) << 1) +((0&0x20) << 0) +((0&0x20) >> 1) + (((*YearTens)&0x20) >> 2) +(((*(YearTens+1))&0x20) >> 3)+ (((*(YearTens+2)) &0x20) >> 4) + (((*(YearTens+3))&0x20)>>5);
	Buffer_7up[line7][mtrix9] = (((*(MonthOnes+4))&0x40)<<1) + ((0&0x40) << 0) +32              +((0&0x40) >> 2) + (((*YearTens)&0x40) >> 3) +(((*(YearTens+1))&0x40) >> 4)+ (((*(YearTens+2)) &0x40) >> 5) + (((*(YearTens+3))&0x40)>>6);
	Buffer_7up[line8][mtrix9] = (((*(MonthOnes+4))&0x80)<<0) + ((0&0x80) >> 1) +((0&0x80) >> 2) +((0&0x80) >> 3) + (((*YearTens)&0x80) >> 4) +(((*(YearTens+1))&0x80) >> 5)+ (((*(YearTens+2)) &0x80) >> 6) + (((*(YearTens+3))&0x80)>>7);
	
	Buffer_7up[line1][mtrix10]= (((*(YearTens+4))&0x01)<<7) + ((0&0x01) << 6) +(((*YearOnes)&0x01) << 5) +(((*(YearOnes+1))&0x01) << 4) + (((*(YearOnes+2))&0x01) << 3) +(((*(YearOnes+3))&0x01) << 2)+ (((*(YearOnes+4))&0x01) << 1) + ( 0&0x01);
	Buffer_7up[line2][mtrix10]= (((*(YearTens+4))&0x02)<<6) + ((0&0x02) << 5) +(((*YearOnes)&0x02) << 4) +(((*(YearOnes+1))&0x02) << 3) + (((*(YearOnes+2))&0x02) << 2) +(((*(YearOnes+3))&0x02) << 1)+ (((*(YearOnes+4))&0x02) << 0) + ((0&0x02)>>1);
	Buffer_7up[line3][mtrix10]= (((*(YearTens+4))&0x04)<<5) + ((0&0x04) << 4) +(((*YearOnes)&0x04) << 3) +(((*(YearOnes+1))&0x04) << 2) + (((*(YearOnes+2))&0x04) << 1) +(((*(YearOnes+3))&0x04) << 0)+ (((*(YearOnes+4))&0x04) >> 1) + ((0&0x04)>>2);
	Buffer_7up[line4][mtrix10]= (((*(YearTens+4))&0x08)<<4) + ((0&0x08) << 3) +(((*YearOnes)&0x08) << 2) +(((*(YearOnes+1))&0x08) << 1) + (((*(YearOnes+2))&0x08) << 0) +(((*(YearOnes+3))&0x08) >> 1)+ (((*(YearOnes+4))&0x08) >> 2) + ((0&0x08)>>3);
	Buffer_7up[line5][mtrix10]= (((*(YearTens+4))&0x10)<<3) + ((0&0x10) << 2) +(((*YearOnes)&0x10) << 1) +(((*(YearOnes+1))&0x10) << 0) + (((*(YearOnes+2))&0x10) >> 1) +(((*(YearOnes+3))&0x10) >> 2)+ (((*(YearOnes+4))&0x10) >> 3) + ((0&0x10)>>4);
	Buffer_7up[line6][mtrix10]= (((*(YearTens+4))&0x20)<<2) + ((0&0x20) << 1) +(((*YearOnes)&0x20) << 0) +(((*(YearOnes+1))&0x20) >> 1) + (((*(YearOnes+2))&0x20) >> 2) +(((*(YearOnes+3))&0x20) >> 3)+ (((*(YearOnes+4))&0x20) >> 4) + ((0&0x20)>>5);
	Buffer_7up[line7][mtrix10]= (((*(YearTens+4))&0x40)<<1) + ((0&0x40) << 0) +(((*YearOnes)&0x40) >> 1) +(((*(YearOnes+1))&0x40) >> 2) + (((*(YearOnes+2))&0x40) >> 3) +(((*(YearOnes+3))&0x40) >> 4)+ (((*(YearOnes+4))&0x40) >> 5) + ((0&0x40)>>6);
	Buffer_7up[line8][mtrix10]= (((*(YearTens+4))&0x80)<<0) + ((0&0x80) >> 1) +(((*YearOnes)&0x80) >> 2) +(((*(YearOnes+1))&0x80) >> 3) + (((*(YearOnes+2))&0x80) >> 4) +(((*(YearOnes+3))&0x80) >> 5)+ (((*(YearOnes+4))&0x80) >> 6) + ((0&0x80)>>7);
	
	YearTens = _sysfont + setOffset(dis_year/10);
  YearOnes = _sysfont + setOffset(dis_year%10);
	Buffer_7up[line1][mtrix11]= (((*(YearTens+0))&0x01)<<7) + (((*(YearTens+1))&0x01) << 6) +(((*(YearTens+2))&0x01) << 5) +(((*(YearTens+3))&0x01) << 4) + (((*(YearTens+4))&0x01) << 3) +((0&0x01) << 2)+ (((*(YearOnes+0))&0x01) << 1) + ( (*(YearOnes+1))&0x01);
	Buffer_7up[line2][mtrix11]= (((*(YearTens+0))&0x02)<<6) + (((*(YearTens+1))&0x02) << 5) +(((*(YearTens+2))&0x02) << 4) +(((*(YearTens+3))&0x02) << 3) + (((*(YearTens+4))&0x02) << 2) +((0&0x02) << 1)+ (((*(YearOnes+0))&0x02) << 0) + (((*(YearOnes+1))&0x02)>>1);
	Buffer_7up[line3][mtrix11]= (((*(YearTens+0))&0x04)<<5) + (((*(YearTens+1))&0x04) << 4) +(((*(YearTens+2))&0x04) << 3) +(((*(YearTens+3))&0x04) << 2) + (((*(YearTens+4))&0x04) << 1) +((0&0x04) << 0)+ (((*(YearOnes+0))&0x04) >> 1) + (((*(YearOnes+1))&0x04)>>2);
	Buffer_7up[line4][mtrix11]= (((*(YearTens+0))&0x08)<<4) + (((*(YearTens+1))&0x08) << 3) +(((*(YearTens+2))&0x08) << 2) +(((*(YearTens+3))&0x08) << 1) + (((*(YearTens+4))&0x08) << 0) +((0&0x08) >> 1)+ (((*(YearOnes+0))&0x08) >> 2) + (((*(YearOnes+1))&0x08)>>3);
	Buffer_7up[line5][mtrix11]= (((*(YearTens+0))&0x10)<<3) + (((*(YearTens+1))&0x10) << 2) +(((*(YearTens+2))&0x10) << 1) +(((*(YearTens+3))&0x10) << 0) + (((*(YearTens+4))&0x10) >> 1) +((0&0x10) >> 2)+ (((*(YearOnes+0))&0x10) >> 3) + (((*(YearOnes+1))&0x10)>>4);
	Buffer_7up[line6][mtrix11]= (((*(YearTens+0))&0x20)<<2) + (((*(YearTens+1))&0x20) << 1) +(((*(YearTens+2))&0x20) << 0) +(((*(YearTens+3))&0x20) >> 1) + (((*(YearTens+4))&0x20) >> 2) +((0&0x20) >> 3)+ (((*(YearOnes+0))&0x20) >> 4) + (((*(YearOnes+1))&0x20)>>5);
	Buffer_7up[line7][mtrix11]= (((*(YearTens+0))&0x40)<<1) + (((*(YearTens+1))&0x40) << 0) +(((*(YearTens+2))&0x40) >> 1) +(((*(YearTens+3))&0x40) >> 2) + (((*(YearTens+4))&0x40) >> 3) +((0&0x40) >> 4)+ (((*(YearOnes+0))&0x40) >> 5) + (((*(YearOnes+1))&0x40)>>6);
	Buffer_7up[line8][mtrix11]= (((*(YearTens+0))&0x80)<<0) + (((*(YearTens+1))&0x80) >> 1) +(((*(YearTens+2))&0x80) >> 2) +(((*(YearTens+3))&0x80) >> 3) + (((*(YearTens+4))&0x80) >> 4) +((0&0x80) >> 5)+ (((*(YearOnes+0))&0x80) >> 6) + (((*(YearOnes+1))&0x80)>>7);
	
	Buffer_7up[line1][mtrix12]= (((*(YearOnes+2))&0x01)<<7) + (((*(YearOnes+3))&0x01) << 6) +(((*(YearOnes+4))&0x01) << 5);// +(((*(YearOnes+1))&0x01) << 4) + (((*(YearOnes+2))&0x01) << 3) +(((*(YearOnes+3))&0x01) << 2)+ (((*(YearOnes+4))&0x01) << 1) + ( 0&0x01);
	Buffer_7up[line2][mtrix12]= (((*(YearOnes+2))&0x02)<<6) + (((*(YearOnes+3))&0x02) << 5) +(((*(YearOnes+4))&0x02) << 4);// +(((*(YearOnes+1))&0x02) << 3) + (((*(YearOnes+2))&0x02) << 2) +(((*(YearOnes+3))&0x02) << 1)+ (((*(YearOnes+4))&0x02) << 0) + ((0&0x02)>>1);
	Buffer_7up[line3][mtrix12]= (((*(YearOnes+2))&0x04)<<5) + (((*(YearOnes+3))&0x04) << 4) +(((*(YearOnes+4))&0x04) << 3);
	Buffer_7up[line4][mtrix12]= (((*(YearOnes+2))&0x08)<<4) + (((*(YearOnes+3))&0x08) << 3) +(((*(YearOnes+4))&0x08) << 2);
	Buffer_7up[line5][mtrix12]= (((*(YearOnes+2))&0x10)<<3) + (((*(YearOnes+3))&0x10) << 2) +(((*(YearOnes+4))&0x10) << 1);
	Buffer_7up[line6][mtrix12]= (((*(YearOnes+2))&0x20)<<2) + (((*(YearOnes+3))&0x20) << 1) +(((*(YearOnes+4))&0x20) << 0);
	Buffer_7up[line7][mtrix12]= (((*(YearOnes+2))&0x40)<<1) + (((*(YearOnes+3))&0x40) << 0) +(((*(YearOnes+4))&0x40) >> 1);
	Buffer_7up[line8][mtrix12]= (((*(YearOnes+2))&0x80)<<0) + (((*(YearOnes+3))&0x80) >> 1) +(((*(YearOnes+4))&0x80) >> 2);
}

void load_line2(uint8_t dis_hour,uint8_t dis_min,uint8_t dis_sec,uint8_t dot)
{ 

	
	const uint8_t *temp,*temp1;
	const uint8_t *min,*min1;
	const uint8_t *sec,*sec1;
	
	
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
	
	Buffer_5down[line1][mtrix2] = (((*(temp1+2))&0x01)<<7)+ (((*(temp1+3))&0x01) << 6)+(((*(temp1+4))&0x01) << 5)   +0							+0   						  +0+ (((*min)&0x01) << 1) +  ((*(min+1))&0x01);
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
	
	line2_matrix_SendAdd(shut_down,0x01); //normal operation
	line2_matrix_SendAdd(shut_down,0x01); //normal operation
	line2_matrix_SendAdd(shut_down,0x01); //normal operation
	line2_matrix_SendAdd(shut_down,0x01); //normal operation
	line2_matrix_SendAdd(shut_down,0x01); //normal operation
	//line2_matrix_SendAdd(shut_down,0x01); //normal operation
	HAL_GPIO_WritePin(LED_LOAD_GPIO_Port, LED_LOAD_Pin, GPIO_PIN_SET);

	HAL_GPIO_WritePin(LED_LOAD_GPIO_Port, LED_LOAD_Pin, GPIO_PIN_RESET);HAL_Delay(1);
	line2_matrix_SendAdd(decodeMode,0x00); //decoding
	line2_matrix_SendAdd(decodeMode,0x00); //decoding
	line2_matrix_SendAdd(decodeMode,0x00); //decoding
	line2_matrix_SendAdd(decodeMode,0x00); //decoding
	line2_matrix_SendAdd(decodeMode,0x00); //decoding
	HAL_GPIO_WritePin(LED_LOAD_GPIO_Port, LED_LOAD_Pin, GPIO_PIN_SET);
	
	HAL_GPIO_WritePin(LED_LOAD_GPIO_Port, LED_LOAD_Pin, GPIO_PIN_RESET);
	
	line2_matrix_SendAdd(intensity,LEDintensity); //intensity
	line2_matrix_SendAdd(intensity,LEDintensity); //intensity
	line2_matrix_SendAdd(intensity,LEDintensity); //intensity
	line2_matrix_SendAdd(intensity,LEDintensity); //intensity
	line2_matrix_SendAdd(intensity,LEDintensity); //intensity
	//line2_matrix_SendAdd(intensity,LEDintensity); //intensity
	HAL_GPIO_WritePin(LED_LOAD_GPIO_Port, LED_LOAD_Pin, GPIO_PIN_SET);
	
	HAL_GPIO_WritePin(LED_LOAD_GPIO_Port, LED_LOAD_Pin, GPIO_PIN_RESET);
	
	line2_matrix_SendAdd(scanLimit,0x07); //all digits on
	line2_matrix_SendAdd(scanLimit,0x07); //all digits on
	line2_matrix_SendAdd(scanLimit,0x07); //all digits on
	line2_matrix_SendAdd(scanLimit,0x07); //all digits on
	line2_matrix_SendAdd(scanLimit,0x07); //all digits on
	//line2_matrix_SendAdd(scanLimit,0x07); //all digits on
	HAL_GPIO_WritePin(LED_LOAD_GPIO_Port, LED_LOAD_Pin, GPIO_PIN_SET);
	
	HAL_GPIO_WritePin(LED_LOAD_GPIO_Port, LED_LOAD_Pin, GPIO_PIN_RESET);HAL_Delay(1);
	line2_matrix_SendAdd(disTest,0x00); //display test off
	line2_matrix_SendAdd(disTest,0x00); //display test off
	line2_matrix_SendAdd(disTest,0x00); //display test off
	line2_matrix_SendAdd(disTest,0x00); //display test off
	line2_matrix_SendAdd(disTest,0x00); //display test off
	HAL_GPIO_WritePin(LED_LOAD_GPIO_Port, LED_LOAD_Pin, GPIO_PIN_SET);
	
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
	
	HAL_GPIO_WritePin(LOAD2_GPIO_Port, LOAD2_Pin, GPIO_PIN_RESET);
	up7_matrix_SendAdd(decodeMode,0x00); //decoding
	up7_matrix_SendAdd(decodeMode,0x00); //decoding
	up7_matrix_SendAdd(decodeMode,0x00); //decoding
	up7_matrix_SendAdd(decodeMode,0x00); //decoding
	up7_matrix_SendAdd(decodeMode,0x00); //decoding
	up7_matrix_SendAdd(decodeMode,0x00); //decoding
	up7_matrix_SendAdd(decodeMode,0x00); //decoding
	HAL_GPIO_WritePin(LOAD2_GPIO_Port, LOAD2_Pin, GPIO_PIN_SET);
	
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
	
	HAL_GPIO_WritePin(LOAD2_GPIO_Port, LOAD2_Pin, GPIO_PIN_RESET);
	up7_matrix_SendAdd(disTest,0x00); //display test off
	up7_matrix_SendAdd(disTest,0x00); //display test off
	up7_matrix_SendAdd(disTest,0x00); //display test off
	up7_matrix_SendAdd(disTest,0x00); //display test off
	up7_matrix_SendAdd(disTest,0x00); //display test off
	up7_matrix_SendAdd(disTest,0x00); //display test off
	up7_matrix_SendAdd(disTest,0x00); //display test off
	HAL_GPIO_WritePin(LOAD2_GPIO_Port, LOAD2_Pin, GPIO_PIN_SET);
	
	
}



void chinhdosang(void)
	{
		//uint32_t i;
		
		LEDintensity++;
		if(LEDintensity > 15) LEDintensity =1;
		if(LEDintensity < 1) LEDintensity =1;
		
		//Luu bo sang moi vao bo nho!
		stm32g474flashEraseThenSave();
		
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
		//line2_matrix_SendAdd(intensity,LEDintensity); //intensity
		HAL_GPIO_WritePin(LED_LOAD_GPIO_Port, LED_LOAD_Pin, GPIO_PIN_SET);
		
	}

#endif


