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
 ADC_HandleTypeDef hadc2;
DMA_HandleTypeDef hdma_adc2;

I2C_HandleTypeDef hi2c2;

TIM_HandleTypeDef htim8;
TIM_HandleTypeDef htim20;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */
extern GPS_t GPS;
uint8_t u1Timeout=0;
uint8_t u2Timeout=0;
uint8_t u3Timeout=0;
uint32_t tim20ct=0;
/* Size of Reception buffer */
#define RX1BUFFERSIZE                      1000
#define RX2BUFFERSIZE                      1000
#define RX3BUFFERSIZE                      100
/* Buffer used for reception */
uint8_t Rx1Buffer[RX1BUFFERSIZE];
uint8_t Rx2Buffer[RX2BUFFERSIZE];
uint8_t Rx3Buffer[RX3BUFFERSIZE];
/* Variables for ADC conversion data computation to physical values */
__IO uint16_t uhADCxConvertedData_Voltage_mVolt = 0U;        /* Value of voltage on GPIO pin (on which is mapped ADC channel) calculated from ADC conversion data (unit: mV) */
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_I2C2_Init(void);
static void MX_ADC2_Init(void);
static void MX_TIM20_Init(void);
static void MX_TIM8_Init(void);
/* USER CODE BEGIN PFP */
static uint32_t GetPage(uint32_t Address);
static uint32_t GetBank(uint32_t Address);
static void storeValue(void);
static void loadValue(void);
//----------------------Phan giao tiep i2c-thoi gian thuc ---------------------//
unsigned char i2c_write[1];
char time[7],giaycu,ngay_al,thang_al,nhietdo,nhietdole;
unsigned char i2c_rv[19];
void BCD_Decoder()
{
	//printf("i2c_rv[3]: %d",i2c_rv[3]);
	for(char x=0;x<7;x++) time[x]=(i2c_rv[x] & 0x0f) + (i2c_rv[x]>>4)*10;
	//time[3] --; 
}
unsigned char BCD_Encoder(unsigned char temp)
{
	return ((temp/10)<<4)|(temp%10);
}
void laythoigian()
{
	HAL_I2C_Mem_Read(&hi2c2,0x68<<1,0,I2C_MEMADD_SIZE_8BIT,i2c_rv,19,1000); //read time
	BCD_Decoder(); //chuyen doi
	nhietdo = i2c_rv[17];
	nhietdole = i2c_rv[18]>>6;
	if(nhietdole == 1) nhietdole = 25;
	else if(nhietdole == 2) nhietdole = 5;
	else if(nhietdole == 3) nhietdole = 75;
	else nhietdole = 0;
}
void ghids(unsigned char add, unsigned char dat)
{
	i2c_write[0] = BCD_Encoder(dat);
	HAL_I2C_Mem_Write(&hi2c2,0x68<<1,add,I2C_MEMADD_SIZE_8BIT,i2c_write,1,1000); 
}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
uint8_t test=0;
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
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_I2C2_Init();
  MX_ADC2_Init();
  MX_TIM20_Init();
  MX_TIM8_Init();
  /* USER CODE BEGIN 2 */
	
  printf("This code gen by STMcube;STM32G473RBT@128MHz\r\n");
	HAL_Delay(100);
	LCD_Init();
	LCD_Clear();
	LCD_Gotoxy(0,0);
	LCD_Puts("STM32G473RBT@128MHz");
	LCD_Gotoxy(1,0);
	LCD_Puts("GPS: ok.");
	lcdprintf("IP: %s","192.168.22.153");
	storeValue();
	loadValue();
	HAL_Delay(1000);
	LCD_Clear();
	LCD_Gotoxy(0,0);
	laythoigian();
	if(time[3] == 6) LCD_Puts("Fri ");
			else	if(time[3] == 5) LCD_Puts("Thu ");
			else	if(time[3] == 4) LCD_Puts("Wed ");
			else	if(time[3] == 3) LCD_Puts("Tue ");
			else	if(time[3] == 2) LCD_Puts("Mon ");
			else	if(time[3] == 7) LCD_Puts("Sat ");
			else	if(time[3] == 1) LCD_Puts("Sun ");
			else LCD_Puts("??? ");
			
	lcdprintf("%2d/%2d/20%2d %2d:%2d:%2d",time[4],time[5],time[6],time[2],time[1],time[0]);
	/*ghids(0,0);
	ghids(1,05);
	ghids(2,11);
	ghids(3,6);
	ghids(4,25);
	ghids(5,3);
	ghids(6,22);
	*/
	/*
	if (HAL_UART_Receive_IT(&huart2, (uint8_t *)aRxBuffer, RXBUFFERSIZE) != HAL_OK)
  {
    // Transfer error in reception process 
    Error_Handler();
  }*/
	GPS_Init();
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
	HAL_UART_Abort(&huart3);
					if (HAL_UART_Receive_IT(&huart3, (uint8_t *)Rx3Buffer, RX3BUFFERSIZE) != HAL_OK)
						{
							/* Transfer error in reception process */
							Error_Handler();
						}
	HAL_TIM_Base_Start_IT(&htim20);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		if(tim20ct > 1000) {
			tim20ct = 0;
				
			//printf("1S\r\n");

		}
		if(tim20ct < 100) {
			
			HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED_CPU_GPIO_Port, LED_CPU_Pin, GPIO_PIN_RESET);
		}
		else
			{
				HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(LED_CPU_GPIO_Port, LED_CPU_Pin, GPIO_PIN_SET);
			}
		
		//HAL_Delay(50);
		
		//laythoigian();
		//printf("Time :%d %d %d %d %d %d %d, %d,%d\r\n",time[0],time[1],time[2],time[3],time[4],time[5],time[6],nhietdo,nhietdole);
		//HAL_Delay(50);
		//printf("utc_time %f\r\n",GPS.utc_time);
//		if(u1Timeout == 1) 
//			{
//				u1Timeout = 0;
//				//printf("U1 %s; \r\n",Rx1Buffer);
//				huart1.pRxBuffPtr = (uint8_t *)Rx1Buffer;
//				huart1.RxXferCount = RX1BUFFERSIZE;
//				memset(Rx1Buffer,0,RX1BUFFERSIZE);
//			}
			
//		if(u2Timeout == 1) 
//			{
//				u2Timeout = 0;
//				//printf("U2 %s; \r\n",Rx2Buffer);
//				huart2.pRxBuffPtr = (uint8_t *)Rx2Buffer;
//				huart2.RxXferCount = RX2BUFFERSIZE;
//				memset(Rx2Buffer,0,RX2BUFFERSIZE);
//			}
		if(u3Timeout == 1) 
			{
				u3Timeout = 0;
				printf("U3 %s; \r\n",Rx3Buffer);
				huart3.pRxBuffPtr = (uint8_t *)Rx3Buffer;
				huart3.RxXferCount = RX3BUFFERSIZE;
				memset(Rx3Buffer,0,RX3BUFFERSIZE);
			}
		
		//printf("ADC %d; \r\n",uhADCxConvertedData_Voltage_mVolt);

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
  hadc2.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc2.Init.Resolution = ADC_RESOLUTION_12B;
  hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc2.Init.GainCompensation = 0;
  hadc2.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc2.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc2.Init.LowPowerAutoWait = DISABLE;
  hadc2.Init.ContinuousConvMode = ENABLE;
  hadc2.Init.NbrOfConversion = 1;
  hadc2.Init.DiscontinuousConvMode = DISABLE;
  hadc2.Init.ExternalTrigConv = ADC_EXTERNALTRIG_T8_TRGO;
  hadc2.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
  hadc2.Init.DMAContinuousRequests = ENABLE;
  hadc2.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
  hadc2.Init.OversamplingMode = DISABLE;
  if (HAL_ADC_Init(&hadc2) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_2;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
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
  htim8.Init.Prescaler = 149;
  htim8.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim8.Init.Period = 499;
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
  huart3.Init.BaudRate = 115200;
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
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMAMUX1_CLK_ENABLE();
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

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
				
				printf("buffer full!\r\n");
				HAL_UART_Receive_IT(&huart3, (uint8_t *)Rx3Buffer, RX3BUFFERSIZE);
				printf("U3 %s; \r\n",Rx3Buffer);
				huart3.pRxBuffPtr = (uint8_t *)Rx3Buffer;
				huart3.RxXferCount = RX3BUFFERSIZE;
				memset(Rx3Buffer,0,RX3BUFFERSIZE);
			}
			if(UartHandle == &huart1) 
			{
				GPS_UART_CallBack();
			}
			if(UartHandle == &huart1) 
			{
				GPS2_UART_CallBack();
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
	printf("Error_Handler");
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
