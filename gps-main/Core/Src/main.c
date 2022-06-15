/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include "lcd.h"
#include "gps.h"
#include "storevalue.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

uint32_t t,t1,t2,t3,t4;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* Definitions of environment analog values */
  /* Value of analog reference voltage (Vref+), connected to analog voltage   */
  /* supply Vdda (unit: mV).                                                  */
  #define VDDA_APPLI                       (3287U)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
 ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc2;

I2C_HandleTypeDef hi2c2;

TIM_HandleTypeDef htim8;
TIM_HandleTypeDef htim20;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */
main_sst main_status;
extern GPS_t GPS;
uint8_t u1Timeout=0;
uint8_t u2Timeout=0;
uint8_t u3Timeout=0;
uint32_t tim20ct=0;
uint8_t pps_down;
uint8_t TimeMessage[20];
/* Buffer used for reception */
uint8_t Rx1Buffer[RX1BUFFERSIZE];
uint8_t Rx2Buffer[RX2BUFFERSIZE];
uint8_t Rx3Buffer[RX3BUFFERSIZE];
uint32_t u1_halt,u2_halt,u3_halt;
/* Variables for ADC conversion data computation to physical values */
__IO uint16_t uhADCxConvertedData_Voltage_mVolt = 0U;        /* Value of voltage on GPIO pin (on which is mapped ADC channel) calculated from ADC conversion data (unit: mV) */
uint8_t updateLCD = 1;
extern char ds3231_reg[7];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_I2C2_Init(void);
static void MX_ADC2_Init(void);
static void MX_TIM20_Init(void);
static void MX_TIM8_Init(void);
static void MX_ADC1_Init(void);
/* USER CODE BEGIN PFP */
static uint32_t GetPage(uint32_t Address);
static uint32_t GetBank(uint32_t Address);
static void storeValue(void);
static void loadValue(void);


void scan_ADC(void);

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

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
GPSClock_t GPS_clock;
void check(uint8_t *gps)
{
	printf("%c",gps[0]);
	if(gps[0] =='G') printf("ok\r\n");
	else gps++;
	printf("%c",gps[0]);
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
uint8_t test=0;
	uint32_t raw;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_I2C2_Init();
  MX_ADC2_Init();
  MX_TIM20_Init();
  MX_TIM8_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */
	
  printf("This code gen by STMcube;STM32G473RBT@128MHz\r\n");
	HAL_Delay(100);
	LCD_Init();
	LCD_Clear();
	LCD_Gotoxy(0,0);
	LCD_Puts("GPS clock master");
	//LCD_Gotoxy(1,0);
	//LCD_Puts("GPS: ok.");
	//lcdprintf("IP: %s","192.168.22.153");
	//storeValue();
	//loadValue();
	HAL_Delay(1000);
	LCD_Clear();
	LCD_Gotoxy(0,0);
	
	laythoigian();
	/*
	ghids(14,0);//1HZ out SQW
	//ghids(DS_SECOND_REG,0);
	//ghids(DS_MIN_REG,05);
	//ghids(DS_HOUR_REG,11);
	//ghids(DS_DAY_REG,6);
	ghids(DS_DATE_REG,14);
	ghids(DS_MONTH_REG,6);
	ghids(DS_YEAR_REG,22);
	*/
	/*
	if (HAL_UART_Receive_IT(&huart2, (uint8_t *)aRxBuffer, RXBUFFERSIZE) != HAL_OK)
  {
    // Transfer error in reception process 
    Error_Handler();
  }*/
	//GPS_Init();
	HAL_GPIO_WritePin(LED_GPS2_GPIO_Port, LED_GPS2_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED_GPS1_GPIO_Port, LED_GPS1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED_CPU_GPIO_Port, LED_CPU_Pin, GPIO_PIN_SET);
	
	//uhADCxConvertedData_Voltage_mVolt = VAR_CONVERTED_DATA_INIT_VALUE;
	/* Run the ADC calibration in single-ended mode */
//  if (HAL_ADCEx_Calibration_Start(&hadc2, ADC_SINGLE_ENDED) != HAL_OK)
//  {
//    /* Calibration Error */
//    //Error_Handler();
//  }
	/*## Start ADC conversions ###############################################*/
  /* Start ADC group regular conversion with DMA */
//  if (HAL_ADC_Start_DMA(&hadc2,
//                        (uint32_t *)&uhADCxConvertedData_Voltage_mVolt,
//                        1
//                       ) != HAL_OK)
//  {
//    /* ADC conversion start error */
//    //Error_Handler();
//  }  
	//printf("ADC PA1 %d; \r\n",uhADCxConvertedData_Voltage_mVolt);
	//HAL_ADC_Start(&hadc2);
	//uhADCxConvertedData_Voltage_mVolt = HAL_ADC_GetValue(&hadc2);
	//printf("ADC PA1 2%d; \r\n",uhADCxConvertedData_Voltage_mVolt);
	
	
	/* Start Timer trigger */
  //if (HAL_TIM_Base_Start(&htim20) != HAL_OK)
  //{
    /* Counter enable error */
    //Error_Handler();
  //}
//	if (HAL_TIM_Base_Start(&htim8) != HAL_OK)
//  {
//    /* Counter enable error */
//    Error_Handler();
//  }
	//HAL_Delay(100);
//	HAL_UART_Abort(&huart1);
//	if (HAL_UART_Receive_IT(&huart1, (uint8_t *)Rx1Buffer, RX1BUFFERSIZE) != HAL_OK)
//	{
//		/* Transfer error in reception process */
//		Error_Handler();
//	}
//	HAL_UART_Abort(&huart2);
//	if (HAL_UART_Receive_IT(&huart2, (uint8_t *)Rx2Buffer, RX2BUFFERSIZE) != HAL_OK)
//  {
//    /* Transfer error in reception process */
//    Error_Handler();
//  }

  LCD_Clear();
	LCD_Gotoxy(0,0);
	LCD_Puts("GPS1:");
	LCD_Gotoxy(0,20);
	LCD_Puts("RTC: ");
	LCD_Gotoxy(1,0);
	LCD_Puts("GPS2:");
	//LCD_Gotoxy(1,20);
	//LCD_Puts("IP: ");
	//Tim phuong an xu ly treo uart!!!
	HAL_UART_Abort(&huart3);
	if (HAL_UART_Receive_IT(&huart3, (uint8_t *)Rx3Buffer, RX3BUFFERSIZE) != HAL_OK)
		{
			/* Transfer error in reception process */
			Error_Handler();
		}
	HAL_TIM_Base_Start_IT(&htim20);
	//HAL_TIM_Base_Start_IT(&htim8);
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
	GPS_clock.gps1_valid = 'x';
	GPS_clock.gps2_valid = 'x';
	//GPS_clock.gps1_time[6] = 0;
	//GPS_clock.gps2_time[6] = 0;
	HAL_UART_Abort(&huart1);
	HAL_UART_Receive_IT(&huart1, Rx1Buffer, RX1BUFFERSIZE);
	HAL_UART_Abort(&huart2);
	HAL_UART_Receive_IT(&huart2, Rx2Buffer, RX2BUFFERSIZE);
  fractionOfSecond = 0;
	GPS_clock.gps1_lock = 0;
	GPS_clock.gps2_lock = 0;
	GPS_clock.update_rtc = 100;
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
	//Neu tin hieu ve tinh kem, thoi gian tu bo thu GPS se ko on dinh!!!
	u1_halt = 0;
	u2_halt = 0;
	u3_halt = 0;
	pps_down = 0;
	while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		//t = fractionOfSecond;
		check_uart();
		//t1 = fractionOfSecond;
		if(updateLCD == 1) 
		{
			updateLCD = 0;
			//Neu co GPS : update theo gps de ko bi tre hien thi so voi gps
			//Neu ko co GPS update the RTC
			sweep_LCD();
			HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
		}
		//t2 = fractionOfSecond;
		if(tim20ct > 990) {
			tim20ct = 0;
			scan_ADC();	
			
			//printf("1S\r\n");
			//laythoigian();
			
			//Dem thoi gian ko hoat dong cua U1,U2,U3
			u1_halt++;
			u2_halt++;
			u3_halt++;
			if(pps_down < 3) pps_down++;
			if(pps_down > 2) 
			{
				updateLCD =1;
				//Ko co pps thi coi nhu GPS1 chua lock
				GPS_clock.gps1_lock = 0;
			}
			
		}
		
		if(tim20ct < 100) {
			
			
			//HAL_GPIO_WritePin(LED_CPU_GPIO_Port, LED_CPU_Pin, GPIO_PIN_RESET);
			//HAL_GPIO_WritePin(LED_GPS1_GPIO_Port, LED_GPS1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED_GPS2_GPIO_Port, LED_GPS2_Pin, GPIO_PIN_RESET);
		}
		else
			{
				//HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
				//HAL_GPIO_WritePin(LED_CPU_GPIO_Port, LED_CPU_Pin, GPIO_PIN_SET);
				//HAL_GPIO_WritePin(LED_GPS1_GPIO_Port, LED_GPS1_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(LED_GPS2_GPIO_Port, LED_GPS2_Pin, GPIO_PIN_SET);
			}

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
  RCC_OscInitStruct.PLL.PLLN = 16;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_MultiModeTypeDef multimode = {0};
  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV128;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.GainCompensation = 0;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
  hadc1.Init.OversamplingMode = DISABLE;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure the ADC multi-mode
  */
  multimode.Mode = ADC_MODE_INDEPENDENT;
  if (HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_2;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_92CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief ADC2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC2_Init(void)
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

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.Timing = 0x20A0C4DF;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
  * @brief TIM8 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM8_Init(void)
{

  /* USER CODE BEGIN TIM8_Init 0 */

  /* USER CODE END TIM8_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM8_Init 1 */

  /* USER CODE END TIM8_Init 1 */
  htim8.Instance = TIM8;
  htim8.Init.Prescaler = 1280-1;
  htim8.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim8.Init.Period = 9999;
  htim8.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim8.Init.RepetitionCounter = 0;
  htim8.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim8) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim8, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_ENABLE;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim8, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM8_Init 2 */

  /* USER CODE END TIM8_Init 2 */

}

/**
  * @brief TIM20 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM20_Init(void)
{

  /* USER CODE BEGIN TIM20_Init 0 */

  /* USER CODE END TIM20_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM20_Init 1 */

  /* USER CODE END TIM20_Init 1 */
  htim20.Instance = TIM20;
  htim20.Init.Prescaler = 128-1;
  htim20.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim20.Init.Period = 999;
  htim20.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim20.Init.RepetitionCounter = 0;
  htim20.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim20) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim20, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim20, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM20_Init 2 */

  /* USER CODE END TIM20_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart2, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart2, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 9600;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart3, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart3, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, MAIN_PPS_Pin|GPIO_PIN_6|LCD_D5_Pin|LCD_D6_Pin
                          |LCD_D7_Pin|LED_GPS2_Pin|LED_GPS1_Pin|LED_CPU_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LCD_RS_Pin|LCD_RW_Pin|LCD_EN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : MAIN_PPS_Pin PC6 LCD_D5_Pin LCD_D6_Pin
                           LCD_D7_Pin LED_GPS2_Pin LED_GPS1_Pin LED_CPU_Pin */
  GPIO_InitStruct.Pin = MAIN_PPS_Pin|GPIO_PIN_6|LCD_D5_Pin|LCD_D6_Pin
                          |LCD_D7_Pin|LED_GPS2_Pin|LED_GPS1_Pin|LED_CPU_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : LED_Pin */
  GPIO_InitStruct.Pin = LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LCD_RS_Pin LCD_RW_Pin LCD_EN_Pin */
  GPIO_InitStruct.Pin = LCD_RS_Pin|LCD_RW_Pin|LCD_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PB12 PB13 PB14 PB15
                           PB4 PB5 PB6 PB7
                           PB9 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7
                          |GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : GPS2PPS_Pin GPS1PPS_Pin */
  GPIO_InitStruct.Pin = GPS2PPS_Pin|GPS1PPS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PB3 */
  GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 4 */

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
	//Uu tien GPS1: neu có pps thi lay cua GPS1 truoc
	//Neu GPS1 ko co PPS thi moi lay PPS cua GPS2
	if (GPIO_Pin == GPS1PPS_Pin)
  {
		//t = fractionOfSecond;
		//fractionOfSecond = 0;
		HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(MAIN_PPS_GPIO_Port, MAIN_PPS_Pin, GPIO_PIN_SET);
		//day la thoi diem bat dau cua giay ( start of second) 
		// - cap nhat LCD
		// - cap nhat thoi gian
		// - gui ban tin toi mach giao tiep
		//GPS_clock.gps1_lock = 0
		if(GPS_clock.update_rtc > 1) GPS_clock.update_rtc --;
		if(GPS_clock.gps1_lock == 1)
		{
			if(GPS_clock.update_rtc == 1)//luu thoi gian vao rtc
			{
				GPS_clock.update_rtc = 100;
				
				ghids(14,0);//1HZ out SQW
				ghids(DS_SECOND_REG,seconds);
				ghids(DS_MIN_REG,minutes);
				ghids(DS_HOUR_REG,hours);
				
				ghids(DS_DATE_REG,days);
				ghids(DS_MONTH_REG,months);
				ghids(DS_YEAR_REG,years);
			}
			//Gui thoi gian den mach giao tiep
			HAL_UART_Transmit(&huart3, TimeMessage, 20, 100);
		}
		
		updateLCD =1;
		pps_down = 0;
		//printf("GPS1PPS\r\n");
  }
	else if (GPIO_Pin == GPS2PPS_Pin)
  {
		HAL_GPIO_WritePin(MAIN_PPS_GPIO_Port, MAIN_PPS_Pin, GPIO_PIN_SET);
		//printf("GPS2PPS\r\n");
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
		
		
		ADC_Select_CH13();
		HAL_ADC_Start(&hadc2);
		HAL_ADC_PollForConversion(&hadc2, 200);
		raw = HAL_ADC_GetValue(&hadc2);
		HAL_ADC_Stop(&hadc2);
		// Convert to string and print
		//printf("PA5-ADC2-IN13 :%d\r\n", raw);
		
		
		ADC_Select_CH3();
		HAL_ADC_Start(&hadc2);
		HAL_ADC_PollForConversion(&hadc2, 200);
		raw = HAL_ADC_GetValue(&hadc2);
		HAL_ADC_Stop(&hadc2);
		// Convert to string and print
		//printf("PA6-ADC2-IN3 :%d\r\n", raw);
		
		
		ADC_Select_CH4();
		HAL_ADC_Start(&hadc2);
		HAL_ADC_PollForConversion(&hadc2, 200);
		raw = HAL_ADC_GetValue(&hadc2);
		HAL_ADC_Stop(&hadc2);
		// Convert to string and print
		//printf("PA7-ADC2-IN4 :%d\r\n", raw);
	}
#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
	/* Place your implementation of fputc here */
	/* e.g. write a character to the USART */
	HAL_UART_Transmit(&huart3, (uint8_t *)&ch, 1, 100);
	return ch;
}
//khi nao buffer full thi no se goi ham nay
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
  
    if(UartHandle == &huart3) 
			{
				
				//printf("buffer full!\r\n");
				HAL_UART_Receive_IT(&huart3, (uint8_t *)Rx3Buffer, RX3BUFFERSIZE);
				//printf("U3 %s; \r\n",Rx3Buffer);
				huart3.pRxBuffPtr = (uint8_t *)Rx3Buffer;
				huart3.RxXferCount = RX3BUFFERSIZE;
				memset(Rx3Buffer,0,RX3BUFFERSIZE);
			}
			if(UartHandle == &huart1) 
			{
				//GPS_UART_CallBack();
				HAL_UART_Abort(GPS1_USART);
				HAL_UART_Receive_IT(&huart1, Rx1Buffer, RX1BUFFERSIZE);
			}
			if(UartHandle == &huart1) 
			{
				//GPS2_UART_CallBack();
			}
				        
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle)
	{
		if(UartHandle == &huart3) 
			{
				printf("HAL_UART_ErrorCallback!\r\n");
				HAL_UART_Abort(&huart3);
				HAL_UART_Receive_IT(&huart3, (uint8_t *)Rx3Buffer, RX3BUFFERSIZE);
			}
	}
static void loadValue(void)
{
	__IO uint32_t MemoryProgramStatus = 0;
	__IO uint32_t data32 = 0;
	uint32_t Address = 0;
	/* Check if the programmed data is OK
      MemoryProgramStatus = 0: data programmed correctly
      MemoryProgramStatus != 0: number of words not programmed correctly ******/

  MemoryProgramStatus = 0x0;

  
    data32 = *(__IO uint32_t *)FLASH_USER_START_ADDR;
    
    if (data32 != DATA_32)
    {
      MemoryProgramStatus++;
    }
    
	
	/*Check if there is an issue to program data*/
  if (MemoryProgramStatus == 0)
  {
    /* No error detected. Switch on LED1*/
    //BSP_LED_On(LED1);
		printf("Memory is ok\r\n");
  }
  else
  {
    /* Error detected. Switch on LED3*/
    //BSP_LED_On(LED3);
		printf("Error detected \r\n");
		return;
  }
	
	data32 = *(__IO uint32_t *)(FLASH_USER_START_ADDR+4);
	//printf("first data32 %x\r\n",data32);
	
	//gWIZNETINFO.ip[3] = (uint8_t)data32;
	//gWIZNETINFO.ip[2] = (uint8_t)(data32>>8);
	//gWIZNETINFO.ip[1] = (uint8_t)(data32>>16);
	//gWIZNETINFO.ip[0] = (uint8_t)(data32>>24);	
	//printf("Load IP: %d.%d.%d.%d\r\n",gWIZNETINFO.ip[0],gWIZNETINFO.ip[1],gWIZNETINFO.ip[2],gWIZNETINFO.ip[3]);
		
	data32 = *(__IO uint32_t *)(FLASH_USER_START_ADDR+8);
	printf("last data32 %x\r\n",data32);
		data32 = *(__IO uint32_t *)(FLASH_USER_START_ADDR+12);
	printf("first data32 %x\r\n",data32);
	data32 = *(__IO uint32_t *)(FLASH_USER_START_ADDR+16);
	printf("last data32 %x\r\n",data32);
		data32 = *(__IO uint32_t *)(FLASH_USER_START_ADDR+20);
	printf("first data32 %x\r\n",data32);
}

static void storeValue(void)
{
	uint32_t FirstPage = 0, NbOfPages = 0, BankNumber = 0,PageError = 0;
	static FLASH_EraseInitTypeDef EraseInitStruct;
	uint64_t temp;
	/* Unlock the Flash to enable the flash control register access *************/
  HAL_FLASH_Unlock();
	/* Clear OPTVERR bit set on virgin samples */
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);
	/* Erase the user Flash area
    (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/
  
  /* Get the 1st page to erase */
  FirstPage = GetPage(FLASH_USER_START_ADDR);

  /* Get the number of pages to erase from 1st page */
  NbOfPages = GetPage(FLASH_USER_END_ADDR) - FirstPage + 1;

  /* Get the bank */
  BankNumber = GetBank(FLASH_USER_START_ADDR);
  
  /* Fill EraseInit structure*/
  EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
  EraseInitStruct.Banks       = BankNumber;
  EraseInitStruct.Page        = FirstPage;
  EraseInitStruct.NbPages     = NbOfPages;
	
	
	if (HAL_FLASHEx_Erase(&EraseInitStruct, &PageError) != HAL_OK)
  {
    /*
      Error occurred while page erase.
      User can add here some code to deal with this error.
      PageError will contain the faulty page and then to know the code error on this page,
      user can call function 'HAL_FLASH_GetError()'
    */
		printf("Error occurred while page erase.\r\n");
    
  }
	
	/* Program the user Flash area word by word
    (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/
	
	//temp = DATA_32;
	//temp = temp<<32;
	temp  = 0;
	//temp |= (gWIZNETINFO.ip[0] <<24) |(gWIZNETINFO.ip[1] <<16)|(gWIZNETINFO.ip[2] <<8)|gWIZNETINFO.ip[3];
  //printf("temp : %x\r\n",(gWIZNETINFO.ip[0] <<24) +(gWIZNETINFO.ip[1] <<16)+(gWIZNETINFO.ip[2] <<8)+gWIZNETINFO.ip[3]);
	temp = DATA_32 | (temp<<32);
    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, FLASH_USER_START_ADDR, temp) == HAL_OK)
    {
			printf("Saved!\r\n");
    }
   else
    {
      /* Error occurred while writing data in Flash memory.
         User can add here some code to deal with this error */
			printf("Error occurred while writing data in Flash memory\r\n");
      
    }
		
		temp  = 0xFFFFFF01C0A816FC;//sn|fw
		//temp |= (gWIZNETINFO.ip[0] <<24) |(gWIZNETINFO.ip[1] <<16)|(gWIZNETINFO.ip[2] <<8)|gWIZNETINFO.ip[3];
		//printf("temp : %x\r\n",(gWIZNETINFO.ip[0] <<24) +(gWIZNETINFO.ip[1] <<16)+(gWIZNETINFO.ip[2] <<8)+gWIZNETINFO.ip[3]);
		
		if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, (FLASH_USER_START_ADDR+8), temp) == HAL_OK)
    {
			printf("Saved!\r\n");
    }
   else
    {
      /* Error occurred while writing data in Flash memory.
         User can add here some code to deal with this error */
			printf("Error occurred while writing data in Flash memory\r\n");
      
    }
		
		temp  = 0x0808080801020304;//sn|fw
		//temp |= (gWIZNETINFO.ip[0] <<24) |(gWIZNETINFO.ip[1] <<16)|(gWIZNETINFO.ip[2] <<8)|gWIZNETINFO.ip[3];
		//printf("temp : %x\r\n",(gWIZNETINFO.ip[0] <<24) +(gWIZNETINFO.ip[1] <<16)+(gWIZNETINFO.ip[2] <<8)+gWIZNETINFO.ip[3]);
		
		if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, (FLASH_USER_START_ADDR+16), temp) == HAL_OK)
    {
			printf("Saved!\r\n");
    }
   else
    {
      /* Error occurred while writing data in Flash memory.
         User can add here some code to deal with this error */
			printf("Error occurred while writing data in Flash memory\r\n");
      
    }
  
	
	/* Lock the Flash to disable the flash control register access (recommended
     to protect the FLASH memory against possible unwanted operation) *********/
  HAL_FLASH_Lock();
}
/**
  * @brief  Gets the page of a given address
  * @param  Addr: Address of the FLASH Memory
  * @retval The page of a given address
  */
static uint32_t GetPage(uint32_t Addr)
{
  uint32_t page = 0;

  if (Addr < (FLASH_BASE + FLASH_BANK_SIZE))
  {
    /* Bank 1 */
    page = (Addr - FLASH_BASE) / FLASH_PAGE_SIZE;
  }
  else
  {
    /* Bank 2 */
    page = (Addr - (FLASH_BASE + FLASH_BANK_SIZE)) / FLASH_PAGE_SIZE;
  }

  return page;
}

/**
  * @brief  Gets the bank of a given address
  * @param  Addr: Address of the FLASH Memory
  * @retval The bank of a given address
  */
static uint32_t GetBank(uint32_t Addr)
{
  return FLASH_BANK_1;
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
	//printf("Error_Handler");
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
