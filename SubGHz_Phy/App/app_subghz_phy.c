/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    app_subghz_phy.c
 * @author  MCD Application Team
 * @brief   Application of the SubGHz_Phy Middleware
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

/* Includes ------------------------------------------------------------------*/
#include "app_subghz_phy.h"
#include "subghz_phy_app.h"
#include "sys_app.h"

/* USER CODE BEGIN Includes */
#include "main.h"
#include "stm32wlxx_hal.h"
#include "utilities_conf.h"
#include <stdint.h>
#include <stdio.h>


/* USER CODE END Includes */

/* External variables ---------------------------------------------------------*/
/* USER CODE BEGIN EV */

/* USER CODE END EV */

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
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Exported functions --------------------------------------------------------*/

void MX_SubGHz_Phy_Init(void)
{
  /* USER CODE BEGIN MX_SubGHz_Phy_Init_1 */

  /* USER CODE END MX_SubGHz_Phy_Init_1 */
  SystemApp_Init();
  /* USER CODE BEGIN MX_SubGHz_Phy_Init_1_1 */

  /* USER CODE END MX_SubGHz_Phy_Init_1_1 */
  SubghzApp_Init();
  /* USER CODE BEGIN MX_SubGHz_Phy_Init_2 */

  /* USER CODE END MX_SubGHz_Phy_Init_2 */
}

void MX_SubGHz_Phy_Process(void)
{
  /* USER CODE BEGIN MX_SubGHz_Phy_Process_1 */

#if (NODE == TRANSMITTER)
#define FIXTURE_N PAYLOAD_LEN
  static uint8_t fixtures_val[FIXTURE_N];

  for (uint8_t i = 0; i < sizeof(fixtures_val); i++)
  {
    fixtures_val[i] = i;
  }

  printf("Sending: ");
  for (uint8_t i = 0; i < sizeof(fixtures_val); i++)
  {
    printf("%d", fixtures_val[i]);
  }
  printf("\r\n");

  HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);  
  Transmitt(fixtures_val);
  HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
  HAL_Delay(50);
#elif (NODE == RECEIVER)
#else
#error "Invalid NODE configuration!"
#endif

  /* USER CODE END MX_SubGHz_Phy_Process_1 */
  /* USER CODE BEGIN MX_SubGHz_Phy_Process_OS */

  /* USER CODE END MX_SubGHz_Phy_Process_OS */
}

/* USER CODE BEGIN EF */

/* USER CODE END EF */

/* Private Functions Definition -----------------------------------------------*/
/* USER CODE BEGIN PrFD */

/* USER CODE END PrFD */
