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
extern char time[7];
void loadValue(void);
void storeValue(void);
void w5500_lib_init(void);
void checklink(void);
void MAX7219_Init (void);
void MAX7219_SendAddrDat (unsigned char addr,unsigned char dat);


#define DS_SECOND_REG 0
#define DS_MIN_REG 1
#define DS_HOUR_REG 2
#define DS_DAY_REG 3
#define DS_DATE_REG 4
#define DS_MONTH_REG 5
#define DS_YEAR_REG 6


void laythoigian(void);
void ghids(unsigned char add, unsigned char dat);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define PC1_Pin GPIO_PIN_1
#define PC1_GPIO_Port GPIOC
#define PC3_Pin GPIO_PIN_3
#define PC3_GPIO_Port GPIOC
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
