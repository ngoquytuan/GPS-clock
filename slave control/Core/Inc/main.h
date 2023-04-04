/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */


typedef struct {
	uint8_t ip[4];
	uint8_t sn[4];
	uint8_t gw[4];
	uint8_t dns[4];
	uint32_t keyword;
	uint32_t LED_intensity;
	uint8_t ntp_ip[4];
} dataSave;		


/* Size of Reception buffer */
#define RXBUFFERSIZE                      100



#define SIGNAL_FROM_MASTER_OK   1
#define SIGNAL_FROM_MASTER_BAD  0

//Trang thai hoat dong cua dong ho console
//lay thoi gian tu DS3231
#define RED 		1
//Lay thoi gian tu dong ho master ( co dong bo)
#define GREEN   0
// No GPS and  synced
#define BOTH    2

#define STABE_NUMBER 				30
#define HAVE_SIGNAL 				1
#define NO_SIGNAL 				  0

#define RTC_FINE 						2	
#define RTC_OUT_OF_BATTERY 	1
#define NO_RTC 							0

#define SYNC_WiTH_RS485 							    0
#define SYNC_WiTH_NTP_Server 							1

#define NO_CONNECTION 							0
#define CONNECTED 	     						1
typedef struct{

  uint8_t work_mode;
	uint8_t rs458_connection;
	uint8_t ntp_client_status;
	uint8_t ethernet_connection;
	uint8_t rtc_status; 
	uint8_t sync_status; 
} slave_status;

extern slave_status slave_clock;

extern char ds3231_reg[7],nhietdo;
extern uint8_t days;
extern uint8_t months;
extern uint8_t years;
extern uint8_t hours;
extern uint8_t minutes;
extern uint8_t seconds;
extern uint8_t server_hour, server_minute, server_second,server_day, server_month,server_year ;
extern uint8_t aRxBuffer[RXBUFFERSIZE];
extern uint8_t LEDintensity;
extern UART_HandleTypeDef huart2;
extern uint32_t ms_couters;
extern struct tm* timeinfo;

void stm32g474flashEraseThenSave(void);
void stm32g474_FactoryLoad(void);

void w5500_lib_init(void);
void checklink(void);
void control(void);
void resetUART(void);
void resetInitLED(void);



void chinhdosang(void);
void update_display(void);
	
void slaveClockFucnsInit(void);
void slaveClockRun(void);
void SNTP_init(void);		
int8_t SNTP_run(void);	
//void wzn_event_handle(void);

void RTC_Update(void);
//define for DS3231
#define DS_SECOND_REG 0
#define DS_MIN_REG 1
#define DS_HOUR_REG 2
#define DS_DAY_REG 3
#define DS_DATE_REG 4
#define DS_MONTH_REG 5
#define DS_YEAR_REG 6


void laythoigian(void);
void ghids(unsigned char add, unsigned char dat);
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define PC1_Pin GPIO_PIN_1
#define PC1_GPIO_Port GPIOC
#define PC3_Pin GPIO_PIN_3
#define PC3_GPIO_Port GPIOC
#define LOAD2_Pin GPIO_PIN_0
#define LOAD2_GPIO_Port GPIOA
#define RD485_Pin GPIO_PIN_4
#define RD485_GPIO_Port GPIOA
#define SCSn_Pin GPIO_PIN_0
#define SCSn_GPIO_Port GPIOB
#define INTn_Pin GPIO_PIN_1
#define INTn_GPIO_Port GPIOB
#define INTn_EXTI_IRQn EXTI1_IRQn
#define RSTn_Pin GPIO_PIN_2
#define RSTn_GPIO_Port GPIOB
#define FR_Pin GPIO_PIN_12
#define FR_GPIO_Port GPIOB
#define FR_EXTI_IRQn EXTI15_10_IRQn
#define SQW_Pin GPIO_PIN_8
#define SQW_GPIO_Port GPIOA
#define SQW_EXTI_IRQn EXTI9_5_IRQn
#define LED_Pin GPIO_PIN_15
#define LED_GPIO_Port GPIOA
#define CLK2_Pin GPIO_PIN_10
#define CLK2_GPIO_Port GPIOC
#define DIN2_Pin GPIO_PIN_11
#define DIN2_GPIO_Port GPIOC
#define LED_LOAD_Pin GPIO_PIN_3
#define LED_LOAD_GPIO_Port GPIOB
#define LED_CLK_Pin GPIO_PIN_4
#define LED_CLK_GPIO_Port GPIOB
#define LED_DIN_Pin GPIO_PIN_5
#define LED_DIN_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
