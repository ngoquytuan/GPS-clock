#include <time.h>
#include "main.h"


extern time_t timenow;
extern I2C_HandleTypeDef hi2c3;
extern struct tm currtime;
extern uint8_t rtc_pps;
extern const time_t built_time;
extern uint8_t just_set_time_flag;
/*
Cap nhat thoi gian vao RTC
*/
void RTC_Update(void)
	{
		//ghids(14,0);//1HZ out SQW
		ghids(DS_SECOND_REG,seconds);
		ghids(DS_MIN_REG,minutes);
		ghids(DS_HOUR_REG,hours);
		
		ghids(DS_DATE_REG,days);
		ghids(DS_MONTH_REG,months);
		ghids(DS_YEAR_REG,years);		
		
		just_set_time_flag = 1;
		laythoigian();
		update_display();
	}
	
void RTCInit(void)
{
	//Neu ko co xung RTC => RTC het pin hoac hong!
	if(rtc_pps == 0) ghids(14,0);//1HZ out SQW
	//printf("RTC pps: %d\r\n",rtc_pps);
	laythoigian();
		
	currtime.tm_year = 100+ years;
	currtime.tm_mon  = months-1;
	currtime.tm_mday = days;
	currtime.tm_sec  = seconds;
	currtime.tm_min  = minutes;
	currtime.tm_hour = hours;
	timenow = mktime(&currtime);
	//RTC neu bi loi	
	if(timenow < built_time) 
		{
			timeinfo = localtime( &built_time );
			asctime(timeinfo);
			days    = timeinfo->tm_mday;
			months  = timeinfo->tm_mon +1;
			years   = timeinfo->tm_year-100;
			hours   = timeinfo->tm_hour;
			minutes = timeinfo->tm_min;
			seconds = timeinfo->tm_sec+1;
			#ifdef DebugEnable
			printf("RTC loi!!!\r\n");
			#endif 
			RTC_Update();
			timenow = built_time ;
		}
		#ifdef DebugEnable
			printf("Time :%dh%dm%ds;%d %d/%d/%d; LED intensity : %d\r\n",hours,minutes,seconds,ds3231_reg[3],days,months,years, LEDintensity);
		#endif 
}
//----------------------Phan giao tiep i2c-thoi gian thuc ---------------------//
//Phai pull up SDA SCL

/*
Doc thoi gian tu RTC
*/
void laythoigian(void)
{
	unsigned char i2c_rv[19];
	if( HAL_I2C_Mem_Read(&hi2c3,0x68<<1,0,I2C_MEMADD_SIZE_8BIT,i2c_rv,19,1000) == HAL_ERROR) 
		{
			slave_clock.rtc_status = NO_RTC;
			//printf("NO RTC");
			return;
		}
	
	for(char x=0;x<7;x++) ds3231_reg[x]=(i2c_rv[x] & 0x0f) + (i2c_rv[x]>>4)*10;
	
	seconds = ds3231_reg[0];
	hours   = ds3231_reg[2];
	minutes = ds3231_reg[1];
	
	days		= ds3231_reg[4];
	months  = ds3231_reg[5]; 
	years   = ds3231_reg[6];
		
		//Tinh toan timenow
	currtime.tm_year = 100+ years;
	currtime.tm_mon  = months-1;
	currtime.tm_mday = days;
	currtime.tm_sec  = seconds;
	currtime.tm_min  = minutes;
	currtime.tm_hour = hours;
	timenow = mktime(&currtime);
}
//----------------------Phan giao tiep i2c-thoi gian thuc ---------------------//


unsigned char BCD_Encoder(unsigned char temp)
{
	return ((temp/10)<<4)|(temp%10);
}
void ghids(unsigned char add, unsigned char dat)
{
	unsigned char i2c_write[1];
	i2c_write[0] = BCD_Encoder(dat);
	HAL_I2C_Mem_Write(&hi2c3,0x68<<1,add,I2C_MEMADD_SIZE_8BIT,i2c_write,1,1000); 
}

