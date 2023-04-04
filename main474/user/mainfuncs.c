#include "main.h"
#include <stdio.h>
#include <string.h>
#include "gps.h"
#include "rtc.h"
#include "lcd.h"
#include "maindefines.h"

uint32_t delta_rtc_gps;
uint32_t t0,t1,t2,t3,t4;

extern uint8_t gps1_newdata;
extern uint8_t gps2_newdata;

extern uint32_t u3Timeout;
extern uint8_t numSat1;
extern uint8_t numSat2;
uint8_t trigger_to_send = 0;
char dc1_status;
char dc2_status;
main_sst main_status;
//char master_date_time[20];

uint8_t TimeMessage[20];
uint8_t Rx3Buffer[RX3BUFFERSIZE];
//uint32_t u1_halt,u2_halt;
uint32_t u3_halt = 0;

uint8_t gps1_pps = use_done;
uint8_t gps2_pps = use_done;
uint8_t rtc_pps  = use_done;
uint8_t rtc_update = 0;

uint8_t TimeToSaveRTC = PeriodSaveRTC;

//uint8_t Flag_sweepLCD = 0;
uint8_t Flag_send2conn = 0;



uint8_t couter_GPS1 = 0;
uint8_t couter_GPS2 = 0;

//uint8_t Flag5seconds = 0;



uint8_t timeSource = RTC;

//volatile uint32_t t0=0,t1,t2,t3,t4=0,t5,tgps1,tgps2;
/*
//Lay thoi gian dua vao ban tin gui
*/


void get_dateTimefromRTC(void)
{
	uint8_t temp_hours,temp_minutes,temp_seconds,temp_days,temp_months,temp_years;

	laythoigian();
	rtc_update = 1;
	
	temp_seconds = seconds;
	temp_minutes = minutes;
	temp_hours   =   hours ;
	temp_days    = days;
	temp_months  = months;
	temp_years   = years;
	
	
	TimeMessage[4] = '0' + temp_hours /10;
	TimeMessage[5] = '0' + temp_hours %10;
	TimeMessage[6] = '0' + temp_minutes /10;	
	TimeMessage[7] = '0' + temp_minutes %10;
	TimeMessage[8] = '0' + temp_seconds /10;	
	TimeMessage[9] = '0' + temp_seconds %10;	
	
	TimeMessage[10] = '0' + temp_days /10;
	TimeMessage[11] = '0' + temp_days %10;
	TimeMessage[12] = '0' + temp_months /10;
	TimeMessage[13] = '0' + temp_months %10;
	TimeMessage[14] = '0' + temp_years /10;
	TimeMessage[15] = '0' + temp_years %10;
	
	
}
/*
Kiem tra xem nguon thoi gian nao kha dung bay gio
*/
void select_time_source(void)
{
	//Ko co A thi coi nhu ko co GPS
	if((gps1_valid[0] != 'A')&&(gps2_valid[0] != 'A')&&(timeSource != RTC))
	{
		timeSource = RTC;
		//printf("USE RTC\r\n");
		LCD_Gotoxy(0,4);
		LCD_Putc(':');
		LCD_Gotoxy(1,4);
		LCD_Putc(':');
		LCD_Gotoxy(0,24);
		LCD_Putc('.');
	}		
	else if((couter_GPS1 >= GPS_Stable_counter)&&(numSat1 >= numSat2)&&(numSat1 > 3)&&(timeSource != GPS1))
	{
		timeSource = GPS1;
		//printf("USE GPS1\r\n");
		LCD_Gotoxy(0,4);
		LCD_Putc(';');
		LCD_Gotoxy(1,4);
		LCD_Putc(':');
		LCD_Gotoxy(0,24);
		LCD_Putc(':');
	}
	else if((couter_GPS2 >= GPS_Stable_counter)&&(numSat2 > numSat1)&&(numSat2 > 3)&&(timeSource != GPS2))
	{
		timeSource = GPS2;
		//printf("USE GPS2\r\n");
		LCD_Gotoxy(0,4);
		LCD_Putc(':');
		LCD_Gotoxy(1,4);
		LCD_Putc(';');
		LCD_Gotoxy(0,24);
		LCD_Putc(':');
	}
	
	
	//Tin hieu PPS da on dinh
	if(couter_GPS1 > GPS_Stable_counter) 
	{
		couter_GPS1 = GPS_Stable_counter;
	}
	if(couter_GPS2 > GPS_Stable_counter)
	{
		couter_GPS2 = GPS_Stable_counter;
	}
	
	//GPS LED Sources indicate
	if(gps1_valid[0] == 'A') 
		{
			HAL_GPIO_WritePin(LED_GPS1_GPIO_Port, LED_GPS1_Pin, GPIO_PIN_SET);
			TimeMessage[16] = 'A';
		}
	else 
		{
			couter_GPS1 = 0;//dang chay ngon mat anten
			HAL_GPIO_WritePin(LED_GPS1_GPIO_Port, LED_GPS1_Pin, GPIO_PIN_RESET);
			TimeMessage[16] = 'V';
		}
		
	if(gps2_valid[0] == 'A') 
		{
			HAL_GPIO_WritePin(LED_GPS2_GPIO_Port, LED_GPS2_Pin, GPIO_PIN_SET);
			TimeMessage[17] = 'A';
		}
	else
		{
			couter_GPS2 = 0;//dang chay ngon mat anten
			HAL_GPIO_WritePin(LED_GPS1_GPIO_Port, LED_GPS2_Pin, GPIO_PIN_RESET);
			TimeMessage[17] = 'V';
		} 
	
}

void LCD_reInit(void)
{
	LCD_Init();
	LCD_Gotoxy(0,0);
	LCD_Puts("GPS1:");
			
	LCD_Gotoxy(1,0);
	LCD_Puts("GPS2:");
	
	LCD_Gotoxy(0,21);
	LCD_Puts("RTC:");
}
/*
Moi giay goi ham nay 1 lan
*/
void oneSecondCount(void)
{
	
	
	u3_halt++;
	if(timeSource != RTC) //Co GPS
		{
				if(TimeToSaveRTC > 1) 
						TimeToSaveRTC--;
				//TimeToSaveRTC => luu RTC thoi?
		}
	

	Display_Time();
	scan_ADC();
  select_time_source();
	LCD_reInit();
}

/*
Save info to RTC
*/
void SaveGPStoRTC(void)
{
	uint8_t gio,phut,giay,ngay,thang,nam;
	

	
	if(TimeToSaveRTC < 6)
		{
			//return;
			if(timeSource == GPS1)
			{
					//ko dc luu gia tri lon hon 59 => loi
				giay = 10*(gps1_time[4] - '0') + gps1_time[5]-'0' + 1;
				if(giay <56)//Sap sang phut moi, ko luu tranh loi
				{
					gio   = gps1_time[1]-'0' + 10*(gps1_time[0]-'0') ;
					phut  = gps1_time[3]-'0' + 10*(gps1_time[2]-'0') ;
					//giay = 10*(TimeMessage[8] - '0') + TimeMessage[9]- '0' + 2;
		
					ngay  = gps1_date[1]-'0' + 10*(gps1_date[0]-'0') ;
					thang = gps1_date[3]-'0' + 10*(gps1_date[2]-'0') ;
					nam   = gps1_date[5]-'0' + 10*(gps1_date[4]-'0') ;
		
					UpdateRtcTime(gio,phut,giay,ngay,thang,nam);
					TimeToSaveRTC = PeriodSaveRTC;	//reset counter
					
					
					//t0 = fractionOfSecond;
					//printf("Save RTC %d %d %d, %d/%d/%d\r\n",gio,phut,giay,ngay,thang,nam);
					//LCD_Gotoxy(1,21);
					//LCD_Puts("RTC saved1!");
					//lcdprintf("    %d",t0);
				}
			}
			if(timeSource == GPS2)
			{
						//ko dc luu gia tri lon hon 59 => loi
				giay = 10*(gps2_time[4] - '0') + gps2_time[5]-'0' + 1;
				if(giay <56)//Sap sang phut moi, ko luu tranh loi
				{
					gio   = gps2_time[1]-'0' + 10*(gps2_time[0]-'0') ;
					phut  = gps2_time[3]-'0' + 10*(gps2_time[2]-'0') ;
					//giay = 10*(TimeMessage[8] - '0') + TimeMessage[9]- '0' + 2;
		
					ngay  = gps2_date[1]-'0' + 10*(gps2_date[0]-'0') ;
					thang = gps2_date[3]-'0' + 10*(gps2_date[2]-'0') ;
					nam   = gps2_date[5]-'0' + 10*(gps2_date[4]-'0') ;
		
					UpdateRtcTime(gio,phut,giay,ngay,thang,nam);
					TimeToSaveRTC = PeriodSaveRTC;	//reset counter
					
					
					//t0 = fractionOfSecond;
					//printf("Save RTC\r\n %d %d %d, %d/%d/%d\r\n",gio,phut,giay,ngay,thang,nam);
					//LCD_Gotoxy(1,21);
					//LCD_Puts("RTC saved2!");
					//lcdprintf("    %d",t0);
				}
			}
			
		}
		//HAL_GPIO_WritePin(LED_CPU_GPIO_Port, LED_CPU_Pin, GPIO_PIN_RESET);			
		//printf("#S1:%02dS2:%02dU%d\r\n",numSat1,numSat2,timeSource);
		//printf("#S1:%02dS2:%02dU%d-%d\r\n",numSat1,numSat2,timeSource,delta_rtc_gps);
}



void checkGPSdata(void)
{

}
/*
Gui thoi gian sang mach giao tiep
- Xung PPS
- Time
=> sent
*/
void sendTimeNew(void)
{
	if(trigger_to_send)//Co tin hieu gui, gui luon
	{
		trigger_to_send = 0;
		
		get_dateTimefromRTC();
		HAL_UART_Transmit(&huart3, TimeMessage, 20, 100);
		
		
		
		//Neu ko co A trong gps => bo qua!
		if((gps1_valid[0] == 'A')||(gps2_valid[0] == 'A')) 
		{
			printf("#S1:%02dS2:%02dU%d ",numSat1,numSat2,timeSource);
		}
		HAL_GPIO_WritePin(LED_CPU_GPIO_Port, LED_CPU_Pin, GPIO_PIN_RESET);	
		HAL_GPIO_WritePin(MAIN_PPS_GPIO_Port, MAIN_PPS_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);					
		//printf(" RTC%d\r\n",delta_rtc_gps);
	}
	
}
/*
CHUONG TRINH CHINH
1- Gui ban tin thoi gian sang mach giao tiep
2- Hien thi LCD
3- Luu gio RTC va hien thi IP

1: Gui ban tin
- Neu co GPS tot, gui ban tin theo gps tot
- Neu GPS ko tot, gui ban tin theo RTC
*/
void oneSecRun(void)
{
	if(tim20ct > 999) //1 second
	{
		tim20ct = 0;
		oneSecondCount();
						
	}
}
void alwaysRun(void)
{
	if(gps1_pps == JustHigh)
		{
			gps1_pps = use_done;
			SaveGPStoRTC();
		}
	if(gps2_pps == JustHigh)
		{
			gps2_pps = use_done;
			SaveGPStoRTC();
		}	
	if(rtc_pps == JustHigh)
		{
			rtc_pps = use_done;
		}
	checkUART();   //IP info
	
	if(fractionOfSecond> 999)//100ms
		{
			//Gui xung PPS toi mach giao tiep
			
			//HAL_GPIO_WritePin(MAIN_PPS_GPIO_Port, MAIN_PPS_Pin, GPIO_PIN_SET);
			//HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);			
			//HAL_GPIO_WritePin(LED_CPU_GPIO_Port, LED_CPU_Pin, GPIO_PIN_SET);			
		}
	
}

void sentTime(void)
{
	sendTimeNew();
}

void displayLCD(void)
{
	if(rtc_update == 1)
	{
		rtc_update = 0;
		LCD_Gotoxy(0,26);
		lcdprintf("%02d%02d%02d ",hours,minutes,seconds);
	}
	if(gps1_newdata == 1)
	{
		gps1_newdata = 0;
		LCD_Gotoxy(0,6);
		LCD_Puts(gps1_time);
		
	}
	
	if(gps2_newdata == 1)
	{
		gps2_newdata = 0;
		LCD_Gotoxy(1,6);						
		LCD_Puts(gps2_time);
		
	}
		
}
/*
DEBUG
*/
void debug_tasks(void)
{
	/*
	Hien thi ve tinh
	*/
	/*
	Flag5seconds++;
	if(Flag5seconds%5 == 0) //5s moi lan
	{
		LCD_Gotoxy(1,21);
		lcdprintf("Sat1:%02d Sat2:%02d %01d  ",numSat1,numSat2,timeSource);
	}	
	printf("#S1:%02dS2:%02dU%d\r\n",numSat1,numSat2,timeSource);
	*/
	
	//printf("gps %d\r\n",tgps1);
	//printf("#S1:%02dS2:%02dU%d\r\n",numSat1,numSat2,timeSource);
}
/**
TASKS
*/
void tasks(void)
{
	sentTime();
	displayLCD();
	alwaysRun();
	oneSecRun();
	debug_tasks();
}


/**
KHOI TAO
*/
void main_init(void)
{
	MX_ADC2_Init_MOD();
	//printf("This code gen by STMcube;STM32G474RBT6@128MHz\r\n");
	HAL_Delay(100);
	LCD_Init();
	LCD_Clear();
	LCD_Gotoxy(0,0);
	//LCD_Puts("STM32G474RBT6@128MHz");
	LCD_Puts("ATTECH GPS master clock");
	HAL_UART_Abort(&huart3);
	if (HAL_UART_Receive_IT(&huart3, (uint8_t *)Rx3Buffer, RX3BUFFERSIZE) != HAL_OK)
		{
			/* Transfer error in reception process */
			Error_Handler();
		}
	HAL_TIM_Base_Start_IT(&htim20);
	HAL_TIM_Base_Start(&htim3);
	//HAL_TIM_Base_Start(&htim2);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	/* Run the ADC calibration in single-ended mode */
	if (HAL_ADCEx_Calibration_Start(&hadc2, ADC_SINGLE_ENDED) != HAL_OK)
  {
    /* Calibration Error */
    Error_Handler();
  }
	
	GPS_Init();
	HAL_Delay(999);
	LCD_Clear();
	LCD_Gotoxy(0,0);
	LCD_Puts("GPS1:");
			
	LCD_Gotoxy(1,0);
	LCD_Puts("GPS2:");
	
	LCD_Gotoxy(0,21);
	LCD_Puts("RTC:");
	
	TimeMessage[0] = '$';
	TimeMessage[1] = 'G';
	TimeMessage[2] = 'P';
	TimeMessage[3] = 'S';
	TimeMessage[16] = 'V';
	TimeMessage[17] = 'V';
	TimeMessage[18] = '1';
	TimeMessage[19] = '1';
	laythoigian();
	get_dateTimefromRTC();
	
	//Neu ko co xung RTC => RTC het pin hoac hong!
	if(rtc_pps == use_done) ghids(14,0);//1HZ out SQW
	//printf("RTC pps: %d\r\n",rtc_pps);
	trigger_to_send = 0;
}

/*
Kiem tra UART
*/
void checkUART(void)
{
	if(u3Timeout == 1)
			{
				u3Timeout = 0;
				u3_halt   = 0;//u3 ok
				//printf("\r\nU3:%s\r\n",Rx3Buffer);
				
				if((Rx3Buffer[0] == 'I') && (Rx3Buffer[1] == 'P')&& (Rx3Buffer[3] == ':'))
					{
						Rx3Buffer[19] = 0;
						LCD_Gotoxy(1,21);
						LCD_Puts("                   ");
						LCD_Gotoxy(1,21);
						LCD_Puts((char*)Rx3Buffer);
					}
//				else 
//				{
//						Rx3Buffer[19] = 0;
//						LCD_Gotoxy(1,21);
//						LCD_Puts("                   ");
//						LCD_Gotoxy(1,21);
//						LCD_Puts((char*)Rx3Buffer);
//				}					
				huart3.pRxBuffPtr = (uint8_t *)Rx3Buffer;
				huart3.RxXferCount = RX3BUFFERSIZE;
				memset(Rx3Buffer,0,RX3BUFFERSIZE);
			}
	//Neu ko nhan dc IP thi xoa du lieu tren LCD,  //Kha nang treo r
			if(u3_halt > 30)
			{
				LCD_Gotoxy(1,21);
				LCD_Puts("                   ");
				u3_halt = 0;
				HAL_UART_Abort(&huart3);
				HAL_UART_Receive_IT(&huart3, Rx3Buffer, RX3BUFFERSIZE);
			}
}


//==============================================
/*
Hien thi cac thong tin tren LCD
*/
void Display_Time(void)
{			
			//GPS1
			LCD_Gotoxy(0,5);
			LCD_Putc(gps1_valid[0]);
			
			//LCD_Gotoxy(0,6);
			//LCD_Puts(gps1_time);

			LCD_Gotoxy(0,13);
			LCD_Puts(gps1_date);
	
			//LCD_Gotoxy(0,20);
			//LCD_Putc(numSat1+'0');
			//GPS2
			LCD_Gotoxy(1,5);
			LCD_Putc(gps2_valid[0]);
			
//			LCD_Gotoxy(1,6);						
//			LCD_Puts(gps2_time);
			

			LCD_Gotoxy(1,13);
			LCD_Puts(gps2_date);
      
			//LCD_Gotoxy(1,20);
			//LCD_Putc(numSat2+'0');
			LCD_Gotoxy(0,33);
			lcdprintf("%02d%02d%02d ",days,months,years);
			
}
//==============================================		



/**
  * @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
	* Neu co GPS, se co xung PPS tai thoi diem bat dau moi giay
  * If GPS avaiable, PPS pulse start at the start of a second

			Dac diem GPS LEA M8F
		- Luc bat dau cap dien cho module GPS, ko co xung PPS, sau 1 luc bat dc ve tinh thi bat dau co du lieu, thoi gian ban dau sai lech nhau
		sau 1 thoi gian thi bat dau deu nhau, xung pps thay bang nhau
		- PPS xung 100ms, main pps dang cham hon 4ms
		- Luc moi bat len ko co du lieu thoi gian, ban tin thoi gian
		- Khi co day du GPS, PPS deu nhau, ke ca sau do rut antena ra thi van deu

  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  
	
	if (GPIO_Pin == GPS1PPS_Pin)//printf("GPS1 PPS\r\n");
  {
		if(timeSource == GPS1) 
		{
			fractionOfSecond = 3;
			//trigger_to_send = 1;
			//Gui xung PPS toi mach giao tiep
			//HAL_GPIO_WritePin(MAIN_PPS_GPIO_Port, MAIN_PPS_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(LED_CPU_GPIO_Port, LED_CPU_Pin, GPIO_PIN_SET);			
		}

		gps1_pps = JustHigh;
		couter_GPS1++;
  }
	
	if (GPIO_Pin == GPS2PPS_Pin)//printf("GPS2 PPS\r\n");
  {
		if(timeSource == GPS2)
		{
			fractionOfSecond = 3;
			//trigger_to_send = 1;
			//Gui xung PPS toi mach giao tiep
			//HAL_GPIO_WritePin(MAIN_PPS_GPIO_Port, MAIN_PPS_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(LED_CPU_GPIO_Port, LED_CPU_Pin, GPIO_PIN_SET);			
		}
		gps2_pps = JustHigh;
		couter_GPS2++;
  }
	
	if (GPIO_Pin == RTC_Pin)//printf("RTC_Pin\r\n");
  {
		HAL_GPIO_WritePin(MAIN_PPS_GPIO_Port, MAIN_PPS_Pin, GPIO_PIN_RESET);
		trigger_to_send = 1;
		if(timeSource == RTC)
		{
			HAL_GPIO_WritePin(LED_CPU_GPIO_Port, LED_CPU_Pin, GPIO_PIN_SET);			
		}
		else delta_rtc_gps = fractionOfSecond;
		
		HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
		rtc_pps = JustHigh;
	}
}
/*
Scan ADC
*/
void scan_ADC(void)
	{
		uint32_t raw;
		
		

		//PA6 - 12V
		ADC_Select_CH3();
		HAL_ADC_Start(&hadc2);
		HAL_ADC_PollForConversion(&hadc2, 200);
		raw = HAL_ADC_GetValue(&hadc2);
		HAL_ADC_Stop(&hadc2);
		// Convert to string and print
		//printf("PA6-ADC2-IN3 :%d\r\n", raw);
		
		//printf("Den ap dc2: %2.1f\r\n", (float)(main_status.vin2x10)/10.0);		
			
		//LCD_Gotoxy(1,21);
		//lcdprintf("PA6-ADC2-IN3 %d  ",main_status.vin2x10);
			
		
		
		//Do dien ap 5V
		ADC_Select_CH4();
		HAL_ADC_Start(&hadc2);
		HAL_ADC_PollForConversion(&hadc2, 200);
		raw = HAL_ADC_GetValue(&hadc2);
		HAL_ADC_Stop(&hadc2);
		main_status.vin2x10 = (uint16_t)(k5v * (float)raw);
		// Convert to string and print
		//printf("PA7-ADC2-IN4 :%d\r\n", raw);
		//LCD_Gotoxy(1,21);
		//lcdprintf("PA7-ADC2-IN4 %d  ",main_status.vin2x10);
		
		//DCIN1
		ADC_Select_CH13();
		HAL_ADC_Start(&hadc2);
		HAL_ADC_PollForConversion(&hadc2, 200);
		raw = HAL_ADC_GetValue(&hadc2);
		HAL_ADC_Stop(&hadc2);
		//LCD_Gotoxy(1,21);
		//lcdprintf("PA5-IN13 %d  ",raw);
		// Convert to string and print
		//printf("PA5-ADC2-IN13 :%d\r\n", raw);
		main_status.vin1x10 = (uint16_t)(kDCin1 * (float)raw);
		//LCD_Gotoxy(1,21);
		//lcdprintf("vin1x10 %d  ",main_status.vin1x10);
		//printf("Den ap dc1: %2.1f\r\n", (float)(main_status.vin1x10)/10.0);	
		if (main_status.vin1x10 > 9)
		{
			dc1_status = '1';
			TimeMessage[18] = '1';
		}
		else
			{
				dc1_status = '0';	
				TimeMessage[18] = '0';
			}	
		
		//DCIN2
		ADC_Select_CH17();
		HAL_ADC_Start(&hadc2);
		HAL_ADC_PollForConversion(&hadc2, 200);
		raw = HAL_ADC_GetValue(&hadc2);
		HAL_ADC_Stop(&hadc2);
		// Convert to string and print
		//printf("PA4-ADC2-IN17 :%d\r\n", raw);
		main_status.vin2x10 = (uint16_t)(k12v * (float)raw);
		if (main_status.vin2x10 > 9)
		{
			dc2_status = '1';
			TimeMessage[19] = '1';
		}
		else
		{
			dc2_status = '0';
			TimeMessage[19] = '0';
		}
		//LCD_Gotoxy(1,21);
		//lcdprintf("PA4-IN17 %d  ",raw);
		//lcdprintf("DCIN2 %d  ",raw);
		//lcdprintf("DCIN1 %d DCIN2 %d  ",main_status.vin1x10,main_status.vin2x10);
		
	}
//=======================================================
void ADC_Select_CH3 (void)
{
	ADC_ChannelConfTypeDef sConfig = {0};
	  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
	  */
	  sConfig.Channel = ADC_CHANNEL_3;
	  sConfig.Rank = ADC_REGULAR_RANK_1;
		sConfig.SamplingTime = ADC_SAMPLETIME_92CYCLES_5;
		sConfig.SingleDiff = ADC_SINGLE_ENDED;
		sConfig.OffsetNumber = ADC_OFFSET_NONE;
		sConfig.Offset = 0;
		if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
		{
			Error_Handler();
		}
}

//=======================================================
void ADC_Select_CH4 (void)
{
	ADC_ChannelConfTypeDef sConfig = {0};
	  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
	  */
	  sConfig.Channel = ADC_CHANNEL_4;
	  sConfig.Rank = ADC_REGULAR_RANK_1;
		sConfig.SamplingTime = ADC_SAMPLETIME_92CYCLES_5;
		sConfig.SingleDiff = ADC_SINGLE_ENDED;
		sConfig.OffsetNumber = ADC_OFFSET_NONE;
		sConfig.Offset = 0;
		if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
		{
			Error_Handler();
		}
}
//=======================================================
void ADC_Select_CH13 (void)
{
	ADC_ChannelConfTypeDef sConfig = {0};
	
	sConfig.Channel = ADC_CHANNEL_13;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_92CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
}
//=======================================================
void ADC_Select_CH17 (void)
{
	ADC_ChannelConfTypeDef sConfig = {0};
	  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
	  */
	sConfig.Channel = ADC_CHANNEL_17;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_247CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
}
//=======================================================
void MX_ADC2_Init_MOD(void)
{

  /* USER CODE BEGIN ADC2_Init 0 */

  /* USER CODE END ADC2_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC2_Init 1 */
//hadc2.Init.GainCompensation = VDDA_APPLI * GAIN_COMPENSATION_X1_FACTOR / DIGITAL_SCALE_12BITS;
  /* USER CODE END ADC2_Init 1 */

  /** Common config
  */
  hadc2.Instance = ADC2;
  hadc2.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV256;
  hadc2.Init.Resolution = ADC_RESOLUTION_12B;
  hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc2.Init.GainCompensation = 0;
  hadc2.Init.ScanConvMode = ADC_SCAN_ENABLE;
  hadc2.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc2.Init.LowPowerAutoWait = DISABLE;
  hadc2.Init.ContinuousConvMode = ENABLE;
  hadc2.Init.NbrOfConversion = 1;
  hadc2.Init.DiscontinuousConvMode = DISABLE;
  hadc2.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc2.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc2.Init.DMAContinuousRequests = DISABLE;
  hadc2.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
  hadc2.Init.OversamplingMode = DISABLE;
  if (HAL_ADC_Init(&hadc2) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_17;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

//  /** Configure Regular Channel
//  */
//  sConfig.Channel = ADC_CHANNEL_3;
//  sConfig.Rank = ADC_REGULAR_RANK_2;
//  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
//  {
//    Error_Handler();
//  }

//  /** Configure Regular Channel
//  */
//  sConfig.Channel = ADC_CHANNEL_4;
//  sConfig.Rank = ADC_REGULAR_RANK_3;
//  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
//  {
//    Error_Handler();
//  }

//  /** Configure Regular Channel
//  */
//  sConfig.Channel = ADC_CHANNEL_13;
//  sConfig.Rank = ADC_REGULAR_RANK_4;
//  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
//  {
//    Error_Handler();
//  }
  /* USER CODE BEGIN ADC2_Init 2 */

  /* USER CODE END ADC2_Init 2 */

}


