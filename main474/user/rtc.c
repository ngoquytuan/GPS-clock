#include "stm32g4xx_hal.h"
#include <string.h>
#include <stdio.h>
#include "rtc.h"

extern uint8_t days,months,years,hours,minutes,seconds ;

//----------------------Phan giao tiep i2c-thoi gian thuc ---------------------//
unsigned char i2c_write[1];
char time[7];
unsigned char i2c_rv[19];

extern I2C_HandleTypeDef hi2c2;

void UpdateRtcTime(void)
{
	ghids(14,0);//1HZ out SQW
  ghids(DS_SECOND_REG,seconds);
  ghids(DS_MIN_REG,minutes);
  ghids(DS_HOUR_REG,hours);
				
  ghids(DS_DATE_REG,days);
  ghids(DS_MONTH_REG,months);
  ghids(DS_YEAR_REG,years);
}
//------------------------------------------------------
void BCD_Decoder(void)
{
	for(char x=0;x<7;x++) time[x]=(i2c_rv[x] & 0x0f) + (i2c_rv[x]>>4)*10;
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
	
	hours   = time[DS_HOUR_REG];
	minutes = time[DS_MIN_REG];
	seconds = time[DS_SECOND_REG];
	days		= time[DS_DATE_REG];
	months  = time[DS_MONTH_REG]; 
	years   = time[DS_YEAR_REG];
}
//=======================================================
void ghids(unsigned char add, unsigned char dat)
{
	i2c_write[0] = BCD_Encoder(dat);
	HAL_I2C_Mem_Write(&hi2c2,0x68<<1,add,I2C_MEMADD_SIZE_8BIT,i2c_write,1,1000); 
}
