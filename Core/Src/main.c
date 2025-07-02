/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "tm_stm32_hd44780.h"
#include <stdlib.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim11;

/* USER CODE BEGIN PV */

unsigned int ScreenUpdate;
unsigned char MathDo;
unsigned long TachoTimerValue;
unsigned int ScreenNumber;
unsigned int TachoIntNumber;

unsigned int DwellStartNumber;
unsigned int DwellEndNumber;
unsigned long CylinderDwellStartCounter[13];
unsigned long CylinderDwellEndCounter[13];

unsigned long TimerValue;
unsigned int  ScreenStatus;


float RPM;
float CylinderDwellMs[13];
float CylinderAdvanceDegree[13];
float CylinderDwellDegree[13];

unsigned long timedifference;

unsigned int CylinderCount;

char PrintString[7];
unsigned int i;



/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM11_Init(void);
/* USER CODE BEGIN PFP */
void SystemClock_ConfigDistributor(void);
void ClearScreenData(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */



/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
	//unsigned int ScreenStatus = ClearScreen;


  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
	SystemClock_ConfigDistributor();
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM11_Init();
  /* USER CODE BEGIN 2 */
	HAL_NVIC_DisableIRQ(EXTI3_IRQn);
	HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);

	TM_HD44780_Init(20, 4);
	ScreenNumber = 0;
	ScreenUpdate = ClearScreen;
	MathDo = MathUpdateDone;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	HAL_TIM_Base_Start(&htim2);    //Start TIM2 without interrupt
	HAL_TIM_Base_Start_IT(&htim3);    //Start TIM3 with interrupt
	while (1)
	{
		if (MathDo == MathUpdate)
		{

			if (TachoTimerValue > 0)
			{
				RPM = 1.0f / ((TachoTimerValue) / 60000000.0f);   //calculate Rpm
			} else {
				RPM = 0;
			}

			for (i = 0; i < 12; i++)
			{
				if ((CylinderDwellEndCounter[i] < CylinderDwellStartCounter[i]) || CylinderDwellEndCounter[i] == 0 || CylinderDwellStartCounter[i] == 0)  //check for any errors in data

				{
					CylinderAdvanceDegree[i] = 0;
					CylinderDwellDegree[i] = 0;
					CylinderDwellMs[i] = 0;
				} else {
					CylinderAdvanceDegree[i] =  (((float)TachoTimerValue - (float)CylinderDwellEndCounter[i])/(float)TachoTimerValue) * 360.0f;             //convert end count to degrees
					CylinderDwellDegree[i] = (((float)CylinderDwellEndCounter[i] - (float)CylinderDwellStartCounter[i]) / (float)TachoTimerValue) * 360.0f; //convert dwell count start and end difference to degrees
					CylinderDwellMs[i] = ((float)CylinderDwellEndCounter[i] - (float)CylinderDwellStartCounter[i]) / 1000.0f;                     //convert 1us count difference to ms
					if (CylinderDwellEndCounter[i] > 0) CylinderCount = i;                                                          //check amount of sparks per rotation.
					timedifference = TachoTimerValue- CylinderDwellEndCounter[CylinderCount];
				}
				CylinderDwellEndCounter[i] = 0;                                                                                 //clear for next round
				CylinderDwellStartCounter[i] = 0;                                                                               //clear for next round
			}
			ScreenUpdate = UpdateScreen;
			MathDo = MathUpdateDone;
			HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
		}

		if ((ScreenUpdate == UpdateScreen ) && (HAL_GPIO_ReadPin(Menu_GPIO_Port,Menu_Pin) == 1))
		{

			switch (ScreenNumber)
			{
			case 0:
				//TM_HD44780_Clear();
				ClearScreenData();
				//Do RPM print
				gcvt(RPM,5,PrintString);
				TM_HD44780_Puts(6, 1, PrintString);

				//Do Advance degree Print
				if (CylinderAdvanceDegree[CylinderCount] > 0)
				{
					gcvt(CylinderAdvanceDegree[CylinderCount], 4, PrintString);
					TM_HD44780_Puts(13, 2, PrintString);
				} else {
					TM_HD44780_Puts(13, 2, "err");

				}
				if (CylinderDwellDegree[CylinderCount] > 0)
				{
					gcvt(CylinderDwellDegree[CylinderCount], 4, PrintString);
					TM_HD44780_Puts(11, 3, PrintString);
				} else {
					TM_HD44780_Puts(11, 3, "err");
				}
				ScreenUpdate = ScreenUpdateDone;
				//HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
				break;


			case 1:
				//TM_HD44780_Clear();
				ClearScreenData();
				gcvt(RPM, 5, PrintString);
				TM_HD44780_Puts(14, 0, PrintString);


					gcvt(CylinderDwellDegree[0], 3,PrintString);
					TM_HD44780_Puts(0, 1, PrintString);

					gcvt(CylinderDwellDegree[1], 3,PrintString);
					TM_HD44780_Puts(5, 1, PrintString);

					gcvt(CylinderDwellDegree[2], 3,PrintString);
					TM_HD44780_Puts(10, 1, PrintString);

					gcvt(CylinderDwellDegree[3], 3,PrintString);
					TM_HD44780_Puts(15, 1, PrintString);

					gcvt(CylinderDwellDegree[4], 3,PrintString);
					TM_HD44780_Puts(0, 2, PrintString);

					gcvt(CylinderDwellDegree[5], 3,PrintString);
					TM_HD44780_Puts(5, 2, PrintString);

					gcvt(CylinderDwellDegree[6], 3,PrintString);
					TM_HD44780_Puts(10, 2, PrintString);

					gcvt(CylinderDwellDegree[7], 3,PrintString);
					TM_HD44780_Puts(15, 2, PrintString);

					gcvt(CylinderDwellDegree[8], 3,PrintString);
					TM_HD44780_Puts(0, 3, PrintString);

					gcvt(CylinderDwellDegree[9], 3,PrintString);
					TM_HD44780_Puts(5, 3, PrintString);

					gcvt(CylinderDwellDegree[10], 3,PrintString);
					TM_HD44780_Puts(10, 3, PrintString);

					gcvt(CylinderDwellDegree[11], 3,PrintString);
					TM_HD44780_Puts(15, 3, PrintString);
				ScreenUpdate = ScreenUpdateDone;
				//HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
				break;

			case 2:

				//TM_HD44780_Clear();
				ClearScreenData();
				gcvt(RPM, 5, PrintString);
				TM_HD44780_Puts(14, 0, PrintString);


					gcvt(CylinderDwellMs[0], 3,PrintString);
					TM_HD44780_Puts(0, 1, PrintString);

					gcvt(CylinderDwellMs[1], 3,PrintString);
					TM_HD44780_Puts(5, 1, PrintString);

					gcvt(CylinderDwellMs[2], 3,PrintString);
					TM_HD44780_Puts(10, 1, PrintString);

					gcvt(CylinderDwellMs[3], 3,PrintString);
					TM_HD44780_Puts(15, 1, PrintString);

					gcvt(CylinderDwellMs[4], 3,PrintString);
					TM_HD44780_Puts(0, 2, PrintString);

					gcvt(CylinderDwellMs[5], 3,PrintString);
					TM_HD44780_Puts(5, 2, PrintString);

					gcvt(CylinderDwellMs[6], 3,PrintString);
					TM_HD44780_Puts(10, 2, PrintString);

					gcvt(CylinderDwellMs[7], 3,PrintString);
					TM_HD44780_Puts(15, 2, PrintString);

					gcvt(CylinderDwellMs[8], 3,PrintString);
					TM_HD44780_Puts(0, 3, PrintString);

					gcvt(CylinderDwellMs[9], 3,PrintString);
					TM_HD44780_Puts(5, 3, PrintString);

					gcvt(CylinderDwellMs[10], 3,PrintString);
					TM_HD44780_Puts(10, 3, PrintString);

					gcvt(CylinderDwellMs[11], 3,PrintString);
					TM_HD44780_Puts(15, 3, PrintString);
				ScreenUpdate = ScreenUpdateDone;
				//HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
				break;

			case 3:
				//TM_HD44780_Clear();
				ClearScreenData();
				gcvt(RPM, 5, PrintString);
				TM_HD44780_Puts(14, 0, PrintString);

					gcvt(CylinderAdvanceDegree[0], 3,PrintString);
					TM_HD44780_Puts(0, 1, PrintString);

					gcvt(CylinderAdvanceDegree[1], 3,PrintString);
					TM_HD44780_Puts(5, 1, PrintString);

					gcvt(CylinderAdvanceDegree[2], 3,PrintString);
					TM_HD44780_Puts(10, 1, PrintString);

					gcvt(CylinderAdvanceDegree[3], 3,PrintString);
					TM_HD44780_Puts(15, 1, PrintString);

					gcvt(CylinderAdvanceDegree[4], 3,PrintString);
					TM_HD44780_Puts(0, 2, PrintString);

					gcvt(CylinderAdvanceDegree[5], 3,PrintString);
					TM_HD44780_Puts(5, 2, PrintString);

					gcvt(CylinderAdvanceDegree[6], 3,PrintString);
					TM_HD44780_Puts(10, 2, PrintString);

					gcvt(CylinderAdvanceDegree[7], 3,PrintString);
					TM_HD44780_Puts(15, 2, PrintString);

					gcvt(CylinderAdvanceDegree[8], 3,PrintString);
					TM_HD44780_Puts(0, 3, PrintString);

					gcvt(CylinderAdvanceDegree[9], 3,PrintString);
					TM_HD44780_Puts(5, 3, PrintString);

					gcvt(CylinderAdvanceDegree[10], 3,PrintString);
					TM_HD44780_Puts(10, 3, PrintString);

					gcvt(CylinderAdvanceDegree[11], 3,PrintString);
					TM_HD44780_Puts(15, 3, PrintString);
				ScreenUpdate = ScreenUpdateDone;
				//HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
				break;



			}
		}

		if (ScreenUpdate == ClearScreen ){
			ClearScreenData();
			ScreenUpdate = ScreenUpdateDone;

		}

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 200;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV8;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV8;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 25;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 1000000-1;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 25000;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 2000-1;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief TIM11 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM11_Init(void)
{

  /* USER CODE BEGIN TIM11_Init 0 */

  /* USER CODE END TIM11_Init 0 */

  /* USER CODE BEGIN TIM11_Init 1 */

  /* USER CODE END TIM11_Init 1 */
  htim11.Instance = TIM11;
  htim11.Init.Prescaler = 25000;
  htim11.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim11.Init.Period = 65535;
  htim11.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim11.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim11) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM11_Init 2 */

  /* USER CODE END TIM11_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */
  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, D5_Pin|D4_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, RS_Pin|D6_Pin|D7_Pin|E_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : D5_Pin D4_Pin */
  GPIO_InitStruct.Pin = D5_Pin|D4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : Ignition_Pin */
  GPIO_InitStruct.Pin = Ignition_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(Ignition_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : Menu_Pin */
  GPIO_InitStruct.Pin = Menu_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(Menu_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : RS_Pin D6_Pin D7_Pin E_Pin */
  GPIO_InitStruct.Pin = RS_Pin|D6_Pin|D7_Pin|E_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : TACHO_Pin */
  GPIO_InitStruct.Pin = TACHO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(TACHO_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI3_IRQn, 3, 0);
  HAL_NVIC_EnableIRQ(EXTI3_IRQn);

  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

  /* USER CODE BEGIN MX_GPIO_Init_2 */
  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void ClearScreenData(void)
{
	  //TM_HD44780_Init(20, 4);
	  switch (ScreenNumber)
	  {
	  case 0:
		  TM_HD44780_Puts(0, 0, "  Distributor Data  ");
		  TM_HD44780_Puts(0, 1, "RPM                 ");
		  TM_HD44780_Puts(0, 2, "Advance Deg         ");
		  TM_HD44780_Puts(0, 3, "Dwell Deg           ");
		  break;
	  case 1:
		  TM_HD44780_Puts(0, 0, "DwellDeg RPM:       ");
		  TM_HD44780_Puts(0, 1, "                    ");
		  TM_HD44780_Puts(0, 2, "                    ");
		  TM_HD44780_Puts(0, 3, "                    ");
	  case 2:
		  TM_HD44780_Puts(0, 0, "Dwell ms RPM:       ");
		  TM_HD44780_Puts(0, 1, "                    ");
		  TM_HD44780_Puts(0, 2, "                    ");
		  TM_HD44780_Puts(0, 3, "                    ");
		  break;
	  case 3:
		  TM_HD44780_Puts(0, 0, "Adv  Deg RPM:       ");
		  TM_HD44780_Puts(0, 1, "                    ");
		  TM_HD44780_Puts(0, 2, "                    ");
		  TM_HD44780_Puts(0, 3, "                    ");
		  break;

		  break;

	  }
}




void SystemClock_ConfigDistributor(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 20;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 200;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV8;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV8;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
