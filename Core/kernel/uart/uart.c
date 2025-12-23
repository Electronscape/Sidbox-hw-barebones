#include "main.h"

#include <stdarg.h>
#include <string.h>

#include <stdio.h>
#include <ctype.h>

#include <uart/uart.h>
#include "cmd.h"

UART_HandleTypeDef huart1;

void MX_USART1_UART_Init(void) {
	huart1.Instance = USART1;
	//huart1.Init.BaudRate = 115200;
	huart1.Init.BaudRate = 256000;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
	huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	if (HAL_UART_Init(&huart1) != HAL_OK) {
		Error_Handler();
	}
	if (HAL_UARTEx_SetTxFifoThreshold(&huart1, UART_TXFIFO_THRESHOLD_1_8)
			!= HAL_OK) {
		Error_Handler();
	}
	if (HAL_UARTEx_SetRxFifoThreshold(&huart1, UART_RXFIFO_THRESHOLD_1_8)
			!= HAL_OK) {
		Error_Handler();
	}
	if (HAL_UARTEx_DisableFifoMode(&huart1) != HAL_OK) {
		Error_Handler();
	}
}

void HAL_UART_MspInit(UART_HandleTypeDef *uartHandle) {

	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = { 0 };
	if (uartHandle->Instance == USART1) {
		PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART1;
		PeriphClkInitStruct.Usart16ClockSelection =
				RCC_USART16CLKSOURCE_D2PCLK2;
		if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) {
			Error_Handler();
		}

		__HAL_RCC_USART1_CLK_ENABLE();

		__HAL_RCC_GPIOA_CLK_ENABLE();
		/**USART1 GPIO Configuration
		 PA9     ------> USART1_TX
		 PA10     ------> USART1_RX
		 */
		GPIO_InitStruct.Pin = TX1_OUT_Pin | R1X_IN_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		HAL_NVIC_SetPriority(USART1_IRQn, IRQ_UART);
		HAL_NVIC_EnableIRQ(USART1_IRQn);
	}
}

void HAL_UART_MspDeInit(UART_HandleTypeDef *uartHandle) {
	if (uartHandle->Instance == USART1) {
		__HAL_RCC_USART1_CLK_DISABLE();

		/**USART1 GPIO Configuration
		 PA9     ------> USART1_TX
		 PA10     ------> USART1_RX
		 */
		HAL_GPIO_DeInit(GPIOA, TX1_OUT_Pin | R1X_IN_Pin);

		/* USART1 interrupt Deinit */
		HAL_NVIC_DisableIRQ(USART1_IRQn);
	}
}

// commander over the serial port data going in
int dbug(const char *fmt, ...) {
    static char buffer[BUFFER_SIZE];

    va_list args;
    va_start(args, fmt);

    // Format the string into the static buffer
    int len = vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    // If there was a formatting error, return -1
    if (len < 0) return -1;
    if (len >= (int)sizeof(buffer)) len = (int)sizeof(buffer) - 1;


    // If the formatted string was larger than our buffer, we just send what fits
    // Since we used sizeof(buffer), vsnprintf has already truncated if needed.

    // Send the entire formatted string in one go
    if (HAL_UART_Transmit(&huart1, (uint8_t *)buffer, (uint16_t)len, 0xFFFF) != HAL_OK) {
        return -1; // Transmission error
    }

    // Return the number of bytes sent
    return len;
}

static uint8_t rx_buffer[BUFFER_SIZE];
static volatile uint16_t rx_index = 0;
static volatile int rx_timeout = 0;
static uint8_t uart_ch;

void resetUART(){
	//HAL_UART_AbortReceive_IT(&huart1);
	//HAL_UART_Receive_IT(&huart1, KBMATRIX, 8);	// 8 bytes of column data
}

void uart_buffercheck_timer(void) {
    if (rx_timeout > 0) {
        rx_timeout--;
        if (rx_timeout == 0) {
            uint16_t n = rx_index;
            rx_index = 0;
            rx_buffer[n < (BUFFER_SIZE-1) ? n : (BUFFER_SIZE-1)] = 0;
            //dbug("out: %s\n", rx_buffer);
            cmd_entry(rx_buffer);
            memset(rx_buffer, 0, sizeof(rx_buffer));
        }
    }
}

void flushCmd(void) {
    rx_index = 0;
    rx_buffer[0] = 0;
    HAL_UART_Receive_IT(&huart1, &uart_ch, 1);
}

void uart_isr(void) {
	uint16_t i = rx_index;

	if (i < (BUFFER_SIZE - 1)) {
		rx_buffer[i] = uart_ch;
		rx_index = i + 1;
		rx_buffer[rx_index] = 0; // keep it a valid C-string
	} else {
		// overflow policy: reset
		rx_index = 0;
		rx_buffer[0] = 0;
	}

	rx_timeout = 5;

	if (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_ORE)) {
		// handle/clear if needed
	}

	HAL_UART_Receive_IT(&huart1, &uart_ch, 1);
}
