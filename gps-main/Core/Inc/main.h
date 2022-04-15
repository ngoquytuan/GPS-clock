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
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
