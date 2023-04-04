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
extern uint8_t days;
extern uint8_t months ;
extern uint8_t years ;
extern uint8_t hours ;
extern uint8_t minutes ;
extern uint8_t seconds ;

/* Size of Reception buffer */
#define RXBUFFERSIZE                      99
#define RX1BUFFERSIZE                     99
extern uint8_t Rx1Buffer[];
extern uint8_t aRxBuffer[];
//#define DebugEnable

#define ADDR_FLASH_PAGE_47    ((uint32_t)0x08017800) /* Base @ of Page 47, 2 Kbytes */
#define ADDR_FLASH_PAGE_48    ((uint32_t)0x08018000) /* Base @ of Page 48, 2 Kbytes */
#define ADDR_FLASH_PAGE_49    ((uint32_t)0x08018800) /* Base @ of Page 49, 2 Kbytes */

#define FLASH_USER_START_ADDR   ADDR_FLASH_PAGE_47   /* Start @ of user Flash area */
#define FLASH_USER_END_ADDR     (ADDR_FLASH_PAGE_48 + FLASH_PAGE_SIZE - 1)   /* End @ of user Flash area */
//Cau truc nay phai dung thu tu
typedef struct {
	uint8_t ip[4];
	uint8_t sn[4];
	uint8_t gw[4];
	uint8_t dns[4];
	uint32_t keyword;
	uint32_t irigGain;
} dataSave;		
#define myMem ((dataSave *) FLASH_USER_START_ADDR)
void stm32g474flashEraseThenSave(void);
void stm32g474_FactoryLoad(void);

int32_t NTPUDP(uint8_t socket_number);
void reInitNTP(uint8_t socket_number);

#define IRIGB_bit0 0
#define IRIGB_bit1 1
#define IRIGB_bitP 2

void resetirigbcode(void);
void build_irigB_code(void);
void w5500_lib_init(void);
void ntpserverdefaultconfig(void);
void INT_NTP(void);

void machGiaoTiep(void);
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_Pin GPIO_PIN_0
#define LED_GPIO_Port GPIOA
#define FR_Pin GPIO_PIN_1
#define FR_GPIO_Port GPIOA
#define SCSn_Pin GPIO_PIN_0
#define SCSn_GPIO_Port GPIOB
#define INTn_Pin GPIO_PIN_1
#define INTn_GPIO_Port GPIOB
#define INTn_EXTI_IRQn EXTI1_IRQn
#define RSTn_Pin GPIO_PIN_2
#define RSTn_GPIO_Port GPIOB
#define TimeRD_Pin GPIO_PIN_12
#define TimeRD_GPIO_Port GPIOB
#define PPS_Pin GPIO_PIN_12
#define PPS_GPIO_Port GPIOA
#define PPS_EXTI_IRQn EXTI15_10_IRQn
/* USER CODE BEGIN Private defines */

//Number of seconds from 1st January 1900 to start of Unix epoch
//According to the Time protocol in RFC 868 it is 2208988800L.
#define STARTOFTIME 2208988800UL

#ifndef UTIL_H
#define UTIL_H
#define htons(x) ( ((x)<< 8 & 0xFF00) | \
                   ((x)>> 8 & 0x00FF) )
#define ntohs(x) htons(x)

#define htonl(x) ( ((x)<<24 & 0xFF000000UL) | \
                   ((x)<< 8 & 0x00FF0000UL) | \
                   ((x)>> 8 & 0x0000FF00UL) | \
                   ((x)>>24 & 0x000000FFUL) )
#define ntohl(x) htonl(x)
#endif
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
