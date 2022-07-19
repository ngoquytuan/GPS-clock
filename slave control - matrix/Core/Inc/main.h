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
#define DebugEnable

#define ADDR_FLASH_PAGE_47    ((uint32_t)0x08017800) /* Base @ of Page 47, 2 Kbytes */
#define ADDR_FLASH_PAGE_48    ((uint32_t)0x08018000) /* Base @ of Page 48, 2 Kbytes */
#define ADDR_FLASH_PAGE_49    ((uint32_t)0x08018800) /* Base @ of Page 49, 2 Kbytes */
#define ADDR_FLASH_PAGE_59    ((uint32_t)0x0801D800) /* Base @ of Page 59, 2 Kbytes */
#define ADDR_FLASH_PAGE_60    ((uint32_t)0x0801E000) /* Base @ of Page 60, 2 Kbytes */
#define ADDR_FLASH_PAGE_61    ((uint32_t)0x0801E800) /* Base @ of Page 61, 2 Kbytes */

#define FLASH_USER_START_ADDR   ADDR_FLASH_PAGE_59   /* Start @ of user Flash area */
#define FLASH_USER_END_ADDR     (ADDR_FLASH_PAGE_60 + FLASH_PAGE_SIZE - 1)   /* End @ of user Flash area */

typedef struct {
	uint8_t ip[4];
	uint8_t sn[4];
	uint8_t gw[4];
	uint8_t dns[4];
	uint32_t keyword;
	uint32_t LED_intensity;
	uint8_t ntp_ip[4];
} dataSave;		
#define myMem ((dataSave *) FLASH_USER_START_ADDR)

/* Size of Reception buffer */
#define RXBUFFERSIZE                      100

#define decodeMode  0x09
#define intensity   0x0A
#define scanLimit   0x0B
#define shut_down   0x0C
#define disTest     0x0F

#define SIGNAL_FROM_MASTER_OK   1
#define SIGNAL_FROM_MASTER_BAD  0

//Trang thai hoat dong cua dong ho console
//lay thoi gian tu DS3231
#define LOCAL 1
//Lay thoi gian tu dong ho master ( co dong bo)
#define GPS   0
// No GPS and  synced
#define BOTH   2

#define STABE_NUMBER 				30
#define HAVE_SIGNAL 				1
#define NO_SIGNAL 				  0

#define RTC_FINE 						2	
#define RTC_OUT_OF_BATTERY 	1
#define NO_RTC 							0
typedef struct{

  uint8_t work_mode;
	uint8_t rs458_connection;
	uint8_t ntp_client_status;
	uint8_t ethernet_connection;
	uint8_t rtc_status; 
	uint8_t sync_status; 
} slave_status;
extern char ds3231_reg[7],nhietdo;
extern slave_status slave_clock;
extern uint8_t days;
extern uint8_t months;
extern uint8_t years;
extern uint8_t hours;
extern uint8_t minutes;
extern uint8_t seconds;
extern uint8_t aRxBuffer[RXBUFFERSIZE];
extern uint8_t LEDintensity;
extern UART_HandleTypeDef huart2;
extern uint32_t timct;


void stm32g474flashEraseThenSave(void);
void stm32g474_FactoryLoad(void);

void w5500_lib_init(void);
void checklink(void);
void control(void);
void up7_matrix_init (void);
void load_line1(uint8_t dis_date,uint8_t dis_month,uint8_t dis_year);
void scan_7up(void);
void scan_5down(void);
void load_line2(uint8_t dis_hour,uint8_t dis_min,uint8_t dis_sec,uint8_t dot);
void line2_matrix_init (void);
void chinhdosang(void);
void update_display(void);
	
void led_matrix_fucs_init(void);
void led_matrix_fucs(void);
void SNTP_init(void);		
int8_t SNTP_run(void);	
int8_t SNTP_run2(void);

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
