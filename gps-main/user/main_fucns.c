#include "main.h"
#include <stdio.h>
#include <string.h>
#include "lcd.h"

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern I2C_HandleTypeDef hi2c2;
extern uint8_t u1Timeout;
extern uint8_t u2Timeout;
extern uint8_t u3Timeout;

extern GPSClock_t GPS_clock;

extern uint8_t Rx1Buffer[RX1BUFFERSIZE];
extern uint8_t Rx2Buffer[RX2BUFFERSIZE];
extern uint8_t Rx3Buffer[RX3BUFFERSIZE];

extern uint8_t updateLCD;
extern uint32_t t,t1,t2,t3,t4;
uint8_t days = 0;
uint8_t months = 0;
uint8_t years = 0;
uint8_t hours = 0;
uint8_t minutes = 0;
uint8_t seconds = 0;

unsigned char i2c_write[1];
char ds3231_reg[7],giaycu,ngay_al,thang_al,nhietdo,nhietdole;
unsigned char i2c_rv[19];

void check_uart(void)
{
	uint8_t i,j;
	
	if(u1Timeout == 1) 
			{
				u1_halt   = 0;//U1 ok
				u1Timeout = 0;
				//t1= fractionOfSecond;
				//printf("U1 %s; \r\n",Rx1Buffer);
				//HAL_UART_Transmit(&huart3, Rx1Buffer, 80, 100);
				if(Rx1Buffer[0] == '$')
					{
						if((Rx1Buffer[3] == 'R') && (Rx1Buffer[4] == 'M') &&(Rx1Buffer[5] == 'C')&&(Rx1Buffer[13] == '.') )
							{
								GPS_clock.gps1_time[0] = Rx1Buffer[7+0];
								GPS_clock.gps1_time[1] = Rx1Buffer[7+1];
								GPS_clock.gps1_time[2] = Rx1Buffer[7+2];
								GPS_clock.gps1_time[3] = Rx1Buffer[7+3];
								GPS_clock.gps1_time[4] = Rx1Buffer[7+4];
								GPS_clock.gps1_time[5] = Rx1Buffer[7+5];
								GPS_clock.gps1_valid   = Rx1Buffer[7+10];
								TimeMessage[16] = GPS_clock.gps1_valid;
								hours   = 10*(GPS_clock.gps1_time[0] -'0') + GPS_clock.gps1_time[1] - '0';
								minutes = 10*(GPS_clock.gps1_time[2] -'0') + GPS_clock.gps1_time[3] - '0';
								seconds = 10*(GPS_clock.gps1_time[4] -'0') + GPS_clock.gps1_time[5] - '0';
								//Dem 9 so ',' de tim ngay thang nam
							  i = 0;j=0;
								while((i<9) && (j<100))
								{
									if(Rx1Buffer[j++] == ',') i++;
								}
								if((i == 9) && (Rx1Buffer[j+6] ==','))//9 dau ','
								{
									GPS_clock.gps1_date[0] = Rx1Buffer[j];
									GPS_clock.gps1_date[1] = Rx1Buffer[j+1];
									GPS_clock.gps1_date[2] = Rx1Buffer[j+2];
									GPS_clock.gps1_date[3] = Rx1Buffer[j+3];
									GPS_clock.gps1_date[4] = Rx1Buffer[j+4];
									GPS_clock.gps1_date[5] = Rx1Buffer[j+5];
									if( GPS_clock.gps1_valid == 'A' ) GPS_clock.gps1_lock = 1;
									days   = 10*(GPS_clock.gps1_date[0] -'0') + GPS_clock.gps1_date[1] - '0';
									months = 10*(GPS_clock.gps1_date[2] -'0') + GPS_clock.gps1_date[3] - '0';
									years  = 10*(GPS_clock.gps1_date[4] -'0') + GPS_clock.gps1_date[5] - '0';
									//Neu GPS locked => ban tin lay tu GPS1 gui di mach giao tiep
									if(GPS_clock.gps1_lock == 1)
									{
										TimeMessage[4] = GPS_clock.gps1_time[0];
										TimeMessage[5] = GPS_clock.gps1_time[1];
										TimeMessage[6] = GPS_clock.gps1_time[2];
										TimeMessage[7] = GPS_clock.gps1_time[3];
										TimeMessage[8] = GPS_clock.gps1_time[4];
										TimeMessage[9] = GPS_clock.gps1_time[5];
										TimeMessage[10] = GPS_clock.gps1_date[0];
										TimeMessage[11] = GPS_clock.gps1_date[1];
										TimeMessage[12] = GPS_clock.gps1_date[2];
										TimeMessage[13] = GPS_clock.gps1_date[3];
										TimeMessage[14] = GPS_clock.gps1_date[4];
										TimeMessage[15] = GPS_clock.gps1_date[5];
									}
									
									//HAL_UART_Transmit(&huart3, GPS_clock.gps1_date, 6, 100);
								  //HAL_UART_Transmit(&huart3, (uint8_t *)&" G1\r\n", 5, 100);
									//printf("%d:%d:%d %d/%d/%d\r\n",hours,minutes,seconds,days,months,years);
								}
								
							}
							
							//HAL_UART_Transmit(&huart3, GPS_clock.gps1_time, 6, 100);
							//HAL_UART_Transmit(&huart3, (uint8_t *)&" G1\r\n", 5, 100);
							//updateLCD = 1;
					}
				
				
				
//				if(GPS_clock.gps1_lock != 1)
//				{
//					updateLCD =1;
//				}
				
				huart1.pRxBuffPtr = (uint8_t *)Rx1Buffer;
				huart1.RxXferCount = RX1BUFFERSIZE;
				memset(Rx1Buffer,0,RX1BUFFERSIZE);
			  HAL_GPIO_WritePin(LED_GPS1_GPIO_Port, LED_GPS1_Pin, GPIO_PIN_SET);
			}
			
		if(u2Timeout == 1) 
			{
				u2Timeout = 0;
				u2_halt   = 0;//U2 ok
				//t2= fractionOfSecond;
				//printf("U2 %s; \r\n",Rx2Buffer);
				//HAL_UART_Transmit(&huart3, Rx2Buffer, 80, 100);
				if(Rx2Buffer[0] == '$')
					{
						if((Rx2Buffer[3] == 'R') && (Rx2Buffer[4] == 'M') &&(Rx2Buffer[5] == 'C')&&(Rx2Buffer[13] == '.') )
							{
								GPS_clock.gps2_time[0] = Rx2Buffer[7+0];
								GPS_clock.gps2_time[1] = Rx2Buffer[7+1];
								GPS_clock.gps2_time[2] = Rx2Buffer[7+2];
								GPS_clock.gps2_time[3] = Rx2Buffer[7+3];
								GPS_clock.gps2_time[4] = Rx2Buffer[7+4];
								GPS_clock.gps2_time[5] = Rx2Buffer[7+5];
								GPS_clock.gps2_valid   = Rx2Buffer[7+10];
								TimeMessage[17] = GPS_clock.gps2_valid;
								//Dem 9 so ',' de tim ngay thang nam
							  i = 0;j=0;
								while((i<9) && (j<100))
								{
									if(Rx2Buffer[j++] == ',') i++;
								}
								if((i == 9) && (Rx2Buffer[j+6] ==','))//9 dau ','
								{
									GPS_clock.gps2_date[0] = Rx2Buffer[j];
									GPS_clock.gps2_date[1] = Rx2Buffer[j+1];
									GPS_clock.gps2_date[2] = Rx2Buffer[j+2];
									GPS_clock.gps2_date[3] = Rx2Buffer[j+3];
									GPS_clock.gps2_date[4] = Rx2Buffer[j+4];
									GPS_clock.gps2_date[5] = Rx2Buffer[j+5];
									if( GPS_clock.gps2_valid == 'A' ) GPS_clock.gps2_lock = 1;
									//Neu GPS1 not lock, GPS2 locked => ban tin lay tu GPS2 gui di mach giao tiep
									if((GPS_clock.gps1_lock != 1) && (GPS_clock.gps2_lock == 1))
									{
										TimeMessage[4] = GPS_clock.gps2_time[0];
										TimeMessage[5] = GPS_clock.gps2_time[1];
										TimeMessage[6] = GPS_clock.gps2_time[2];
										TimeMessage[7] = GPS_clock.gps2_time[3];
										TimeMessage[8] = GPS_clock.gps2_time[4];
										TimeMessage[9] = GPS_clock.gps2_time[5];
										TimeMessage[10] = GPS_clock.gps2_date[0];
										TimeMessage[11] = GPS_clock.gps2_date[1];
										TimeMessage[12] = GPS_clock.gps2_date[2];
										TimeMessage[13] = GPS_clock.gps2_date[3];
										TimeMessage[14] = GPS_clock.gps2_date[4];
										TimeMessage[15] = GPS_clock.gps2_date[5];
										//Gui thoi gian den mach giao tiep
										HAL_UART_Transmit(&huart3, TimeMessage, 20, 100);
									}
									//HAL_UART_Transmit(&huart3, GPS_clock.gps2_date, 6, 100);
								  //HAL_UART_Transmit(&huart3, (uint8_t *)&" G2\r\n", 5, 100);
								}
							}
							//HAL_UART_Transmit(&huart3, GPS_clock.gps2_time, 6, 100);
							//HAL_UART_Transmit(&huart3, (uint8_t *)&" G2\r\n", 5, 100);
							//updateLCD = 1;
					}
//					//neu GPS2 A thi cap nhat LCD theo GPS 2
//					if((GPS_clock.gps2_valid == 'A') && (GPS_clock.gps1_lock != 1))
//				{
//					updateLCD =1;
//				}
				
				huart2.pRxBuffPtr = (uint8_t *)Rx2Buffer;
				huart2.RxXferCount = RX2BUFFERSIZE;
				memset(Rx2Buffer,0,RX2BUFFERSIZE);
				HAL_GPIO_WritePin(LED_CPU_GPIO_Port, LED_CPU_Pin, GPIO_PIN_SET);
			}
		if(u3Timeout == 1) 
			{
				u3Timeout = 0;
				u3_halt   = 0;//u3 ok
				//printf("U3 %s; \r\n",Rx3Buffer);
				if((Rx3Buffer[0] == 'I') && (Rx3Buffer[1] == 'P')&& (Rx3Buffer[3] == ':'))
					{
						GPS_clock.con_ip[0] = Rx3Buffer[4];
						GPS_clock.con_ip[1] = Rx3Buffer[5];
						GPS_clock.con_ip[2] = Rx3Buffer[6];
						GPS_clock.con_ip[3] = Rx3Buffer[7];
						GPS_clock.con_ip[4] = Rx3Buffer[8];
						GPS_clock.con_ip[5] = Rx3Buffer[9];
						GPS_clock.con_ip[6] = Rx3Buffer[10];
						GPS_clock.con_ip[7] = Rx3Buffer[11];
						GPS_clock.con_ip[8] = Rx3Buffer[12];
						GPS_clock.con_ip[9] = Rx3Buffer[13];
						GPS_clock.con_ip[10] = Rx3Buffer[14];
						GPS_clock.con_ip[11] = Rx3Buffer[15];
						GPS_clock.con_ip[12] = Rx3Buffer[16];
						GPS_clock.con_ip[13] = Rx3Buffer[17];
						GPS_clock.con_ip[14] = Rx3Buffer[18];
						GPS_clock.con_ip[15] = Rx3Buffer[19];
						//HAL_UART_Transmit(&huart3, GPS_clock.con_ip, 16, 100);
					  //HAL_UART_Transmit(&huart3, (uint8_t *)&" IP\r\n", 5, 100);
						LCD_Gotoxy(1,20);
						LCD_Puts("IP: ");
						LCD_Puts2(GPS_clock.con_ip,16);
					}
					
				huart3.pRxBuffPtr = (uint8_t *)Rx3Buffer;
				huart3.RxXferCount = RX3BUFFERSIZE;
				memset(Rx3Buffer,0,RX3BUFFERSIZE);
			}
			
			if(u1_halt > UART_MAYBE_HALT) //Kha nang treo r
			{
				u1_halt = 0;
				HAL_UART_Abort(&huart1);
				HAL_UART_Receive_IT(&huart1, Rx1Buffer, RX1BUFFERSIZE);
				//printf("Restart huart1\r\n");
			}
			if(u2_halt > UART_MAYBE_HALT) //Kha nang treo r
			{
				u2_halt = 0;
				HAL_UART_Abort(&huart2);
				HAL_UART_Receive_IT(&huart2, Rx2Buffer, RX2BUFFERSIZE);
				//printf("Restart huart2\r\n");
			}
			if(u3_halt > UART_MAYBE_HALT) //Kha nang treo r
			{
				u3_halt = 0;
				HAL_UART_Abort(&huart3);
				HAL_UART_Receive_IT(&huart3, Rx3Buffer, RX3BUFFERSIZE);
				//printf("Restart huart3\r\n");
			}
			//Neu UART ko nhan duoc du lieu nua thi xoa di
			if((u1_halt > 10) && (GPS_clock.gps1_valid != 'X'))
			{
				GPS_clock.gps1_valid = 'X';
				GPS_clock.gps1_lock = 0;
				memset(GPS_clock.gps1_time,' ',6);
				memset(GPS_clock.gps1_date,' ',6);
			}
			if((u2_halt > 10) && (GPS_clock.gps2_valid != 'X'))
			{
				GPS_clock.gps2_valid = 'X';
				GPS_clock.gps2_lock = 0;
				memset(GPS_clock.gps2_time,' ',6);
				memset(GPS_clock.gps2_date,' ',6);
			}
			//Neu ko nhan dc IP thi xoa du lieu tren LCD
			if((u3_halt > 30) && (GPS_clock.con_ip[0] != ' '))
			{
				memset(GPS_clock.con_ip,' ',16);
				LCD_Gotoxy(1,20);
				LCD_Puts("    ");
				LCD_Puts2(GPS_clock.con_ip,16);
			}
}

void sweep_LCD(void)
{
	laythoigian();
	LCD_Gotoxy(0,5);
	LCD_Putc(GPS_clock.gps1_valid);
	LCD_Gotoxy(0,7);
	LCD_Puts2(GPS_clock.gps1_time, 6);
  //LCD_Puts(GPS_clock.gps1_time);
	LCD_Gotoxy(0,14);
	LCD_Puts2(GPS_clock.gps1_date, 6);
	LCD_Gotoxy(0,25);
	LCD_Puts2(GPS_clock.rtc_time, 6);
	LCD_Gotoxy(0,32);
	LCD_Puts2(GPS_clock.rtc_date, 6);
	
	LCD_Gotoxy(1,5);
	LCD_Putc(GPS_clock.gps2_valid);
	LCD_Gotoxy(1,7);
	LCD_Puts2(GPS_clock.gps2_time, 6);
  //LCD_Puts(GPS_clock.gps2_time);
	LCD_Gotoxy(1,14);
	LCD_Puts2(GPS_clock.gps2_date, 6);
}

//----------------------Phan giao tiep i2c-thoi gian thuc ---------------------//

void BCD_Decoder()
{
	for(char x=0;x<7;x++) ds3231_reg[x]=(i2c_rv[x] & 0x0f) + (i2c_rv[x]>>4)*10;
}
unsigned char BCD_Encoder(unsigned char temp)
{
	return ((temp/10)<<4)|(temp%10);
}
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
	
	GPS_clock.rtc_time[1] = '0' + hours %10;
  GPS_clock.rtc_time[0] = '0' + hours /10;
  GPS_clock.rtc_time[3] = '0' + minutes %10;
  GPS_clock.rtc_time[2] = '0' + minutes /10;	
  GPS_clock.rtc_time[5] = '0' + seconds %10;
  GPS_clock.rtc_time[4] = '0' + seconds /10;	
	
	GPS_clock.rtc_date[1] = '0' + days %10;
  GPS_clock.rtc_date[0] = '0' + days /10;
  GPS_clock.rtc_date[3] = '0' + months %10;
  GPS_clock.rtc_date[2] = '0' + months /10;	
  GPS_clock.rtc_date[5] = '0' + years %10;
  GPS_clock.rtc_date[4] = '0' + years /10;
	//Neu GPS1 not lock, GPS2 not lock => ban tin lay tu RTC gui di mach giao tiep
	if((GPS_clock.gps1_lock != 1) && (GPS_clock.gps2_lock != 1))
	{
		TimeMessage[4] = GPS_clock.rtc_time[0];
		TimeMessage[5] = GPS_clock.rtc_time[1];
		TimeMessage[6] = GPS_clock.rtc_time[2];
		TimeMessage[7] = GPS_clock.rtc_time[3];
		TimeMessage[8] = GPS_clock.rtc_time[4];
		TimeMessage[9] = GPS_clock.rtc_time[5];
		TimeMessage[10] = GPS_clock.rtc_date[0];
		TimeMessage[11] = GPS_clock.rtc_date[1];
		TimeMessage[12] = GPS_clock.rtc_date[2];
		TimeMessage[13] = GPS_clock.rtc_date[3];
		TimeMessage[14] = GPS_clock.rtc_date[4];
		TimeMessage[15] = GPS_clock.rtc_date[5];
		//Gui thoi gian den mach giao tiep
		HAL_UART_Transmit(&huart3, TimeMessage, 20, 100);
	}
}
void ghids(unsigned char add, unsigned char dat)
{
	i2c_write[0] = BCD_Encoder(dat);
	HAL_I2C_Mem_Write(&hi2c2,0x68<<1,add,I2C_MEMADD_SIZE_8BIT,i2c_write,1,1000); 
}



