/*
CHUONG TRINH CHINH
1- Gui ban tin thoi gian sang mach giao tiep
2- Hien thi LCD
3- Luu gio RTC va hien thi IP

1: Gui ban tin
- Neu co GPS tot, gui ban tin theo gps tot
- Neu GPS ko tot, gui ban tin theo RTC
*/
void main_apps(void)
{
	HAL_GPIO_TogglePin(LED_CPU_GPIO_Port, LED_CPU_Pin);
	
	//Flag_send2conn = 1;//Ko gui ban tin nua => debug
	
	if(mainGPSSignal == NOGPS)//Ko co GPS, tin hieu GPS chua on dinh, gui ban tin theo RTC
	{
		if(rtc_pps == 1)
		{
			rtc_pps = 0;
			tim20ct = 0;
			if(Flag_send2conn != 1)
			{
				get_dateTimefromRTC();
				//Gui thoi gian den mach giao tiep
				//t3 = counterOf10Second;
				HAL_UART_Transmit(&huart3, TimeMessage, 20, 100);
			  Flag_send2conn = 1;//Co bao da gui ban tin 
				//printf("--RTC\r\n");
				printf("#S1:%02dS2:%02dU%d\r\n",numSat1,numSat2,mainGPSSignal);
				//printf("t0 :%d,t1 :%d,t2 :%d,t3 :%d\r\n",t0,t1,t2,t3);
			}
			//HAL_GPIO_WritePin(MAIN_PPS_GPIO_Port, MAIN_PPS_Pin, GPIO_PIN_RESET);
			seconds++;
		  if(seconds>59) laythoigian();
			Display_Time();
			Flag_sweepLCD = 1;//Co bao da sweep LCD
			
		}
	}
	else if(mainGPSSignal == GPS1)//GPS1 tin hieu tot
	{
		if(gps1_pps == 1) 
		{
			gps1_pps = 0;
			gps2_pps = 0;
			if(Flag_send2conn != 1)
			{
				dateAndTime(gps1_time,gps1_date);
				//Gui thoi gian den mach giao tiep
				//t3 = counterOf10Second;
				HAL_UART_Transmit(&huart3, TimeMessage, 20, 100);
				Flag_send2conn = 1;//Co bao da gui ban tin
				//printf("--GPS1\r\n");			
				printf("#S1:%02dS2:%02dU%d\r\n",numSat1,numSat2,mainGPSSignal);
				//printf("t0 :%d,t1 :%d,t2 :%d,t3 :%d,t4 :%d\r\n",t0,t1,t2,t3,t4);		
			}
			//HAL_GPIO_WritePin(MAIN_PPS_GPIO_Port, MAIN_PPS_Pin, GPIO_PIN_RESET);
			SaveGPStoRTC();
			
			seconds++;
		  if(seconds>59) laythoigian();
			
			Display_Time();
			
			Flag_sweepLCD = 1;//Co bao da sweep LCD
			
		}
	}
	else if(mainGPSSignal == GPS2)//GPS2 tin hieu tot
	{
		if(gps2_pps == 1) 
		{
			gps2_pps = 0;
			gps1_pps = 0;
			if(Flag_send2conn != 1)
			{
				dateAndTime(gps2_time,gps2_date);
				//Gui thoi gian den mach giao tiep
				//t3 = counterOf10Second;
				HAL_UART_Transmit(&huart3, TimeMessage, 20, 100);
				Flag_send2conn = 1;//Co bao da gui ban tin 
				//printf("--GPS2\r\n");
				printf("#S1:%02dS2:%02dU%d\r\n",numSat1,numSat2,mainGPSSignal);
				//printf("t0 :%d,t1 :%d,t2 :%d,t3 :%d,t4 :%d\r\n",t0,t1,t2,t3,t4);
			}
			//HAL_GPIO_WritePin(MAIN_PPS_GPIO_Port, MAIN_PPS_Pin, GPIO_PIN_RESET);
			SaveGPStoRTC();
			seconds++;
		if(seconds>59) laythoigian();
			Display_Time();
			Flag_sweepLCD = 1;//Co bao da sweep LCD
			
		}
	}

	if(tim20ct > 999) //1 second
	{
		tim20ct = 0;
		oneSecondCount();
		//HAL_GPIO_WritePin(LED_GPS1_GPIO_Port, LED_GPS1_Pin, GPIO_PIN_RESET);
		//HAL_GPIO_WritePin(LED_GPS2_GPIO_Port, LED_GPS2_Pin, GPIO_PIN_RESET);
	}
	
		
	if(fractionOfSecond> 999)//100ms
		{
			//Gui xung PPS toi mach giao tiep
			HAL_GPIO_WritePin(MAIN_PPS_GPIO_Port, MAIN_PPS_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);			
		}
		
		
		if((rtc_pps == 1)&&(mainGPSSignal != NOGPS))
		{
			rtc_pps = 0;
			tim20ct = 0;
			//printf("t0 :%d,t1 :%d,t2 :%d\r\n",t0,t1,t2);
			//LCD_Gotoxy(1,21);
			//LCD_Puts("RTC INT");
			//LCD_Putc(48+seconds/10);
			//LCD_Putc(48+seconds%10);
		}
	checkUART();
	//HAL_GPIO_WritePin(LED_CPU_GPIO_Port, LED_CPU_Pin, GPIO_PIN_RESET);
}

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
  //Dong bo xung PPS GPS
	//Uu tien GPS1: neu có pps thi lay cua GPS1 truoc
	//Neu GPS1 ko co PPS thi moi lay PPS cua GPS2
	if (GPIO_Pin == GPS1PPS_Pin)
  {
		//fractionOfSecond = 0;
		//t4 = fractionOfSecond;
		
		//printf("GPS1PPS\r\n");
		//main_status.gps_pps = 1;
		//main_status.gps1_stt = 'A';
		//t1 = counterOf10Second;
		//HAL_GPIO_WritePin(LED_GPS1_GPIO_Port, LED_GPS1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(MAIN_PPS_GPIO_Port, MAIN_PPS_Pin, GPIO_PIN_SET);
		gps1_pps = 1;
		Flag_Have_PPS1++;
  }
	if (GPIO_Pin == GPS2PPS_Pin)
  {
		//fractionOfSecond = 0;
		//t0 = fractionOfSecond;
		HAL_GPIO_WritePin(MAIN_PPS_GPIO_Port, MAIN_PPS_Pin, GPIO_PIN_SET);
		//printf("GPS2PPS\r\n");
		//t2 = counterOf10Second;
		//HAL_GPIO_WritePin(LED_GPS2_GPIO_Port, LED_GPS2_Pin, GPIO_PIN_SET);
		gps2_pps = 1;
		Flag_Have_PPS2++;
  }
	if (GPIO_Pin == RTC_Pin)
  {
		//printf("RTC_Pin\r\n");
		//t0 = counterOf10Second;
		fractionOfSecond = 0;
		//counterOf10Second = 0;
		//Gui xung PPS toi mach giao tiep
		//HAL_GPIO_WritePin(MAIN_PPS_GPIO_Port, MAIN_PPS_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
		
		
		rtc_pps = 1;
	}
}

/*
Moi giay goi ham nay 1 lan
*/
void oneSecondCount(void)
{
	//t3 = fractionOfSecond;
	u3_halt++;
	if(TimeToSaveRTC > 1) TimeToSaveRTC--;
	
	//Truong hop nay mat xung tu RTC -> MCU : het pin
	//Neu chua gui ban tin cho mach giao tiep => gui
	if(Flag_send2conn == 0)
	{
		get_dateTimefromRTC();
		//Gui thoi gian den mach giao tiep
		HAL_UART_Transmit(&huart3, TimeMessage, 20, 100);
		//printf("#S1:%02dS2:%02dU%d\r\n",numSat1,numSat2,timeSource);
		//printf("No RTC");
	}
	else Flag_send2conn = 0;
	
	//neu chua quet LCD => quet LCD
	if(Flag_sweepLCD == 0)
	{
		//dateAndTime(gps1_time,gps1_date);
		Display_Time();
		//printf("No GPS");
	}
	else Flag_sweepLCD = 0;
	
	//laythoigian();
	scan_ADC();
	
  select_time_source();
	
	
}










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
  //Dong bo xung PPS GPS
	//Uu tien GPS1: neu có pps thi lay cua GPS1 truoc
	//Neu GPS1 ko co PPS thi moi lay PPS cua GPS2
	if (GPIO_Pin == GPS1PPS_Pin)
  {
		if(timeSource == GPS1) fractionOfSecond = 3;

		//fractionOfSecond = 0;
		//HAL_GPIO_WritePin(LED_GPS1_GPIO_Port, LED_GPS1_Pin, GPIO_PIN_SET);
		//HAL_GPIO_WritePin(MAIN_PPS_GPIO_Port, MAIN_PPS_Pin, GPIO_PIN_SET);
		gps1_pps = JustHigh;
		couter_GPS1++;
  }
	if (GPIO_Pin == GPS2PPS_Pin)//printf("GPS2PPS\r\n");
  {
		if(timeSource == GPS2) fractionOfSecond = 6;
		
		//HAL_GPIO_WritePin(MAIN_PPS_GPIO_Port, MAIN_PPS_Pin, GPIO_PIN_SET);
		
		gps2_pps = JustHigh;
		couter_GPS2++;
  }
	if (GPIO_Pin == RTC_Pin)
  {
		//printf("RTC_Pin\r\n");
		delta_rtc_gps = fractionOfSecond;
		//fractionOfSecond = 0;
		
		//Gui xung PPS toi mach giao tiep
		HAL_GPIO_WritePin(MAIN_PPS_GPIO_Port, MAIN_PPS_Pin, GPIO_PIN_SET);
		
		HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
		
		
		rtc_pps = JustHigh;
	}
}





void oneSecondCount(void)
{
	
	laythoigian();
	u3_halt++;
	if(TimeToSaveRTC > 1) TimeToSaveRTC--;

	Display_Time();
	scan_ADC();
  select_time_source();
	
	Flag_send2conn = UNSENT;
}

void dateAndTime(char* timeStr,char* dateStr)
{

	//laythoigian();
	
	TimeMessage[4] = timeStr[0];
	TimeMessage[5] = timeStr[1];
	TimeMessage[6] = timeStr[2];
	TimeMessage[7] = timeStr[3];
	TimeMessage[8] = timeStr[4];
	TimeMessage[9] = timeStr[5];
	
	TimeMessage[10] = dateStr[0];
	TimeMessage[11] = dateStr[1];
	TimeMessage[12] = dateStr[2];
	TimeMessage[13] = dateStr[3];
	TimeMessage[14] = dateStr[4];
	TimeMessage[15] = dateStr[5];
	
	TimeMessage[16] = gps1_valid[0];
	TimeMessage[17] = gps2_valid[0];
}

void sendTimeMessage(void)
{
	if(timeSource == RTC)//Ko co GPS, tin hieu GPS chua on dinh, gui ban tin theo RTC
	{
		if(rtc_pps == JustHigh)
		{
			rtc_pps = use_done;
			
			LCD_Gotoxy(0,26);
			lcdprintf("%02d%02d%02d ",hours,minutes,seconds);
			
			if(Flag_send2conn != SENT)
			{
				get_dateTimefromRTC();
				//Gui thoi gian den mach giao tiep
				//t3 = counterOf10Second;
				HAL_UART_Transmit(&huart3, TimeMessage, 20, 100);
			  Flag_send2conn = SENT;//Co bao da gui ban tin 
				//printf("--RTC\r\n");
				printf("#S1:%02dS2:%02dU%d\r\n",numSat1,numSat2,timeSource);
			}
			
			
			
		}
	}
	else if(timeSource == GPS1)//GPS1 tin hieu tot
	{
		if(gps1_pps == JustHigh) 
		{
			gps1_pps = use_done;
			//gps2_pps = use_done;
			if(Flag_send2conn != SENT)
			{
				//dateAndTime(gps1_time,gps1_date);
				get_dateTimefromRTC();
				//Gui thoi gian den mach giao tiep
				//t3 = counterOf10Second;
				HAL_UART_Transmit(&huart3, TimeMessage, 20, 100);
				Flag_send2conn = SENT;//Co bao da gui ban tin
				//printf("--GPS1\r\n");			
				
				
			}
			//HAL_GPIO_WritePin(MAIN_PPS_GPIO_Port, MAIN_PPS_Pin, GPIO_PIN_RESET);
			SaveGPStoRTC();
			//printf("#S1:%02dS2:%02dU%d\r\n",numSat1,numSat2,timeSource);
		}
	}
	else if(timeSource == GPS2)//GPS2 tin hieu tot
	{
		if(gps2_pps == JustHigh) 
		{
			gps2_pps = use_done;
			//gps1_pps = use_done;
			if(Flag_send2conn != SENT)
			{
				//dateAndTime(gps2_time,gps2_date);
				get_dateTimefromRTC();
				//Gui thoi gian den mach giao tiep
				//t3 = counterOf10Second;
				HAL_UART_Transmit(&huart3, TimeMessage, 20, 100);
				Flag_send2conn = SENT;//Co bao da gui ban tin 
				//printf("--GPS2\r\n");
				//printf("#S1:%02dS2:%02dU%d\r\n",numSat1,numSat2,timeSource);
			}
			//HAL_GPIO_WritePin(MAIN_PPS_GPIO_Port, MAIN_PPS_Pin, GPIO_PIN_RESET);
			SaveGPStoRTC();
			
			
		}
	}
}

void get_dateTimefromRTC2(void)
{
	uint8_t temp_hours,temp_minutes,temp_seconds,temp_days,temp_months,temp_years;
	laythoigian();
	
	temp_seconds = seconds + 1;
	temp_minutes = minutes;
	temp_hours   =   hours ;
	temp_days    = days;
	temp_months  = months;
	temp_years   = years;
	
	if(temp_seconds == 60) {temp_seconds = 0;temp_minutes++;}
	if(temp_minutes == 60) {temp_minutes = 0;temp_hours++;}
	if(temp_hours == 24) {temp_hours = 0;temp_days++;}
	
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
