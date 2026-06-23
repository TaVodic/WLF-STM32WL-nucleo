/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    subghz_phy_app.h
 * @author  MCD Application Team
 * @brief   Header of application of the SubGHz_Phy Middleware
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
#ifndef __SUBGHZ_PHY_APP_H__
#define __SUBGHZ_PHY_APP_H__

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/

/* USER CODE BEGIN EC */
#define TRANSMITTER 1
#define RECEIVER    2

#ifndef NODE
#define NODE TRANSMITTER // node is defined by CMake
#endif

#define PAYLOAD_LEN 15

#define RF_FREQUENCY              869500000 /* Hz */
#define FSK_FDEV                  25000     /* Hz */
#define FSK_DATARATE              50000     /* bps */
#define FSK_BANDWIDTH             50000     /* Hz */
#define FSK_PREAMBLE_LENGTH       5         /* Same for Tx and Rx */
#define FSK_FIX_LENGTH_PAYLOAD_ON true
#define TX_OUTPUT_POWER           10 /* dBm */
#define FSK_AFC_BANDWIDTH         83333
#define RX_CONTINUOUS_MODE        true
#define FSK_MODULATION_SHAPING                                                 \
  RADIO_FSK_MOD_SHAPING_G_BT_1; // could be experimented with
#define FSK_LENGTH_MODE RADIO_FSK_PACKET_FIXED_LENGTH

  /* USER CODE END EC */

  /* External variables
   * --------------------------------------------------------*/
  /* USER CODE BEGIN EV */

  /* USER CODE END EV */

  /* Exported macros
   * -----------------------------------------------------------*/
  /* USER CODE BEGIN EM */

  /* USER CODE END EM */

  /* Exported functions prototypes
   * ---------------------------------------------*/
  /**
   * @brief  Init Subghz Application
   */
  void SubghzApp_Init(void);

  /* USER CODE BEGIN EFP */
  void Transmitt(const char* payload);

  /* USER CODE END EFP */

#ifdef __cplusplus
}
#endif

#endif /*__SUBGHZ_PHY_APP_H__*/
