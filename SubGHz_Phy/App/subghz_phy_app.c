/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    subghz_phy_app.c
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
#include "subghz_phy_app.h"
#include "platform.h"
#include "radio.h"
#include "radio_driver.h"
#include "radio_ex.h"
#include "stm32wlxx_hal_gpio.h"
#include "sys_app.h"

/* USER CODE BEGIN Includes */
#include <stdint.h>
#include <stdio.h>
#include <string.h>

/* USER CODE END Includes */

/* External variables
 * ---------------------------------------------------------*/
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define MAX_APP_BUFFER_SIZE 255
#define RX_TIMEOUT_VALUE    3000
#define TX_TIMEOUT_VALUE    3000

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* Radio events function pointer */
static RadioEvents_t RadioEvents;

/* USER CODE BEGIN PV */
/* App Rx Buffer*/
static uint8_t BufferRx[MAX_APP_BUFFER_SIZE];
/* App Tx Buffer*/
static uint8_t BufferTx[MAX_APP_BUFFER_SIZE];
/* Last  Received Buffer Size*/
uint16_t RxBufferSize = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/*!
 * @brief Function to be executed on Radio Tx Done event
 */
static void OnTxDone(void);

/**
 * @brief Function to be executed on Radio Rx Done event
 * @param  payload ptr of buffer received
 * @param  size buffer size
 * @param  rssi
 * @param  LoraSnr_FskCfo
 */
static void OnRxDone(uint8_t *payload, uint16_t size, int16_t rssi,
                     int8_t LoraSnr_FskCfo);

/**
 * @brief Function executed on Radio Tx Timeout event
 */
static void OnTxTimeout(void);

/**
 * @brief Function executed on Radio Rx Timeout event
 */
static void OnRxTimeout(void);

/**
 * @brief Function executed on Radio Rx Error event
 */
static void OnRxError(void);

/* USER CODE BEGIN PFP */

void Transmitt(const char* payload);

/* USER CODE END PFP */

/* Exported functions
 * ---------------------------------------------------------*/
void SubghzApp_Init(void)
{
  /* USER CODE BEGIN SubghzApp_Init_1 */

  /* USER CODE END SubghzApp_Init_1 */

  /* Radio initialization */
  RadioEvents.TxDone = OnTxDone;
  RadioEvents.RxDone = OnRxDone;
  RadioEvents.TxTimeout = OnTxTimeout;
  RadioEvents.RxTimeout = OnRxTimeout;
  RadioEvents.RxError = OnRxError;

  Radio.Init(&RadioEvents);

  /* USER CODE BEGIN SubghzApp_Init_2 */
  Radio.SetChannel(RF_FREQUENCY);

  printf("---------------\n\r");
  printf("STM32WL SUBGHZ DEVEL\n");
  printf("FSK_MODULATION\n\r");
  printf("FSK_F=%d Hz\n\r", RF_FREQUENCY);
  printf("FSK_DR=%d bits/s\n\r", FSK_DATARATE);

#if (NODE == TRANSMITTER)
  printf("DEVICE: TRANSMITTER\n\n");
  Radio.SetTxConfig(MODEM_FSK, TX_OUTPUT_POWER, FSK_FDEV, 0, FSK_DATARATE, 0,
                    FSK_PREAMBLE_LENGTH, FSK_FIX_LENGTH_PAYLOAD_ON, true, 0, 0,
                    0, TX_TIMEOUT_VALUE);
  Radio.SetMaxPayloadLength(MODEM_FSK, MAX_APP_BUFFER_SIZE);

  TxConfigGeneric_t txConf;
  txConf.fsk.BitRate = FSK_DATARATE;
  txConf.fsk.FrequencyDeviation = FSK_FDEV;
  txConf.fsk.ModulationShaping =
      FSK_MODULATION_SHAPING; // could be experimented with

  txConf.fsk.HeaderType = FSK_LENGTH_MODE;
  txConf.fsk.PreambleLen = FSK_PREAMBLE_LENGTH;
  txConf.fsk.SyncWordLength = 3;
  txConf.fsk.SyncWord = ( uint8_t[] ){ 0xC1, 0x94, 0xC1, 0x00, 0x00, 0x00, 0x00, 0x00 };

  txConf.fsk.CrcLength = RADIO_FSK_CRC_2_BYTES_CCIT;
  txConf.fsk.CrcPolynomial = CRC_POLYNOMIAL_CCITT;
  txConf.fsk.CrcSeed = CRC_CCITT_SEED;

  txConf.fsk.whiteSeed = 0x01FF;
  txConf.fsk.Whitening = RADIO_FSK_DC_FREEWHITENING; // CCITT

  Radio.RadioSetTxGenericConfig(GENERIC_FSK, &txConf, TX_OUTPUT_POWER, 0);

  memset(BufferTx, 0x0, MAX_APP_BUFFER_SIZE);

#elif (NODE == RECEIVER)
  printf("DEVICE: RECEIVER\n\n");
  /*Radio.SetRxConfig(MODEM_FSK, FSK_BANDWIDTH, FSK_DATARATE, 0,
                    FSK_AFC_BANDWIDTH, FSK_PREAMBLE_LENGTH, 0,
                    FSK_FIX_LENGTH_PAYLOAD_ON, PAYLOAD_LEN, true, 0, 0, false,
                    RX_CONTINUOUS_MODE);
  Radio.SetMaxPayloadLength(MODEM_FSK, PAYLOAD_LEN);*/

  RxConfigGeneric_t rxConfig;
  rxConfig.fsk.Bandwidth = FSK_BANDWIDTH;
  rxConfig.fsk.BitRate = FSK_DATARATE;
  rxConfig.fsk.ModulationShaping = FSK_MODULATION_SHAPING;

  rxConfig.fsk.PreambleLen = FSK_PREAMBLE_LENGTH;  
  rxConfig.fsk.PreambleMinDetect = RADIO_PREAMBLE_DETECTOR_08_BITS;
  //rxConfig.fsk.StopTimerOnPreambleDetect;

  rxConfig.fsk.SyncWordLength = 3;
  rxConfig.fsk.SyncWord = ( uint8_t[] ){ 0xC1, 0x94, 0xC1, 0x00, 0x00, 0x00, 0x00, 0x00 };

  rxConfig.fsk.AddrComp = 0;

  rxConfig.fsk.LengthMode = FSK_LENGTH_MODE;
  rxConfig.fsk.MaxPayloadLength = PAYLOAD_LEN;

  rxConfig.fsk.CrcLength = RADIO_FSK_CRC_2_BYTES_CCIT;
  rxConfig.fsk.CrcPolynomial = CRC_POLYNOMIAL_CCITT;

  rxConfig.fsk.Whitening = RADIO_FSK_DC_FREEWHITENING; // CCITT;
  rxConfig.fsk.whiteSeed = 0x01FF;;

  Radio.RadioSetRxGenericConfig(GENERIC_FSK, &rxConfig, RX_CONTINUOUS_MODE, 0);

  Radio.Rx(0);
#else
#error "Invalid NODE configuration!"
#endif

  /* USER CODE END SubghzApp_Init_2 */
}

/* USER CODE BEGIN EF */

/* USER CODE END EF */

/* Private functions ---------------------------------------------------------*/
static void OnTxDone(void)
{
  /* USER CODE BEGIN OnTxDone */
  /* USER CODE END OnTxDone */
}

static void OnRxDone(uint8_t *payload, uint16_t size, int16_t rssi,
                     int8_t LoraSnr_FskCfo)
{
  /* USER CODE BEGIN OnRxDone */
  HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
  printf("OnRxDone!\r\n");
  printf("RssiValue=%d dBm, Cfo=%dkHz,\n\r", rssi, LoraSnr_FskCfo);

  memset(BufferRx, 0, MAX_APP_BUFFER_SIZE);
  RxBufferSize = size;
  if (RxBufferSize <= MAX_APP_BUFFER_SIZE)
  {
    memcpy(BufferRx, payload, RxBufferSize);
  }

  printf("payload size=%d \n\r", size);
  for (int32_t i = 0; i < PAYLOAD_LEN; i++)
  {
    printf("%d ", BufferRx[i]);
    if (i % 16 == 15)
    {
      printf("\n\r");
    }
  }
  printf("\n\r");
  for (int32_t i = 0; i < PAYLOAD_LEN; i++)
  {
    printf("%c", BufferRx[i]);
    if (i % 16 == 15)
    {
      printf("\n\r");
    }
  }
  printf("\n\r");
  HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
  /* USER CODE END OnRxDone */
}

static void OnTxTimeout(void)
{
  /* USER CODE BEGIN OnTxTimeout */
  /* USER CODE END OnTxTimeout */
}

static void OnRxTimeout(void)
{
  /* USER CODE BEGIN OnRxTimeout */
  /* USER CODE END OnRxTimeout */
}

static void OnRxError(void)
{
  /* USER CODE BEGIN OnRxError */
  /* USER CODE END OnRxError */
}

/* USER CODE BEGIN PrFD */
void Transmitt(const char* payload)
{
  strcpy((char *)BufferTx, payload);
  Radio.Send(BufferTx, PAYLOAD_LEN);
}

/* USER CODE END PrFD */
