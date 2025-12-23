#include "main.h"
#include "interrupts.h"
#include "audio/audio.h"
//void SystemClock_Config(void);
//void PeriphCommonClock_Config(void);
//static void MPU_Config(void);

// VBL Vsync Intterupt found in the display/LCD.C   HAL_GPEXT4_ScreenUpdate();

extern TIM_HandleTypeDef hTmrAudioSampler;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	// this timer is turned on when we're using MICA MODE; should always be turned off otherwise!

	if (htim == &hTmrAudioSampler) {	// audio systems
		processSampler();
	}
}

// serial port interrupts.
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if (huart->Instance == USART1) uart_isr();
}
