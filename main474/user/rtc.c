#include "stm32g4xx_hal.h"
#include <string.h>
#include <stdio.h>
#include "rtc.h"

extern uint8_t days,months,years,hours,minutes,seconds ;

//----------------------Phan giao tiep i2c-thoi gian thuc ---------------------//
unsigned char i2c_write[1];
char ds3231_reg[7];
unsigned char i2c_rv[19];

extern I2C_HandleTypeDef hi2c2;

void UpdateRtcTime(uint8_t ho,uint8_t min,uint8_t sec,uint8_t da,uint8_t mon,uint8_t yea)
{
	ghids(14,0);//1HZ out SQW
  ghids(DS_SECOND_REG,sec);
  ghids(DS_MIN_REG,min);
  ghids(DS_HOUR_REG,ho);
				
  ghids(DS_DATE_REG,da);
  ghids(DS_MONTH_REG,mon);
  ghids(DS_YEAR_REG,yea);
}
//------------------------------------------------------
void BCD_Decoder(void)
{
	for(char x=0;x<7;x++) ds3231_reg[x]=(i2c_rv[x] & 0x0f) + (i2c_rv[x]>>4)*10;
}
unsigned char BCD_Encoder(unsigned char temp)
{
	return ((temp/10)<<4)|(temp%10);
}
//=======================================================

void laythoigian(void)
{
	HAL_I2C_Mem_Read(&hi2c2,0x68<<1,0,I2C_MEMADD_SIZE_8BIT,i2c_rv,19,1000); //read time
	BCD_Decoder(); //chuyen doi
	
	hours   = ds3231_reg[DS_HOUR_REG];
	minutes = ds3231_reg[DS_MIN_REG];
	seconds = ds3231_reg[DS_SECOND_REG];
	days		= ds3231_reg[DS_DATE_REG];
	months  = ds3231_reg[DS_MONTH_REG]; 
	years   = ds3231_reg[DS_YEAR_REG];
}
//=======================================================
void ghids(unsigned char add, unsigned char dat)
{
	i2c_write[0] = BCD_Encoder(dat);
	HAL_I2C_Mem_Write(&hi2c2,0x68<<1,add,I2C_MEMADD_SIZE_8BIT,i2c_write,1,1000); 
}
