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
#include <stdint.h>
#include <string.h>
#include <time.h>
#include "stdio.h"

#include "wizchip_conf.h"
#include "irigb.h"

#include "httpServer.h"
#include "webpage.h"
#include "snmp.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define fractionOfSecond TIM4->CNT
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

//#define irigb_debug
//bo dem irigB 0-99
int8_t codecounter = 0;
volatile unsigned int hourcode=0, minutecode=0, secondcode=0, daycode=0, monthcode=0, yearcode=0, nodaycode=0;
uint8_t startbuildcode,stopbuildcode;
void resetirigbcode(void);
volatile char time_buff1[7], time_buff2[7];
volatile char date_buff1[7], date_buff2[7];
unsigned char daysofmonth[12]={31,28,31,30,31,30,31,31,30,31,30,31};
unsigned char daysofmonth2[12]={31,29,31,30,31,30,31,31,30,31,30,31};
//unsigned long timeofday=0;
/* end of irigb variables -------------------------------------------------------------*/

extern time_t unixTime_last_sync;
extern uint8_t serverPacket[];
extern time_t micros_recv;
//time_t micros_offset = 0;
//time_t transmitTime = 0;
//time_t micros_transmit = 0;
extern time_t recvTime;

time_t timenow = 1651133756;
struct tm* timeinfo;

uint32_t	_loop1=0;
wiz_NetInfo myipWIZNETINFO = { .mac = {0x0A, 0x09, 0xDC,0x4F, 0xEB, 0x6F},
															 .ip = {192, 168, 1, 100},
															 .sn = {255,255,255,1},
															 .gw = {192, 168, 1, 1},
															 .dns = {8,8,8,8},
															 .dhcp = NETINFO_STATIC };


uint8_t u1Timeout = 0;
uint8_t u2Timeout = 0;
uint32_t timct=0;
														
//for smnp
// lost signal : if after timeOutLostSignal seconds without GPS master message => lost
int8_t lostSignal = 0;
int8_t timeOutLostSignal = 30;
uint8_t gps1_stt = 0;
uint8_t gps2_stt = 0;
uint8_t power1_stt = 0;
uint8_t power2_stt = 0;
uint8_t SNMPmanagerIP[4] ={192, 168, 22, 164};
uint8_t SNMPagentIP[4]   ={192, 168, 22, 164};

uint8_t days = 0;
uint8_t months = 0;
uint8_t years = 0;
uint8_t hours = 0;
uint8_t minutes = 0;
uint8_t seconds = 0;
struct tm currtime;

//NTP client
extern uint16_t RetrySend ; //60 giay
extern uint16_t sycnPeriod ;// 1 gio
void SNTP_init(void);
int8_t SNTP_run(void);

//UART
/* Size of Reception buffer */
#define RXBUFFERSIZE                      100
/* Buffer used for reception */
uint8_t aRxBuffer[RXBUFFERSIZE];
volatile uint32_t snmp_tick_1ms = 0;
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
 DAC_HandleTypeDef hdac1;
DMA_HandleTypeDef hdma_dac1_ch1;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_DAC1_Init(void);
static void MX_SPI1_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM4_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void storeValue(void);
void loadValue(void);
void w5500_lib_init(void);
void ntpserverdefaultconfig(void);
void INT_NTP(void);
int32_t NTPUDP(void);

void main_message_handle(void);
void u1_message_handle(void);
void build_irigB_code(void);
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  int32_t ret = 0;	
	int8_t count_to_send_main=0;
	char msg[100];
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
  MX_DAC1_Init();
  MX_SPI1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  printf("This code gen by STMcube; STM32G474RBT6@160MHz\r\n");
	
	
	
	//storeValue();
	//loadValue();
	w5500_lib_init();
	
	HAL_GPIO_WritePin(TimeRD_GPIO_Port, TimeRD_Pin, GPIO_PIN_SET);
	HAL_UART_Transmit(&huart1, (uint8_t *)"UART1 TX ok\r\n", 13, 100);
	sprintf(msg,"IP %d.%d.%d.%d\r\n",myipWIZNETINFO.ip[0],myipWIZNETINFO.ip[1],myipWIZNETINFO.ip[2],myipWIZNETINFO.ip[3]);
	HAL_UART_Transmit(&huart1, (uint8_t *)msg, 20, 100);
	HAL_GPIO_WritePin(TimeRD_GPIO_Port, TimeRD_Pin, GPIO_PIN_RESET);
	
	HAL_DAC_Start(&hdac1, DAC_CHANNEL_1);
	HAL_DAC_Start_DMA(&hdac1, DAC_CHANNEL_1, (uint32_t*)_Bit0, 1280, DAC_ALIGN_12B_R);
	
	HAL_TIM_Base_Start(&htim2);
	HAL_TIM_Base_Start_IT(&htim3);
	HAL_TIM_Base_Start(&htim4);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	
	SNTP_init();
	
	printf("Run\r\n");
	unixTime_last_sync = timenow;
	timeinfo = localtime( &timenow );
	printf("Current local time and date: %s\r\n", asctime(timeinfo));
	
	//printf("HH-MM-SS :%d-%d-%d\r\n",timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec);
	//printf("DD-MM-YY :%d-%d-%d\r\n",timeinfo->tm_mday,1+timeinfo->tm_mon,timeinfo->tm_year-100);
	codecounter = 0;		
	days = timeinfo->tm_mday;
  months = 1+timeinfo->tm_mon;
  years = timeinfo->tm_year-100;
  hours = timeinfo->tm_hour;
  minutes = timeinfo->tm_min;
  seconds = timeinfo->tm_sec;

  build_irigB_code();			
	ntpserverdefaultconfig();
	/* SNMP(Simple Network Management Protocol) Agent Initialize */
	// NMS (SNMP manager) IP address
	//snmpd_init(SNMPmanagerIP,SNMPagentIP,SOCK_agent,SOCK_trap);	
	snmp_init();
	loadwebpages();
	fractionOfSecond = 0;
	
	HAL_UART_Abort(&huart2);
  if (HAL_UART_Receive_IT(&huart2, (uint8_t *)aRxBuffer, RXBUFFERSIZE) != HAL_OK)
  {
    /* Transfer error in reception process */
    Error_Handler();
  }
	
  while (1)
  {
		
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		if( (ret = NTPUDP()) < 0) {
				printf("SOCKET ERROR : %d\r\n", ret);
				NVIC_SystemReset();
		}
		
		{	//SNMPv1 run
			//Run SNMP Agent Fucntion
			/* SNMP Agent Handler */
			//SMI Network Management Private Enterprise Codes: : moi cong ty phai dang ky 1 so rieng, 
			//tham khao : https://www.iana.org/assignments/enterprise-numbers/enterprise-numbers
			// Vi du Arduino : 36582
    	// SNMP Agent daemon process : User can add the OID and OID mapped functions to snmpData[] array in snmprun.c/.h
			// [net-snmp version 5.7 package for windows] is used for this demo.
			// * Command example
    	// [Command] Get:			  snmpget -v 1 -c public 192.168.22.163 .1.3.6.1.2.1.1.1.0 			// (sysDescr)
    	// [Command] Get: 			snmpget -v 1 -c public 192.168.1.165 .1.3.6.1.4.1.6.1.0 			// (Custom, get LED status)
    	// [Command] Get-Next: 	snmpwalk -v 1 -c public 192.168.22.163 .1.3.6.1
			// [Command] Set: 			snmpset -v 1 -c public 192.168.1.165 .1.3.6.1.4.1.6.1.1 i 1			// (Custom, LED 'On')
    	// [Command] Set: 			snmpset -v 1 -c public 192.168.1.165 .1.3.6.1.4.1.6.1.1 i 0			// (Custom, LED 'Off')
			snmpd_run2();
			
		}
		
		// web server 	
			httpServer_run(0);
			httpServer_run(1);
			httpServer_run(2);
		
		SNTP_run();
		if(timct > 1000) {
			timct = 0;
			timenow++;
			count_to_send_main++;
			
			RetrySend++;
			sycnPeriod++;
			
			HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
		
			//printf("1s\r\n");
			HAL_Delay(50);
			HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
			
			HAL_Delay(50);
			
			
		}
		if(count_to_send_main > 59)
		{
			count_to_send_main = 0;
			//printf("IP:%02X.%02X.%02X.%02X\r\n",myipWIZNETINFO.ip[0],myipWIZNETINFO.ip[1],myipWIZNETINFO.ip[2],myipWIZNETINFO.ip[3]);
			printf("IP %d.%d.%d.%d\r\n",myipWIZNETINFO.ip[0],myipWIZNETINFO.ip[1],myipWIZNETINFO.ip[2],myipWIZNETINFO.ip[3]);
		}
		if(u2Timeout == 1) 
			{
				u2Timeout = 0;
				main_message_handle();
				//Truyen ban tin cho cac dong ho slave
				HAL_GPIO_WritePin(TimeRD_GPIO_Port, TimeRD_Pin, GPIO_PIN_SET);
				HAL_UART_Transmit(&huart1, aRxBuffer, 20, 100);
				HAL_GPIO_WritePin(TimeRD_GPIO_Port, TimeRD_Pin, GPIO_PIN_RESET);
				//printf("aRxBuffer %s; \r\n",aRxBuffer);
				huart2.pRxBuffPtr = (uint8_t *)aRxBuffer;
				huart2.RxXferCount = RXBUFFERSIZE;
				memset(aRxBuffer,0,RXBUFFERSIZE);
			}
		if(u1Timeout == 1) 
			{
				u1Timeout = 0;
				u1_message_handle();
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
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
  RCC_OscInitStruct.PLL.PLLN = 40;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief DAC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_DAC1_Init(void)
{

  /* USER CODE BEGIN DAC1_Init 0 */

  /* USER CODE END DAC1_Init 0 */

  DAC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN DAC1_Init 1 */

  /* USER CODE END DAC1_Init 1 */

  /** DAC Initialization
  */
  hdac1.Instance = DAC1;
  if (HAL_DAC_Init(&hdac1) != HAL_OK)
  {
    Error_Handler();
  }

  /** DAC channel OUT1 config
  */
  sConfig.DAC_HighFrequency = DAC_HIGH_FREQUENCY_INTERFACE_MODE_AUTOMATIC;
  sConfig.DAC_DMADoubleDataMode = DISABLE;
  sConfig.DAC_SignedFormat = DISABLE;
  sConfig.DAC_SampleAndHold = DAC_SAMPLEANDHOLD_DISABLE;
  sConfig.DAC_Trigger = DAC_TRIGGER_T2_TRGO;
  sConfig.DAC_Trigger2 = DAC_TRIGGER_NONE;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  sConfig.DAC_ConnectOnChipPeripheral = DAC_CHIPCONNECT_EXTERNAL;
  sConfig.DAC_UserTrimming = DAC_TRIMMING_FACTORY;
  if (HAL_DAC_ConfigChannel(&hdac1, &sConfig, DAC_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DAC1_Init 2 */

  /* USER CODE END DAC1_Init 2 */

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
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 1249;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

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
  //1ms counter
  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 160-1;
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
	//	16/Aug/2019, Tuan, use Timer for make a couter 0-0.9999 second for NTP fraction
	//timer uptade frequency = TIM_CLK/(TIM_PSC+1)/(TIM_ARR + 1)
	//TIM_CLK/(TIM_Period + 1) /(Prescaler+1)
  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 16000-1;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 9999;
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
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, SCSn_Pin|RSTn_Pin|TimeRD_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : LED_Pin */
  GPIO_InitStruct.Pin = LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : FR_Pin */
  GPIO_InitStruct.Pin = FR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(FR_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : SCSn_Pin RSTn_Pin TimeRD_Pin */
  GPIO_InitStruct.Pin = SCSn_Pin|RSTn_Pin|TimeRD_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : INTn_Pin */
  GPIO_InitStruct.Pin = INTn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(INTn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PPS_Pin */
  GPIO_InitStruct.Pin = PPS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(PPS_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

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
	if (GPIO_Pin == PPS_Pin)
  {
		//HAL_GPIO_WritePin(MAIN_PPS_GPIO_Port, MAIN_PPS_Pin, GPIO_PIN_SET);
		printf("PPS\r\n");
		//fractionOfSecond = 0;
  }
	if (GPIO_Pin == INTn_Pin)
  {
		//Nhan duoc ban tin NTP, xu ly thoi gian va phan hoi
		//Receied NTP message, processing and respond
		//printf("NTP\r\n");
		INT_NTP();
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

void build_irigB_code(void)
{
	uint8_t n=0;						
					
									
						hourcode   = hours;
						minutecode = minutes;
						secondcode = seconds;

						daycode   = days;
						monthcode = months;
						yearcode  = years;
	
#ifdef irigb_debug
						printf("HH-MM-SS :%d-%d-%d",hours,minutes,seconds);
						printf("DD-MM-YY :%d-%d-%d",days,months,years);
#endif
						for (n=0; n<monthcode-1;n++)
						{
							if ((yearcode%4)==0) nodaycode+=daysofmonth2[n];
							else nodaycode+=daysofmonth[n];
						}
						nodaycode+=daycode;
								

						if (secondcode>=40) {irigb_code[8]=1;secondcode-=40;}
						if (secondcode>=20) {irigb_code[7]=1;secondcode-=20;}
						if (secondcode>=10) {irigb_code[6]=1;secondcode-=10;}
						if (secondcode>=8) {irigb_code[4]=1;secondcode-=8;}
						if (secondcode>=4) {irigb_code[3]=1;secondcode-=4;}
						if (secondcode>=2) {irigb_code[2]=1;secondcode-=2;}
						if (secondcode>=1) {irigb_code[1]=1;}

						if (minutecode>=40) {irigb_code[17]=1;minutecode-=40;}
						if (minutecode>=20) {irigb_code[16]=1;minutecode-=20;}
						if (minutecode>=10) {irigb_code[15]=1;minutecode-=10;}
						if (minutecode>=8) {irigb_code[13]=1;minutecode-=8;}
						if (minutecode>=4) {irigb_code[12]=1;minutecode-=4;}
						if (minutecode>=2) {irigb_code[11]=1;minutecode-=2;}
						if (minutecode>=1) {irigb_code[10]=1;}

						if (hourcode>=20) {irigb_code[26]=1;hourcode-=20;}
						if (hourcode>=10) {irigb_code[25]=1;hourcode-=10;}
						if (hourcode>=8) {irigb_code[23]=1;hourcode-=8;}
						if (hourcode>=4) {irigb_code[22]=1;hourcode-=4;}
						if (hourcode>=2) {irigb_code[21]=1;hourcode-=2;}
						if (hourcode>=1) {irigb_code[20]=1;}

					
						if (yearcode>=80) {irigb_code[58]=1;yearcode-=80;}
						if (yearcode>=40) {irigb_code[57]=1;yearcode-=40;}
						if (yearcode>=20) {irigb_code[56]=1;yearcode-=20;}
						if (yearcode>=10) {irigb_code[55]=1;yearcode-=10;}
						if (yearcode>=8) {irigb_code[53]=1;yearcode-=8;}
						if (yearcode>=4) {irigb_code[52]=1;yearcode-=4;}
						if (yearcode>=2) {irigb_code[51]=1;yearcode-=2;}
						if (yearcode>=1) {irigb_code[50]=1;}
					

						if (nodaycode>=200) {irigb_code[41]=1;nodaycode-=200;}
						if (nodaycode>=100) {irigb_code[40]=1;nodaycode-=100;}
						if (nodaycode>=80) {irigb_code[38]=1;nodaycode-=80;}
						if (nodaycode>=40) {irigb_code[37]=1;nodaycode-=40;}
						if (nodaycode>=20) {irigb_code[36]=1;nodaycode-=20;}
						if (nodaycode>=10) {irigb_code[35]=1;nodaycode-=10;}
						if (nodaycode>=8) {irigb_code[33]=1;nodaycode-=8;}
						if (nodaycode>=4) {irigb_code[32]=1;nodaycode-=4;}
						if (nodaycode>=2) {irigb_code[31]=1;nodaycode-=2;}
						if (nodaycode>=1) {irigb_code[30]=1;}
#ifdef irigb_debug						
						printf("IRIGB-code:\r\n");
						for(n = 0; n<100; n++)
						{
							if(irigb_code[n] == 2) printf("P ");
							else printf("%d ",irigb_code[n]);
						}
						printf("\r\n end IRIGB-code:\r\n");
#endif
}
void resetirigbcode(void)
{
unsigned char i=0;
	for (i=0;i<100;i++)
	{
		irigb_code[i]=0;
	}
	irigb_code[0]=2;irigb_code[9]=2;irigb_code[19]=2;irigb_code[29]=2;
	irigb_code[39]=2;irigb_code[49]=2;irigb_code[59]=2;irigb_code[69]=2;
	irigb_code[79]=2;irigb_code[89]=2;irigb_code[99]=2;
	nodaycode=0;
	hourcode=0;
	minutecode=0; 
	secondcode=0; 
	daycode=0; 
	monthcode=0; 
	yearcode=0; 
	nodaycode=0;
}

void HAL_DAC_ConvCpltCallbackCh1(DAC_HandleTypeDef *hdac)
{
	
//	if(_loop1 == 0) 
//		{
//			_loop1 = 1; 
//			//HAL_DAC_Start_DMA(&hdac1, DAC_CHANNEL_1, (uint32_t*)_Bit0, 1280, DAC_ALIGN_12B_R);
//			hdma_dac1_ch1.Instance->CMAR = (uint32_t)_Bit0;
//		}
//	else 
//		{
//			//HAL_DAC_Start_DMA(&hdac1, DAC_CHANNEL_1, (uint32_t*)_BitREF, 1280, DAC_ALIGN_12B_R);
//			hdma_dac1_ch1.Instance->CMAR = (uint32_t)_BitREF;
//			_loop1 = 0;
//		}
		//DAC se tao SIN tung bit0 bit1 bit REF theo bang irigb_code, quet xong 99 -> xong 1s thi quay lai 0 -> bat dau giay moi
		switch(irigb_code[codecounter])
		{
			case IRIGB_bit0:
				hdma_dac1_ch1.Instance->CMAR = (uint32_t)_Bit0;
				break;
			case IRIGB_bit1:
				hdma_dac1_ch1.Instance->CMAR = (uint32_t)_Bit1;
				break;
			case IRIGB_bitP:
				hdma_dac1_ch1.Instance->CMAR = (uint32_t)_BitREF;
				break;
		}
		codecounter++;
		if(codecounter == 99) codecounter = 0;
		
};

//khi nao buffer full thi no se goi ham nay
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
  
			if(UartHandle == &huart1) 
			{
				printf("U1 full\r\n");
			}
			if(UartHandle == &huart2) 
			{
				printf("U2 full\r\n");
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
		/*Truyen gia tri gui len web server*/
		//If there is not GPS master message, no time on webserver
		days 		= 10*convert_atoi(aRxBuffer[10])+convert_atoi(aRxBuffer[11]);
		months 	= 10*convert_atoi(aRxBuffer[12])+convert_atoi(aRxBuffer[13]);
		years 	= 10*convert_atoi(aRxBuffer[14])+convert_atoi(aRxBuffer[15]);
		hours 	= 10*convert_atoi(aRxBuffer[4])+convert_atoi(aRxBuffer[5])  ;//UTC
		minutes = 10*convert_atoi(aRxBuffer[6])+convert_atoi(aRxBuffer[7]);
		seconds = 10*convert_atoi(aRxBuffer[8])+convert_atoi(aRxBuffer[9]);
		
		//Day coi nhu la diem bat dau cua second, bat dau irigb tu day
		codecounter = 0;
		build_irigB_code();	
		
		/*Cap nhap thoi gian NTP*/
		currtime.tm_year = 100+ years;//100+10*convert_atoi(aRxBuffer[14])+convert_atoi(aRxBuffer[15]);//In fact: 2000+xxx-1900
		currtime.tm_mon  = months-1;//10*convert_atoi(aRxBuffer[12])+convert_atoi(aRxBuffer[13])-1;
		currtime.tm_mday = days;//10*convert_atoi(aRxBuffer[10])+convert_atoi(aRxBuffer[11]);
		
		currtime.tm_sec  = seconds;//10*convert_atoi(aRxBuffer[8])+convert_atoi(aRxBuffer[9]);
		currtime.tm_min  = minutes;//10*convert_atoi(aRxBuffer[6])+convert_atoi(aRxBuffer[7]);
		currtime.tm_hour = hours;//10*convert_atoi(aRxBuffer[4])+convert_atoi(aRxBuffer[5]);
		
		timenow = mktime(&currtime);
		timenow = timenow - 25200;//Tru di 7 tieng
		timeOutLostSignal = 30;//seconds 
		lostSignal = GPS_MASTER_OK;
		
		
		#ifdef _U1_DEBUG_ENABLE_
		printf("new timestamp:%d\r\n",timenow);
		timeinfo = localtime( &timenow );
		printf("Current local time and date: %s\r\n", asctime(timeinfo));
		#endif
		//Update last sync NTP time server field!
		unixTime_last_sync = timenow + STARTOFTIME;
		unixTime_last_sync = htonl(unixTime_last_sync);
		memcpy(&serverPacket[16], &unixTime_last_sync, 4);
		
		//Update SNMP data table
		if(aRxBuffer[16]=='A') gps1_stt = 1;
		else gps1_stt = 0;
		if(aRxBuffer[17]=='A') gps2_stt = 1;
		else gps2_stt = 0;
		if(aRxBuffer[18]=='1') power1_stt = 1;
		else power1_stt = 0;
		if(aRxBuffer[19]=='1') power2_stt = 1;
		else power2_stt = 0;
	}
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
