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
void maincpp();
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define BEMF_1_Pin GPIO_PIN_13
#define BEMF_1_GPIO_Port GPIOC
#define BEMF_2_Pin GPIO_PIN_14
#define BEMF_2_GPIO_Port GPIOC
#define BEMF_3_Pin GPIO_PIN_15
#define BEMF_3_GPIO_Port GPIOC
#define PHASE_V1_Pin GPIO_PIN_0
#define PHASE_V1_GPIO_Port GPIOA
#define PHASE_I1_Pin GPIO_PIN_1
#define PHASE_I1_GPIO_Port GPIOA
#define PHASE_V2_Pin GPIO_PIN_2
#define PHASE_V2_GPIO_Port GPIOA
#define PHASE_I2_Pin GPIO_PIN_3
#define PHASE_I2_GPIO_Port GPIOA
#define PHASE_V3_Pin GPIO_PIN_4
#define PHASE_V3_GPIO_Port GPIOA
#define PHASE_I3_Pin GPIO_PIN_5
#define PHASE_I3_GPIO_Port GPIOA
#define VIN_Pin GPIO_PIN_6
#define VIN_GPIO_Port GPIOA
#define TEMP_Pin GPIO_PIN_7
#define TEMP_GPIO_Port GPIOA
#define POTEMTIOMETER_Pin GPIO_PIN_0
#define POTEMTIOMETER_GPIO_Port GPIOB
#define SPARE1_Pin GPIO_PIN_1
#define SPARE1_GPIO_Port GPIOB
#define SPARE2_Pin GPIO_PIN_2
#define SPARE2_GPIO_Port GPIOB
#define PHASE1_LOW_Pin GPIO_PIN_13
#define PHASE1_LOW_GPIO_Port GPIOB
#define PHASE2_LOW_Pin GPIO_PIN_14
#define PHASE2_LOW_GPIO_Port GPIOB
#define PHASE3_LOW_Pin GPIO_PIN_15
#define PHASE3_LOW_GPIO_Port GPIOB
#define PHASE1_HIGH_Pin GPIO_PIN_8
#define PHASE1_HIGH_GPIO_Port GPIOA
#define PHASE2_HIGH_Pin GPIO_PIN_9
#define PHASE2_HIGH_GPIO_Port GPIOA
#define SPARE3_Pin GPIO_PIN_6
#define SPARE3_GPIO_Port GPIOC
#define SPARE4_Pin GPIO_PIN_7
#define SPARE4_GPIO_Port GPIOC
#define PHASE3_HIGH_Pin GPIO_PIN_10
#define PHASE3_HIGH_GPIO_Port GPIOA
#define WS2812_LED_Pin GPIO_PIN_0
#define WS2812_LED_GPIO_Port GPIOD
#define DSHOT_Pin GPIO_PIN_1
#define DSHOT_GPIO_Port GPIOD
#define HALL2_Pin GPIO_PIN_2
#define HALL2_GPIO_Port GPIOD
#define HALL1_Pin GPIO_PIN_3
#define HALL1_GPIO_Port GPIOD

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
