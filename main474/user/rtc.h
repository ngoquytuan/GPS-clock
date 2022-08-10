#ifndef __RTC
#define __RTC

//Realtime address -----------------------


//define for DS3231
#define DS_SECOND_REG 0
#define DS_MIN_REG 		1
#define DS_HOUR_REG 	2
#define DS_DAY_REG 		3
#define DS_DATE_REG 	4
#define DS_MONTH_REG 	5
#define DS_YEAR_REG 	6

void UpdateRtcTime(void);
void BCD_Decoder(void);
void laythoigian(void);
void ghids(unsigned char add, unsigned char dat);
extern char time[7];

#endif
