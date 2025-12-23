#ifndef __USART_H__
#define __USART_H__

#include "main.h"
#include <stdarg.h>
#include <string.h>

#include <stdio.h>
#include <ctype.h>


#define BUFFER_SIZE 4096  // 4KB buffer

extern UART_HandleTypeDef huart1;
void MX_USART1_UART_Init(void);

void HAL_UART_MspInit(UART_HandleTypeDef *uartHandle);
void HAL_UART_MspDeInit(UART_HandleTypeDef *uartHandle);
int dbug(const char *fmt, ...);
void flushCmd(void);
void uart_buffercheck_timer(void);


#endif
