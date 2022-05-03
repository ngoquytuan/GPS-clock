#include "main.h"

void MAX7219_SendByte (unsigned char dat)
{
unsigned char i;
for (i=0;i<8;i++)
{
if((dat&0x80)==0x80) HAL_GPIO_WritePin(LED_DIN_GPIO_Port, LED_DIN_Pin, GPIO_PIN_SET);//GPIO_SetBits(GPIOA, MAX7219_DIN);
else HAL_GPIO_WritePin(LED_DIN_GPIO_Port, LED_DIN_Pin, GPIO_PIN_RESET);//GPIO_ResetBits(GPIOA, MAX7219_DIN);

HAL_GPIO_WritePin(LED_CLK_GPIO_Port, LED_CLK_Pin, GPIO_PIN_RESET);//GPIO_ResetBits(GPIOA, MAX7219_CLK);
HAL_Delay(1);
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
MAX7219_SendAddrDat (0x0c,0x01); //normal operation
MAX7219_SendAddrDat (0x0a,0x01); //intensity
MAX7219_SendAddrDat (0x0b,0x07); //all digits on
MAX7219_SendAddrDat (0x09,0x00); //decoding
MAX7219_SendAddrDat (0x0f,0x00); //display test off
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

