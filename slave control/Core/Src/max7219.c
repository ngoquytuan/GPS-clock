#include "main.h"
uint8_t LEDintensity;
uint8_t blynk = 0;
//uint8_t console_stt = GPS;

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
	MAX7219_SendAddrDat (intensity,0x01); //intensity
#endif
#ifdef SLAVE_WALL
	MAX7219_SendAddrDat (intensity,0x05); //intensity
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
unsigned char const LEDcode[]=
{
0x7e,0x30,0x6d,0x79,0x33,0x5b,0x5f,0x70,0x7f,0x7b, //0..9
0x77,0x1f,0x4e,0x3d,0x4f,0x47,0x67,0x3e,0xff //a..f,P,U,all_on
};

void MAX7219_DisplayInt (long int val)
{
long int a;
a=val;
	MAX7219_SendAddrDat(0x08,LEDcode[1]);
//MAX7219_SendAddrDat(0x08,LEDcode[(a/10000000)]);
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
		MAX7219_SendAddrDat2 (intensity,0x02); //intensity
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
	//MAX7219_SendAddrDat(0x07,0xFF);// all + local + gps
	//MAX7219_SendAddrDat(0x07,0xFB);// all + local
	//MAX7219_SendAddrDat(0x07,0x03);// off all + local
	//MAX7219_SendAddrDat(0x07,0xFD);// all + gps
	//MAX7219_SendAddrDat(0x07,0x05);// off all + gps
	//slave_clock.sync_status = LOCAL;
	
	
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


