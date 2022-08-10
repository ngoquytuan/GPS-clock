#include "main.h"
#include <stdio.h>
#include <string.h>
#include "gps.h"
#include "rtc.h"
#include "lcd.h"

extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern UART_HandleTypeDef huart3;
extern TIM_HandleTypeDef htim20;
extern TIM_HandleTypeDef htim3;

void ADC_Select_CH17(void);
void ADC_Select_CH3(void);
void ADC_Select_CH4(void);
void ADC_Select_CH13(void);

extern uint32_t u3Timeout;
extern uint8_t numSat1;
extern uint8_t numSat2;

char dc1_status;
char dc2_status;
main_sst main_status;
//char master_date_time[20];

uint8_t TimeMessage[20];
uint8_t Rx3Buffer[RX3BUFFERSIZE];
uint32_t u1_halt,u2_halt;
uint32_t u3_halt = 0;
uint8_t gps1_pps = 0;
uint8_t gps2_pps = 0;
#define PeriodSaveRTC 63
uint8_t TimeToSaveRTC = PeriodSaveRTC;

uint8_t Flag_sweepLCD = 0;
uint8_t Flag_send2conn = 0;

volatile uint32_t t0=0,t1,t2,t3,t4=0,t5,tgps1,tgps2;
/*
//Lay thoi gian dua vao ban tin gui
*/
void dateAndTime(char* timeStr,char* dateStr)
{
	hours   = 10*(timeStr[0] -'0') + timeStr[1] - '0';
  minutes = 10*(timeStr[2] -'0') + timeStr[3] - '0';
  seconds = 10*(timeStr[4] -'0') + timeStr[5] - '0';
	days   = 10*(dateStr[0] -'0') + dateStr[1] - '0';
  months = 10*(dateStr[2] -'0') + dateStr[3] - '0';
  years  = 10*(dateStr[4] -'0') + dateStr[5] - '0';
	
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
void dateAndTimeRTC(void)
{
	laythoigian();
	TimeMessage[4] = '0' + hours /10;
	TimeMessage[5] = '0' + hours %10;
	TimeMessage[6] = '0' + minutes /10;	
	TimeMessage[7] = '0' + minutes %10;
	TimeMessage[8] = '0' + seconds /10;	
	TimeMessage[9] = '0' + seconds %10;	
	
	TimeMessage[10] = '0' + days /10;
	TimeMessage[11] = '0' + days %10;
	TimeMessage[12] = '0' + months /10;
	TimeMessage[13] = '0' + months %10;
	TimeMessage[14] = '0' + years /10;
	TimeMessage[15] = '0' + years %10;
	
	TimeMessage[16] = gps1_valid[0];
	TimeMessage[17] = gps2_valid[0];
}
/*
Moi giay goi ham nay 1 lan
*/
void oneSecondCount(void)
{
	//t3 = fractionOfSecond;
	u3_halt++;
	if(TimeToSaveRTC > 1) TimeToSaveRTC--;
	
	//Neu chua gui ban tin cho mach giao tiep => gui
	if(Flag_send2conn == 0)
	{
		//Gui thoi gian den mach giao tiep
		HAL_UART_Transmit(&huart3, TimeMessage, 20, 100);
	}
	else Flag_send2conn = 0;
	
	//neu chua quet LCD => quet LCD
	if(Flag_sweepLCD == 0)
	{
		//dateAndTime(gps1_time,gps1_date);
		Display_Time();
	}
	else Flag_sweepLCD = 0;
	
	scan_ADC();
	UpdateLed();
	
	//printf("t0: %d,t0-t4: %d,t1-t4: %d,t2-t0: %d,t3:%d g1 %d g2 %d \r\n",t0,t0-t4,t1-t4,t2-t0,t3,tgps1-t4,tgps2-t0);
	//printf("gps %d\r\n",tgps1);
	printf(" Sat1:%d Sat2:%d",numSat1,numSat2);
}
/*
Gui ban tin sang mach giao tiep va hien thi thong tin tren LCD
Save info to RTC
*/
void SendAnhDisplay(void)
{
	if(Flag_send2conn == 0)
	{
		//dateAndTime(gps1_time,gps1_date);
		dateAndTimeRTC();
		//Gui thoi gian den mach giao tiep
		HAL_UART_Transmit(&huart3, TimeMessage, 20, 100);
		Flag_send2conn = 1;
	}
	//
	if(TimeToSaveRTC < 6)
		{
			//save time to rtc
			TimeToSaveRTC = PeriodSaveRTC;
			if(seconds <56)
			{
				
				dateAndTime(gps1_time,gps1_date);//????? khi nao theo gps 2
				seconds++;	
				UpdateRtcTime();
				//printf("RTC save\r\n");
			}
			
		}
		
	if(Flag_sweepLCD == 0)
	{
	  Display_Time();
		Flag_sweepLCD = 1;
	}
}
/*
CHUONG TRINH CHINH
*/
void main_apps(void)
{
	if(gps1_pps == 1) 
	{
		//t1 = fractionOfSecond;
		
		gps1_pps = 0;
		SendAnhDisplay();
		
	}
	
	if(gps2_pps == 1) 
	{
		//t2 = fractionOfSecond;
		
		gps2_pps = 0;
		SendAnhDisplay();
	}
	
	
	checkUART();
}
/**
KHOI TAO
*/
void main_init(void)
{
	MX_ADC2_Init_MOD();
	printf("This code gen by STMcube;STM32G474RBT6@128MHz\r\n");
	HAL_Delay(100);
	LCD_Init();
	LCD_Clear();
	LCD_Gotoxy(0,0);
	LCD_Puts("STM32G474RBT6@128MHz");
	HAL_UART_Abort(&huart3);
	if (HAL_UART_Receive_IT(&huart3, (uint8_t *)Rx3Buffer, RX3BUFFERSIZE) != HAL_OK)
		{
			/* Transfer error in reception process */
			Error_Handler();
		}
	HAL_TIM_Base_Start_IT(&htim20);
	HAL_TIM_Base_Start(&htim3);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	/* Run the ADC calibration in single-ended mode */
  if (HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED) != HAL_OK)
  {
    /* Calibration Error */
    Error_Handler();
  }
	if (HAL_ADCEx_Calibration_Start(&hadc2, ADC_SINGLE_ENDED) != HAL_OK)
  {
    /* Calibration Error */
    Error_Handler();
  }
	
	GPS_Init();
	LCD_Clear();
	LCD_Gotoxy(0,0);
	LCD_Puts("GPS1:");
			
	LCD_Gotoxy(1,0);
	LCD_Puts("GPS2:");
	
	LCD_Gotoxy(0,22);
	LCD_Puts("RTC:");
	
	TimeMessage[0] = '$';
	TimeMessage[1] = 'G';
	TimeMessage[2] = 'P';
	TimeMessage[3] = 'S';
	TimeMessage[4] = '0';
	TimeMessage[5] = '0';
	TimeMessage[6] = '0';
	TimeMessage[7] = '0';
	TimeMessage[8] = '0';
	TimeMessage[9] = '0';
	TimeMessage[10] = '0';
	TimeMessage[11] = '0';
	TimeMessage[12] = '0';
	TimeMessage[13] = '0';
	TimeMessage[14] = '0';
	TimeMessage[15] = '0';
	TimeMessage[16] = '0';
	TimeMessage[17] = '0';
	TimeMessage[18] = '0';
	TimeMessage[19] = '0';
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
						LCD_Puts((char*)Rx3Buffer);
					}
							
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
void DisplayRTC_Time(void)
{			
		LCD_Gotoxy(0,26);
		//LCD_Puts("RTC:");
		//laythoigian();			
		lcdprintf("%02d%02d%02d %02d%02d%02d",time[2],time[1],time[0],time[4],time[5],time[6]);
		
}
/*
Hien thi cac thong tin tren LCD
*/
void Display_Time(void)
{			
			//GPS1
			LCD_Gotoxy(0,5);
			LCD_Putc(gps1_valid[0]);
			
			LCD_Gotoxy(0,6);
			LCD_Puts(gps1_time);

			LCD_Gotoxy(0,13);
			LCD_Puts(gps1_date);
	
			//LCD_Gotoxy(0,20);
			//LCD_Putc(numSat1+'0');
			//GPS2
			LCD_Gotoxy(1,5);
			LCD_Putc(gps2_valid[0]);
			
			LCD_Gotoxy(1,6);						
			LCD_Puts(gps2_time);
			

			LCD_Gotoxy(1,13);
			LCD_Puts(gps2_date);
      
			//LCD_Gotoxy(1,20);
			//LCD_Putc(numSat2+'0');
			
			DisplayRTC_Time();
}
//==============================================		
void UpdateLed(void)	
{
	if (gps1_valid[0] == 'A')
	{
		HAL_GPIO_WritePin(LED_GPS1_GPIO_Port, LED_GPS1_Pin, GPIO_PIN_SET);
	}
	else
		HAL_GPIO_WritePin(LED_GPS1_GPIO_Port, LED_GPS1_Pin, GPIO_PIN_RESET);

	if (gps2_valid[0] == 'A')
	{
		HAL_GPIO_WritePin(LED_GPS2_GPIO_Port, LED_GPS2_Pin, GPIO_PIN_SET);
	}
	else
		HAL_GPIO_WritePin(LED_GPS2_GPIO_Port, LED_GPS2_Pin, GPIO_PIN_RESET);
	
}


/**
  * @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
	* Neu co GPS, se co xung PPS tai thoi diem bat dau moi giay
  * If GPS avaiable, PPS pulse start at the start of a second
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  //Dong bo xung PPS GPS
	//Uu tien GPS1: neu c� pps thi lay cua GPS1 truoc
	//Neu GPS1 ko co PPS thi moi lay PPS cua GPS2
	if (GPIO_Pin == GPS1PPS_Pin)
  {
		//fractionOfSecond = 0;
		//t4 = fractionOfSecond;
		HAL_GPIO_WritePin(MAIN_PPS_GPIO_Port, MAIN_PPS_Pin, GPIO_PIN_SET);
		//printf("GPS1PPS\r\n");
		//main_status.gps_pps = 1;
		//main_status.gps1_stt = 'A';
		gps1_pps = 1;
  }
	if (GPIO_Pin == GPS2PPS_Pin)
  {
		//fractionOfSecond = 0;
		//t0 = fractionOfSecond;
		//HAL_GPIO_WritePin(MAIN_PPS_GPIO_Port, MAIN_PPS_Pin, GPIO_PIN_SET);
		//printf("GPS2PPS\r\n");
		gps2_pps = 1;
  }
}

void scan_ADC(void)
	{
		uint32_t raw;
		
			HAL_ADC_Start(&hadc1);
			HAL_ADC_PollForConversion(&hadc1, 100);
			raw = HAL_ADC_GetValue(&hadc1);
			HAL_ADC_Stop(&hadc1);
			// Convert to string and print
			//printf("PA1-ADC1-IN2 :%d\r\n", raw);
		//Tinh toan dien ap 12V
			main_status.v12V = (uint16_t)(k12v * (float)raw);
			//printf("Den ap 12V: %2.1f\r\n", (float)(main_status.v12V)/10.0);
		
			ADC_Select_CH17();
		HAL_ADC_Start(&hadc2);
		HAL_ADC_PollForConversion(&hadc2, 200);
		raw = HAL_ADC_GetValue(&hadc2);
		HAL_ADC_Stop(&hadc2);
		// Convert to string and print
		//printf("PA4-ADC2-IN17 :%d\r\n", raw);
		main_status.vin1x10 = (uint16_t)(kDCin1 * (float)raw);
		//printf("Den ap dc1: %2.1f\r\n", (float)(main_status.vin1x10)/10.0);	
		if (main_status.vin1x10 > 40)
		{
			dc1_status = '1';
		}
		else
			dc1_status = '0';		

		
		ADC_Select_CH3();
		HAL_ADC_Start(&hadc2);
		HAL_ADC_PollForConversion(&hadc2, 200);
		raw = HAL_ADC_GetValue(&hadc2);
		HAL_ADC_Stop(&hadc2);
		// Convert to string and print
		//printf("PA6-ADC2-IN3 :%d\r\n", raw);
		main_status.vin2x10 = (uint16_t)(kDCin2 * (float)raw);
		//printf("Den ap dc2: %2.1f\r\n", (float)(main_status.vin2x10)/10.0);		
		
		if (main_status.vin2x10 > 40)
		{
			dc2_status = '1';
		}
		else
			dc2_status = '0';
		
		
		ADC_Select_CH4();
		HAL_ADC_Start(&hadc2);
		HAL_ADC_PollForConversion(&hadc2, 200);
		raw = HAL_ADC_GetValue(&hadc2);
		HAL_ADC_Stop(&hadc2);
		// Convert to string and print
		//printf("PA7-ADC2-IN4 :%d\r\n", raw);
		
		ADC_Select_CH13();
		HAL_ADC_Start(&hadc2);
		HAL_ADC_PollForConversion(&hadc2, 200);
		raw = HAL_ADC_GetValue(&hadc2);
		HAL_ADC_Stop(&hadc2);
		// Convert to string and print
		//printf("PA5-ADC2-IN13 :%d\r\n", raw);
		
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
  hadc2.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV128;
  hadc2.Init.Resolution = ADC_RESOLUTION_12B;
  hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc2.Init.GainCompensation = 0;
  hadc2.Init.ScanConvMode = ADC_SCAN_ENABLE;
  hadc2.Init.EOCSelection = ADC_EOC_SEQ_CONV;
  hadc2.Init.LowPowerAutoWait = DISABLE;
  hadc2.Init.ContinuousConvMode = ENABLE;
  hadc2.Init.NbrOfConversion = 4;
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

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_3;
  sConfig.Rank = ADC_REGULAR_RANK_2;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_4;
  sConfig.Rank = ADC_REGULAR_RANK_3;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_13;
  sConfig.Rank = ADC_REGULAR_RANK_4;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC2_Init 2 */

  /* USER CODE END ADC2_Init 2 */

}

