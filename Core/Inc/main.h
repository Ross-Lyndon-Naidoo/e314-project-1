/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "stm32h5xx_hal.h"

#include "stm32h5xx_nucleo.h"

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
#define BTN_DEBUG_Pin GPIO_PIN_13
#define BTN_DEBUG_GPIO_Port GPIOC
#define BTN_DEBUG_EXTI_IRQn EXTI13_IRQn
#define BTN_S5_Pin GPIO_PIN_0
#define BTN_S5_GPIO_Port GPIOC
#define BTN_S5_EXTI_IRQn EXTI0_IRQn
#define BTN_S2_Pin GPIO_PIN_1
#define BTN_S2_GPIO_Port GPIOC
#define BTN_S2_EXTI_IRQn EXTI1_IRQn
#define BTN_S3_Pin GPIO_PIN_2
#define BTN_S3_GPIO_Port GPIOC
#define BTN_S3_EXTI_IRQn EXTI2_IRQn
#define BTN_S1_Pin GPIO_PIN_3
#define BTN_S1_GPIO_Port GPIOC
#define BTN_S1_EXTI_IRQn EXTI3_IRQn
#define DIAG_TX_Pin GPIO_PIN_2
#define DIAG_TX_GPIO_Port GPIOA
#define DIAG_RX_Pin GPIO_PIN_3
#define DIAG_RX_GPIO_Port GPIOA
#define LED_DEBUG_Pin GPIO_PIN_5
#define LED_DEBUG_GPIO_Port GPIOA
#define OLED_SCK_Pin GPIO_PIN_10
#define OLED_SCK_GPIO_Port GPIOB
#define KP_C1_Pin GPIO_PIN_14
#define KP_C1_GPIO_Port GPIOB
#define KP_C1_EXTI_IRQn EXTI14_IRQn
#define KP_C2_Pin GPIO_PIN_15
#define KP_C2_GPIO_Port GPIOB
#define KP_C2_EXTI_IRQn EXTI15_IRQn
#define LED_D3_Pin GPIO_PIN_6
#define LED_D3_GPIO_Port GPIOC
#define LED_D5_Pin GPIO_PIN_8
#define LED_D5_GPIO_Port GPIOC
#define LED_D2_Pin GPIO_PIN_9
#define LED_D2_GPIO_Port GPIOC
#define BTN_S4_Pin GPIO_PIN_8
#define BTN_S4_GPIO_Port GPIOA
#define BTN_S4_EXTI_IRQn EXTI8_IRQn
#define LED_D4_Pin GPIO_PIN_9
#define LED_D4_GPIO_Port GPIOA
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define OLED_SDA_Pin GPIO_PIN_3
#define OLED_SDA_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
