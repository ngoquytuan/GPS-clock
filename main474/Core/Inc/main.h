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
extern uint32_t tim20ct;
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

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define MAIN_PPS_Pin GPIO_PIN_2
#define MAIN_PPS_GPIO_Port GPIOC
#define LED_Pin GPIO_PIN_0
#define LED_GPIO_Port GPIOA
#define RTC_Pin GPIO_PIN_1
#define RTC_GPIO_Port GPIOA
#define RTC_EXTI_IRQn EXTI1_IRQn
#define LCD_RS_Pin GPIO_PIN_0
#define LCD_RS_GPIO_Port GPIOB
#define LCD_RW_Pin GPIO_PIN_1
#define LCD_RW_GPIO_Port GPIOB
#define LCD_EN_Pin GPIO_PIN_2
#define LCD_EN_GPIO_Port GPIOB
#define LCD_D4_Pin GPIO_PIN_6
#define LCD_D4_GPIO_Port GPIOC
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
//#define LED_GPS2_Pin GPIO_PIN_10
#define LED_GPS2_GPIO_Port GPIOC
#define LED_GPS1_Pin GPIO_PIN_11
#define LED_GPS1_GPIO_Port GPIOC
//#define LED_CPU_Pin GPIO_PIN_12
#define LED_CPU_GPIO_Port GPIOC
/* USER CODE BEGIN Private defines */
#define LED_GPS2_Pin GPIO_PIN_12
#define LED_CPU_Pin GPIO_PIN_10

extern uint8_t days,months,years,hours,minutes,seconds ;
void MX_ADC2_Init_MOD(void);
void scan_ADC(void);


void Display_Time(void);
void checkUART(void);
void main_init(void);
void oneSecondCount(void);
void tasks(void);
void main_apps(void);
extern ADC_HandleTypeDef hadc2;
extern UART_HandleTypeDef huart3;
extern TIM_HandleTypeDef htim20;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;

void ADC_Select_CH17(void);
void ADC_Select_CH3(void);
void ADC_Select_CH4(void);
void ADC_Select_CH13(void);
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
