#ifndef INTERRUPTS_H
#define INTERRUPTS_H


#include "main.h"

void uart_isr();		// this at the uart.c location

void SetEXTI4Callback();			// in the stm32h7hxx_it.c file
void DMA2D_Callbacks_Init(void);











#endif
