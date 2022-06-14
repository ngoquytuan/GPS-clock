/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <time.h>
#include "stdio.h"
#include "wizchip_conf.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
slave_status slave_clock;
time_t timenow = 1651133756;
struct tm* timeinfo;
struct tm currtime;
uint8_t days = 0;
uint8_t months = 0;
uint8_t years = 0;
uint8_t hours = 0;
uint8_t minutes = 0;
uint8_t seconds = 0;
//Kiem tra RTC co hoat dong khong
uint8_t rtc_timeout = 30;

int8_t haveSignalFromRS485 = NO_SIGNAL;
int8_t stableSignal = SIGNAL_FROM_MASTER_BAD;
int8_t count_Stable_signal = 0;
int8_t timeOutLostSignal = 0;
//Thoi gian luu gio tu RS485 vao RTC
int16_t timeSaveRS485_to_RTC = 60;
extern uint8_t LEDintensity;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
 I2C_HandleTypeDef hi2c3;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
#ifdef SLAVE_WALL
wiz_NetInfo myipWIZNETINFO = { .mac = {0x0A, 0x08, 0xDC,0x4F, 0xEB, 0x6F},
															 .ip = {192, 168, 6, 101},
															 .sn = {255,255,255,1},
															 .gw = {192, 168, 6, 1},
															 .dns = {8,8,8,8},
															 .dhcp = NETINFO_STATIC };
#endif
#ifdef SLAVE_CONSOLE
wiz_NetInfo myipWIZNETINFO = { .mac = {0x0A, 0x08, 0xDC,0x4F, 0xEB, 0x5F},
															 .ip = {192, 168, 6, 99},
															 .sn = {255,255,255,1},
															 .gw = {192, 168, 6, 1},
															 .dns = {8,8,8,8},
															 .dhcp = NETINFO_STATIC };
#endif
uint8_t u2Timeout = 0;
uint32_t timct=0;
															 /* Size of Reception buffer */
#define RXBUFFERSIZE                      100
/* Buffer used for reception */
uint8_t aRxBuffer[RXBUFFERSIZE];
															 
extern unsigned char const LEDcode[];
//NTP client
extern uint16_t RetrySend ; //60 giay
extern uint16_t sycnPeriod ;// 1 gio
//extern uint8_t console_stt;															 
void SNTP_init(void);		
int8_t SNTP_run(void);															 
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_TIM3_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_I2C3_Init(void);
static void MX_TIM4_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
volatile uint16_t phystatus_check_cnt;


void main_message_handle(void);
void uart2_processing(void)
{
	if(u2Timeout == 1) 
			{
				u2Timeout = 0;
				HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
				//main_message_handle();
				//Truyen ban tin cho cac dong ho slave
				//HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_SET);
				//HAL_UART_Transmit(&huart2, aRxBuffer, 20, 100);
				main_message_handle();
				//HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_RESET);
				//printf("aRxBuffer %s; \r\n",aRxBuffer);
				huart2.pRxBuffPtr = (uint8_t *)aRxBuffer;
				huart2.RxXferCount = RXBUFFERSIZE;
				memset(aRxBuffer,0,RXBUFFERSIZE);
			}
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  uint32_t i;
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
  MX_SPI1_Init();
  MX_TIM3_Init();
  MX_USART2_UART_Init();
  MX_I2C3_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
	
	//HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_SET);
	//printf("This code gen by STMcube STM32G474@128MHz\r\n");
	//HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_RESET);
	
	LEDintensity = 1;
	loadValue();
	storeValue();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	
	display_init_check();
	slave_clock.sync_status = LOCAL;
	//MAX7219_SendAddrDat (0x0a,LEDintensity); //intensity
  //HAL_GPIO_ReadPin(SQW_GPIO_Port,SQW_Pin); GPIO_PIN_SET
	
	
	//laythoigian();
	
	//HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_SET);
	//printf("Time :%dh%dm%ds;%d %d/%d/%d , %d\r\n",ds3231_reg[2],ds3231_reg[1],ds3231_reg[0],ds3231_reg[3],ds3231_reg[4],ds3231_reg[5],ds3231_reg[6],nhietdo);
	//HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_RESET);
	
	
  //RTC_factory_RST();
	
	//HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_SET);
	w5500_lib_init();
	//HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_RESET);
	SNTP_init();
	HAL_UART_Abort(&huart2);
	if (HAL_UART_Receive_IT(&huart2, (uint8_t *)aRxBuffer, RXBUFFERSIZE) != HAL_OK)
  {
    /* Transfer error in reception process */
    Error_Handler();
  }
	HAL_TIM_Base_Start_IT(&htim3);
	laythoigian();
/** Cac che do hoat dong cua dong ho slave
	*  Hoat dong theo RTC hoac NTP client hoac RS485
	*  Uu tien 1: NTP client, neu khong co day mang, khong ket noi dc server thi cho tin hieu rs485, neu cung ko co tin hieu rs485 tiep tuc chay local
	*  Chay local voi RTC cho toi khi co rs458 hoac ntp client thi chuyen du lieu, neu du lieu on dinh thi ghi du lieu sang rtc
	*  Neu RTC ko co hoac loi??
	*  IC RTC hong => NO RTC
  *  IC RTC het pin => NO BATTERY 	
	*/
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		uart2_processing();
		if(timct > 990) {
			timct = 0;
//			HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
//			HAL_Delay(50);
//			HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
//			HAL_Delay(50);
			
//			laythoigian();
//			HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_SET);
//			printf("Time :%dh%dm%ds;%d %d/%d/%d\r\n",ds3231_reg[2],ds3231_reg[1],ds3231_reg[0],ds3231_reg[3],ds3231_reg[4],ds3231_reg[5],ds3231_reg[6]);
//			HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_RESET);
			//laythoigian();
			if(timeOutLostSignal) timeOutLostSignal--;
			else haveSignalFromRS485 = NO_SIGNAL;
			
			if(haveSignalFromRS485 == NO_SIGNAL)
			{
#ifdef SLAVE_WALL			
			day_display();
#endif
			console_display();
			console_blink();
			}
			//seconds++;
			RetrySend++;
			sycnPeriod++;
			
			
			
			//kiem tra tinh on dinh cua du lieu GPS
				 
			//slave_clock.sync_status = LOCAL;
			
			if(stableSignal == SIGNAL_FROM_MASTER_OK) count_Stable_signal--; 
      if(count_Stable_signal == 0) 
				{
					stableSignal = SIGNAL_FROM_MASTER_BAD;
					haveSignalFromRS485 = NO_SIGNAL;
					laythoigian();
					slave_clock.sync_status = LOCAL;
				}
			
			if((timeSaveRS485_to_RTC > 1) && (stableSignal == SIGNAL_FROM_MASTER_OK)) timeSaveRS485_to_RTC --;
			
			if(rtc_timeout > 1) rtc_timeout --;
			if(rtc_timeout == 1) 
			{
				slave_clock.rtc_status = RTC_OUT_OF_BATTERY;
//		HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_SET);
//		printf("RTC out of battery\r\n");
//		HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_RESET);
			}
			if(rtc_timeout == 0) 
			{
				slave_clock.rtc_status = RTC_FINE;
			}
		}
		uart2_processing();
		//HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_SET);
		SNTP_run();
		//HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_RESET);
		uart2_processing();
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
  RCC_OscInitStruct.PLL.PLLN = 32;
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
  * @brief I2C3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C3_Init(void)
{

  /* USER CODE BEGIN I2C3_Init 0 */

  /* USER CODE END I2C3_Init 0 */

  /* USER CODE BEGIN I2C3_Init 1 */

  /* USER CODE END I2C3_Init 1 */
  hi2c3.Instance = I2C3;
  hi2c3.Init.Timing = 0x20A0C4DF;
  hi2c3.Init.OwnAddress1 = 0;
  hi2c3.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c3.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c3.Init.OwnAddress2 = 0;
  hi2c3.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c3.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c3.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c3) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c3, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c3, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C3_Init 2 */

  /* USER CODE END I2C3_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 128-1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 999;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 128-1;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 999;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */

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
  HAL_GPIO_WritePin(GPIOC, PC1_Pin|PC3_Pin|CLK2_Pin|DIN2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, RD485_Pin|LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, SCSn_Pin|RSTn_Pin|LED_LOAD_Pin|LED_CLK_Pin
                          |LED_DIN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : PC1_Pin PC3_Pin CLK2_Pin DIN2_Pin */
  GPIO_InitStruct.Pin = PC1_Pin|PC3_Pin|CLK2_Pin|DIN2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : RD485_Pin LED_Pin */
  GPIO_InitStruct.Pin = RD485_Pin|LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : SCSn_Pin RSTn_Pin */
  GPIO_InitStruct.Pin = SCSn_Pin|RSTn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : INTn_Pin FR_Pin */
  GPIO_InitStruct.Pin = INTn_Pin|FR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : SQW_Pin */
  GPIO_InitStruct.Pin = SQW_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(SQW_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LED_LOAD_Pin LED_CLK_Pin LED_DIN_Pin */
  GPIO_InitStruct.Pin = LED_LOAD_Pin|LED_CLK_Pin|LED_DIN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

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
  //Dong bo xung PPS voi mach main GPS, moi lan co xung la thoi diem bat dau cua giay
	//sync the start of a second by pps signal
	
	if (GPIO_Pin == INTn_Pin)
  {
		//Nhan duoc ban tin NTP, xu ly thoi gian va phan hoi
		//Receied NTP message, processing and respond
  }
	if (GPIO_Pin == FR_Pin)
  {
		//factory reset
//		HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_SET);
//		printf("Factory reset\r\n");
//		HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_RESET);
		chinhdosang();
  }
	if (GPIO_Pin == SQW_Pin)
  {
		//RTC second
//		HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_SET);
//		printf("RTC\r\n");
//		HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_RESET);
		//RTC hoat dong binh thuong
		rtc_timeout = 0;
		
				if(haveSignalFromRS485 == NO_SIGNAL)
			{
				seconds++;
				if(seconds > 59) 
				{
				//					seconds = 0;
				//					minutes++;
				//					if(minutes > 59) 
				//					{
				//						minutes = 0;
				//						hours ++;
				//						if(hours > 23) hours = 0;
				//					}
									//Moi phut se dong bo thoi gian voi RTC mot lan
									laythoigian();
				}
			}
  }

}

//khi nao buffer full thi no se goi ham nay
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
			if(UartHandle == &huart2) 
			{
				HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_SET);
				printf("U2 full\r\n");
				HAL_GPIO_WritePin(RD485_GPIO_Port, RD485_Pin, GPIO_PIN_RESET);
			}
				        
}

//Ham chuyen doi char sang int
uint8_t convert_atoi( uint8_t c)
{
	return (uint8_t)c-48;
}
//Xu ly ban tin tu mach main
void main_message_handle(void)
{//=> Ban tin GPS: $GPS034007060819AA10	;$$GPS091259280422AA10
	if((aRxBuffer[0] =='$')&((aRxBuffer[1] =='G')|(aRxBuffer[1] =='g'))&((aRxBuffer[2] =='P')|(aRxBuffer[2] =='p'))&((aRxBuffer[3] =='S')|(aRxBuffer[3] =='s')))
	{
		
		//Ban tin dung, cap nhat du lieu
		//If there is not GPS master message, no time on webserver
		days 		= 10*convert_atoi(aRxBuffer[10])+convert_atoi(aRxBuffer[11]);
		months 	= 10*convert_atoi(aRxBuffer[12])+convert_atoi(aRxBuffer[13]);
		years 	= 10*convert_atoi(aRxBuffer[14])+convert_atoi(aRxBuffer[15]);
		hours 	= 10*convert_atoi(aRxBuffer[4])+convert_atoi(aRxBuffer[5])  ;//UTC
		minutes = 10*convert_atoi(aRxBuffer[6])+convert_atoi(aRxBuffer[7]);
		seconds = 10*convert_atoi(aRxBuffer[8])+convert_atoi(aRxBuffer[9]);
		
		//Hien thi ngay khi co dong bo sang
		console_display();
		console_blink();
		#ifdef SLAVE_WALL			
			day_display();
		#endif
		
		/*Cap nhap thoi gian NTP*/
		currtime.tm_year = 100+ years;//100+10*convert_atoi(aRxBuffer[14])+convert_atoi(aRxBuffer[15]);//In fact: 2000+xxx-1900
		currtime.tm_mon  = months-1;//10*convert_atoi(aRxBuffer[12])+convert_atoi(aRxBuffer[13])-1;
		currtime.tm_mday = days;//10*convert_atoi(aRxBuffer[10])+convert_atoi(aRxBuffer[11]);
		
		currtime.tm_sec  = seconds;//10*convert_atoi(aRxBuffer[8])+convert_atoi(aRxBuffer[9]);
		currtime.tm_min  = minutes;//10*convert_atoi(aRxBuffer[6])+convert_atoi(aRxBuffer[7]);
		currtime.tm_hour = hours;//10*convert_atoi(aRxBuffer[4])+convert_atoi(aRxBuffer[5]);
		
		timenow = mktime(&currtime);
		//timenow = timenow - 25200;//Tru di 7 tieng
		
		
		haveSignalFromRS485 = HAVE_SIGNAL;
		timeOutLostSignal = 10;
		
		
    if(count_Stable_signal < STABE_NUMBER) count_Stable_signal++; 
		
		if(count_Stable_signal >= STABE_NUMBER) 
			{
						stableSignal = SIGNAL_FROM_MASTER_OK;
						
						
			
			}

		if((aRxBuffer[16]=='A') || (aRxBuffer[17]=='A') )
						{ 
							slave_clock.sync_status = GPS;
						}
		if((aRxBuffer[16]=='V') && (aRxBuffer[17]=='V') )				
		  slave_clock.sync_status = BOTH;
						
		if(timeSaveRS485_to_RTC == 1)
		{
			timeSaveRS485_to_RTC = 60;
			//sync rs485 time to RTC 
			ghids(14,0);//1HZ out SQW
			ghids(DS_SECOND_REG,seconds);
			ghids(DS_MIN_REG,minutes);
			ghids(DS_HOUR_REG,hours);
			//ghids(DS_DAY_REG,6);
			ghids(DS_DATE_REG,days);
			ghids(DS_MONTH_REG,months);
			ghids(DS_YEAR_REG,years);
		}
		
		
		#ifdef _U1_DEBUG_ENABLE_
		printf("new timestamp:%d, %d\r\n",timenow, timeOutLostSignal);
		timeinfo = localtime( &timenow );
		printf("Current local time and date: %s\r\n", asctime(timeinfo));
		
		#endif
		//Update last sync NTP time server field!
//		unixTime_last_sync = timenow + STARTOFTIME;
//		unixTime_last_sync = htonl(unixTime_last_sync);
//		memcpy(&serverPacket[16], &unixTime_last_sync, 4);
		
		//Update SNMP data table
//		if(aRxBuffer[16]=='A') gps1_stt = 1;
//		else gps1_stt = 0;
//		if(aRxBuffer[17]=='A') gps2_stt = 1;
//		else gps2_stt = 0;
//		if(aRxBuffer[18]=='1') power1_stt = 1;
//		else power1_stt = 0;
//		if(aRxBuffer[19]=='1') power2_stt = 1;
//		else power2_stt = 0;
	}
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
	HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 100);
	return ch;
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
