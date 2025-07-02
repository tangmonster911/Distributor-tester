/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#include "stm32f4xx_hal.h"

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
#define D5_Pin GPIO_PIN_13
#define D5_GPIO_Port GPIOC
#define Ignition_Pin GPIO_PIN_3
#define Ignition_GPIO_Port GPIOC
#define Ignition_EXTI_IRQn EXTI3_IRQn
#define Menu_Pin GPIO_PIN_5
#define Menu_GPIO_Port GPIOA
#define Menu_EXTI_IRQn EXTI9_5_IRQn
#define RS_Pin GPIO_PIN_2
#define RS_GPIO_Port GPIOB
#define D6_Pin GPIO_PIN_12
#define D6_GPIO_Port GPIOB
#define D7_Pin GPIO_PIN_13
#define D7_GPIO_Port GPIOB
#define TACHO_Pin GPIO_PIN_10
#define TACHO_GPIO_Port GPIOC
#define TACHO_EXTI_IRQn EXTI15_10_IRQn
#define D4_Pin GPIO_PIN_12
#define D4_GPIO_Port GPIOC
#define E_Pin GPIO_PIN_7
#define E_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

#define MinTimerValue     10000           // 6000rpm
#define MaxTimerValue     1000000         // 60 rpm

#define MathUpdate        1
#define MathUpdateDone        0


#define ScreenUpdateDone  3
#define LastRound         2
#define UpdateScreen      1
#define ClearScreen       0

#define AllGood              0
#define advERROR             1
#define bounceERROR          2
#define RpmERROR             3



/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
