/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "stm32g0xx_hal.h"

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
#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOC
#define LCD_CLK_Pin GPIO_PIN_1
#define LCD_CLK_GPIO_Port GPIOA
#define LCD_SDA_Pin GPIO_PIN_2
#define LCD_SDA_GPIO_Port GPIOA
#define LCD_DC_Pin GPIO_PIN_3
#define LCD_DC_GPIO_Port GPIOA
#define LCD_RES_Pin GPIO_PIN_4
#define LCD_RES_GPIO_Port GPIOA
#define LCD_CS_Pin GPIO_PIN_5
#define LCD_CS_GPIO_Port GPIOA
#define LCD_LED_Pin GPIO_PIN_6
#define LCD_LED_GPIO_Port GPIOA
#define ENC1_C_Pin GPIO_PIN_14
#define ENC1_C_GPIO_Port GPIOB
#define ENC2_C_Pin GPIO_PIN_15
#define ENC2_C_GPIO_Port GPIOB
#define ENC1_A_Pin GPIO_PIN_8
#define ENC1_A_GPIO_Port GPIOA
#define ENC1_B_Pin GPIO_PIN_9
#define ENC1_B_GPIO_Port GPIOA
#define ENC2_A_Pin GPIO_PIN_6
#define ENC2_A_GPIO_Port GPIOC
#define ENC2_B_Pin GPIO_PIN_7
#define ENC2_B_GPIO_Port GPIOC
#define FLASH_CS_Pin GPIO_PIN_3
#define FLASH_CS_GPIO_Port GPIOD
#define FLASH_CLK_Pin GPIO_PIN_3
#define FLASH_CLK_GPIO_Port GPIOB
#define FLASH_MISO_Pin GPIO_PIN_4
#define FLASH_MISO_GPIO_Port GPIOB
#define FLASH_MOSI_Pin GPIO_PIN_5
#define FLASH_MOSI_GPIO_Port GPIOB
#define SR_IE_Pin GPIO_PIN_6
#define SR_IE_GPIO_Port GPIOB
#define SR_CE_Pin GPIO_PIN_7
#define SR_CE_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
