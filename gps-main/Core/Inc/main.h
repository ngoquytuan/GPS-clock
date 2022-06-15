/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
#define MAIN_PPS_Pin GPIO_PIN_2
#define MAIN_PPS_GPIO_Port GPIOC
#define LED_Pin GPIO_PIN_0
#define LED_GPIO_Port GPIOA
#define LCD_RS_Pin GPIO_PIN_0
#define LCD_RS_GPIO_Port GPIOB
#define LCD_RW_Pin GPIO_PIN_1
#define LCD_RW_GPIO_Port GPIOB
#define LCD_EN_Pin GPIO_PIN_2
#define LCD_EN_GPIO_Port GPIOB
#define LCD_D5_Pin GPIO_PIN_7
#define LCD_D5_GPIO_Port GPIOC
#define LCD_D6_Pin GPIO_PIN_8
#define LCD_D6_GPIO_Port GPIOC
#define LCD_D7_Pin GPIO_PIN_9
#define LCD_D7_GPIO_Port GPIOC
#define GPS2PPS_Pin GPIO_PIN_10
#define GPS2PPS_GPIO_Port GPIOA
#define GPS2PPS_EXTI_IRQn EXTI15_10_IRQn
#define GPS1PPS_Pin GPIO_PIN_11
#define GPS1PPS_GPIO_Port GPIOA
#define GPS1PPS_EXTI_IRQn EXTI15_10_IRQn
#define LED_GPS2_Pin GPIO_PIN_10
#define LED_GPS2_GPIO_Port GPIOC
#define LED_GPS1_Pin GPIO_PIN_11
#define LED_GPS1_GPIO_Port GPIOC
#define LED_CPU_Pin GPIO_PIN_12
#define LED_CPU_GPIO_Port GPIOC
/* USER CODE BEGIN Private defines */
#define fractionOfSecond TIM8->CNT
//define for DS3231
#define DS_SECOND_REG 0
#define DS_MIN_REG 1
#define DS_HOUR_REG 2
#define DS_DAY_REG 3
#define DS_DATE_REG 4
#define DS_MONTH_REG 5
#define DS_YEAR_REG 6
//Realtime address -----------------------
#define RTC_HOUR_ADDR 2
#define RTC_MINUTE_ADDR	1
#define RTC_SECOND_ADDR	0
#define RTC_DAY_ADDR	4
#define RTC_MONTH_ADDR	5
#define RTC_YEAR_ADDR	6


// hieu chuan gia tri dien ap
#define k12v 		0.0384
#define k5v 		0.0384
#define kDCin1 	0.0384
#define kDCin2 	0.0384

typedef struct{

  uint16_t vin1x10;//dien ap vao 1 x10, 240 = 24.0V
	uint16_t vin2x10;//dien ap vao 2 x10, 722 = 72.2V
	uint16_t v12V;//dien ap vao 12V x10,  123 = 12.3V
	uint16_t v5V;//dien ap vao 5V x10
	uint16_t v3V3;//dien ap vao 3V3 x10
	uint8_t gps1_stt;//trang thai chip GPS1 
	uint8_t gps2_stt;//trang thai chip GPS2
	uint8_t gps_pps; 
} main_sst;
typedef struct{

   
  uint8_t gps1_time[6];
  uint8_t gps2_time[6];
  uint8_t gps1_valid;
  uint8_t gps2_valid;
	uint8_t gps1_date[6];
	uint8_t gps2_date[6];
  uint8_t con_ip[16];  
	uint8_t rtc_time[6];
	uint8_t rtc_date[6];
	uint8_t gps1_lock;
	uint8_t gps2_lock;
  uint8_t update_rtc;  
} GPSClock_t;
// Thoi gian luu GPS vao RTC (minutes)
#define RTC_store_time 60
//Tre xu ly du lieu thoi gian (ns)
#define DelayOfProcess 60
/* User can use this section to tailor ADCx instance under use and associated
   resources */

/* Definitions of data related to this example */
  /* Full-scale digital value with a resolution of 12 bits (voltage range     */
  /* determined by analog voltage references Vref+ and Vref-,                 */
  /* refer to reference manual).                                              */
  #define DIGITAL_SCALE_12BITS             (0xFFFUL)

  /* Init variable out of ADC expected conversion data range */
  #define VAR_CONVERTED_DATA_INIT_VALUE    (DIGITAL_SCALE_12BITS + 1)
  
  /* Gain compensation x1 factor */
  #define GAIN_COMPENSATION_X1_FACTOR      (0x1000UL)

extern uint8_t days,months,years,hours,minutes,seconds ;
extern uint8_t TimeMessage[20];
extern uint32_t u1_halt,u2_halt,u3_halt;
#define UART_MAYBE_HALT                    60
/* Size of Reception buffer */
#define RX1BUFFERSIZE                      1000
#define RX2BUFFERSIZE                      1000
#define RX3BUFFERSIZE                      100
void check_uart(void);
void sweep_LCD(void);
void ghids(unsigned char add, unsigned char dat);
void laythoigian(void);
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
