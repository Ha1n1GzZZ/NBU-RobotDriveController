/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    usart.h
 * @brief   This file contains all the function prototypes for
 *          the usart.c file
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
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
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern UART_HandleTypeDef huart5;

extern UART_HandleTypeDef huart1;

extern UART_HandleTypeDef huart2;

/* USER CODE BEGIN Private defines */


#define FRAME_BYTE_LENGTH 12 // 串口通讯帧数据的字节数（含帧头和帧尾
//#define FRAME_BYTE_LENGTH 9 // for arm test
//      #define FRAME_START 0xA5     // for arm test                 
#define FRAME_START 0x42     // 帧头(Begin)
#define FRAME_END 0x5A       // 帧尾(Z)

typedef struct {
    char RxBuffer[FRAME_BYTE_LENGTH];  // 接收缓冲
    uint8_t aRxBuffer;                 // 接收中断缓冲
    uint8_t Rx_Cnt;                    // 接收缓冲计数
    uint8_t USART_FrameFlag;           // 接收完整数据帧标志，1完整0不完
} UartStruct;

 extern UartStruct uart2Data;  // usart2的数据结构体
extern UartStruct uart1Data;
extern uint8_t uart2_rx;
extern char data[FRAME_BYTE_LENGTH];
extern char last_data[FRAME_BYTE_LENGTH];
// // int fputc(int ch, FILE *f);

 void USART_GetChar(UartStruct* Uartn, char nChar);  // 串口接收到一个字


/* USER CODE END Private defines */

void MX_UART5_Init(void);
void MX_USART1_UART_Init(void);
void MX_USART2_UART_Init(void);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

