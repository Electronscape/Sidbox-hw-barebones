#include "main.h"
#include "stm32h7xx_it.h"
#include "watchdog.h"

void (*EXTI4_Callback_t)(void);

extern PCD_HandleTypeDef hpcd_USB_OTG_FS;
extern DMA_HandleTypeDef hdma_dac1_ch1;
extern DMA_HandleTypeDef hdma_dac1_ch2;
extern DAC_HandleTypeDef hdac1;
extern DMA2D_HandleTypeDef hdma2d;
extern DMA_HandleTypeDef hdma_spi1_tx;
extern DMA_HandleTypeDef hdma_spi2_tx;
extern DMA_HandleTypeDef hdma_spi3_tx;
extern DMA_HandleTypeDef hdma_spi4_tx;
extern DMA_HandleTypeDef hdma_spi5_tx;
extern SPI_HandleTypeDef hspi1;
extern SPI_HandleTypeDef hspi2;
extern SPI_HandleTypeDef hspi3;
extern SPI_HandleTypeDef hspi4;
extern SPI_HandleTypeDef hspi5;
extern TIM_HandleTypeDef hTmrAudioSampler;
extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef hTmrAudioDMARate;
extern TIM_HandleTypeDef htim7;
extern UART_HandleTypeDef huart1;

void NMI_Handler(void) {
	dbug("NMI_Handler: Crash\r\n");
	while (1) {
	}
}

void HardFault_Handler(void) {
	/* USER CODE BEGIN HardFault_IRQn 0 */

	//register uint32_t _msp __asm("msp");
	uint32_t *stack = (uint32_t*) __get_MSP();
	uint32_t *hwinf = (uint32_t*) CRASHBOOT_MEM_INFO;	// store the error bytes
	uint32_t *hwcsh = (uint32_t*) CRASHBOOT_MEM_LOCATION;// store the something shit happened flat
	//uint32_t *stack = (uint32_t *)_msp;

	*hwcsh = MAGICBOOT_CRASH_LEVEL1;	// FFS CRASH

	uint32_t r0 = stack[0];
	uint32_t r1 = stack[1];
	uint32_t r2 = stack[2];
	uint32_t r3 = stack[3];
	uint32_t r12 = stack[4];
	uint32_t lr = stack[5];  // Link Register
	uint32_t pc = stack[6];  // Program Counter at crash
	uint32_t psr = stack[7];  // Program Status Register

	hwinf[0] = stack[0];
	hwinf[1] = stack[1];
	hwinf[2] = stack[2];
	hwinf[3] = stack[3];
	hwinf[4] = stack[4];
	hwinf[5] = stack[5];
	hwinf[6] = stack[6];
	hwinf[7] = stack[7];
	hwinf[8] = SCB->CFSR;
	hwinf[9] = SCB->MMFAR;
	hwinf[10] = SCB->BFAR;
	hwinf[11] = SCB->HFSR;

	dbug("\r\n--- HardFault ---\r\n");
	dbug(" R0  = 0x%08lX\r\n", r0);
	dbug(" R1  = 0x%08lX\r\n", r1);
	dbug(" R2  = 0x%08lX\r\n", r2);
	dbug(" R3  = 0x%08lX\r\n", r3);
	dbug(" R12 = 0x%08lX\r\n", r12);
	dbug(" LR  = 0x%08lX\r\n", lr);
	dbug(" PC  = 0x%08lX\r\n", pc);
	dbug(" xPSR= 0x%08lX\r\n", psr);

	dbug("CFSR  = 0x%08lX\r\n", SCB->CFSR);
	dbug("MMFAR = 0x%08lX\r\n", SCB->MMFAR);
	dbug("BFAR  = 0x%08lX\r\n", SCB->BFAR);

	dbug("FFS(HWF): %08X - CFSR: %8X - ", SCB->HFSR, SCB->CFSR);
	NVIC_SystemReset();

	while (1)
		;  // Halt
}

void MemManage_Handler(void) {

	//register uint32_t _msp __asm("msp");
	uint32_t *stack = (uint32_t*) __get_MSP();
	uint32_t *hwinf = (uint32_t*) CRASHBOOT_MEM_INFO;	// store the error bytes
	uint32_t *hwcsh = (uint32_t*) CRASHBOOT_MEM_LOCATION;// store the something shit happened flat
	*hwcsh = MAGICBOOT_CRASH_LEVEL2;	// FFS CRASH

	uint32_t r0 = stack[0];
	uint32_t r1 = stack[1];
	uint32_t r2 = stack[2];
	uint32_t r3 = stack[3];
	uint32_t r12 = stack[4];
	uint32_t lr = stack[5];  // Link Register
	uint32_t pc = stack[6];  // Program Counter at crash
	uint32_t psr = stack[7];  // Program Status Register

	hwinf[0] = stack[0];
	hwinf[1] = stack[1];
	hwinf[2] = stack[2];
	hwinf[3] = stack[3];
	hwinf[4] = stack[4];
	hwinf[5] = stack[5];
	hwinf[6] = stack[6];
	hwinf[7] = stack[7];
	hwinf[8] = SCB->CFSR;
	hwinf[9] = SCB->MMFAR;
	hwinf[10] = SCB->BFAR;
	hwinf[11] = SCB->HFSR;

	dbug("\r\n--- MEMORY FAULT HANDLER ---\r\n");
	dbug(" R0  = 0x%08lX\r\n", r0);
	dbug(" R1  = 0x%08lX\r\n", r1);
	dbug(" R2  = 0x%08lX\r\n", r2);
	dbug(" R3  = 0x%08lX\r\n", r3);
	dbug(" R12 = 0x%08lX\r\n", r12);
	dbug(" LR  = 0x%08lX\r\n", lr);
	dbug(" PC  = 0x%08lX\r\n", pc);
	dbug(" xPSR= 0x%08lX\r\n", psr);

	dbug("CFSR  = 0x%08lX\r\n", SCB->CFSR);
	dbug("MMFAR = 0x%08lX\r\n", SCB->MMFAR);
	dbug("BFAR  = 0x%08lX\r\n", SCB->BFAR);

	dbug("---MEMORY---\r\n");
	dbug("HFSR  = 0x%08lX\r\n", SCB->HFSR);

	//CRASH_TRAP_Guru("MEMORY HANDLER FAULT", SCB->HFSR, SCB->CFSR, "");
	NVIC_SystemReset();
	while (1) {
	}
}

void BusFault_Handler(void) {
	//register uint32_t _msp __asm("msp");
	uint32_t *stack = (uint32_t*) __get_MSP();
	uint32_t *hwinf = (uint32_t*) CRASHBOOT_MEM_INFO;	// store the error bytes
	uint32_t *hwcsh = (uint32_t*) CRASHBOOT_MEM_LOCATION;// store the something shit happened flat
	//uint32_t *stack = (uint32_t *)_msp;

	*hwcsh = MAGICBOOT_CRASH_LEVEL3;	// FFS CRASH

	uint32_t r0 = stack[0];
	uint32_t r1 = stack[1];
	uint32_t r2 = stack[2];
	uint32_t r3 = stack[3];
	uint32_t r12 = stack[4];
	uint32_t lr = stack[5];  // Link Register
	uint32_t pc = stack[6];  // Program Counter at crash
	uint32_t psr = stack[7];  // Program Status Register

	hwinf[0] = stack[0];
	hwinf[1] = stack[1];
	hwinf[2] = stack[2];
	hwinf[3] = stack[3];
	hwinf[4] = stack[4];
	hwinf[5] = stack[5];
	hwinf[6] = stack[6];
	hwinf[7] = stack[7];
	hwinf[8] = SCB->CFSR;
	hwinf[9] = SCB->MMFAR;
	hwinf[10] = SCB->BFAR;
	hwinf[11] = SCB->HFSR;

	dbug("\r\n--- Bus Fault ---\r\n");
	dbug(" R0  = 0x%08lX\r\n", r0);
	dbug(" R1  = 0x%08lX\r\n", r1);
	dbug(" R2  = 0x%08lX\r\n", r2);
	dbug(" R3  = 0x%08lX\r\n", r3);
	dbug(" R12 = 0x%08lX\r\n", r12);
	dbug(" LR  = 0x%08lX\r\n", lr);
	dbug(" PC  = 0x%08lX\r\n", pc);
	dbug(" xPSR= 0x%08lX\r\n", psr);

	dbug("CFSR  = 0x%08lX\r\n", SCB->CFSR);
	dbug("MMFAR = 0x%08lX\r\n", SCB->MMFAR);
	dbug("BFAR  = 0x%08lX\r\n", SCB->BFAR);

	dbug("FFS(BUS): %08X - CFSR: %8X - ", SCB->HFSR, SCB->CFSR);
	NVIC_SystemReset();

	//CRASH_TRAP_Guru("BUS FAULT", SCB->HFSR, SCB->CFSR);
	while (1) {
	}
}

void UsageFault_Handler(void) {
	//register uint32_t _msp __asm("msp");
	uint32_t *stack = (uint32_t*) __get_MSP();
	uint32_t *hwinf = (uint32_t*) CRASHBOOT_MEM_INFO;	// store the error bytes
	uint32_t *hwcsh = (uint32_t*) CRASHBOOT_MEM_LOCATION;// store the something shit happened flat
	//uint32_t *stack = (uint32_t *)_msp;

	*hwcsh = MAGICBOOT_CRASH_LEVEL3;	// FFS CRASH

	uint32_t r0 = stack[0];
	uint32_t r1 = stack[1];
	uint32_t r2 = stack[2];
	uint32_t r3 = stack[3];
	uint32_t r12 = stack[4];
	uint32_t lr = stack[5];  // Link Register
	uint32_t pc = stack[6];  // Program Counter at crash
	uint32_t psr = stack[7];  // Program Status Register

	hwinf[0] = stack[0];
	hwinf[1] = stack[1];
	hwinf[2] = stack[2];
	hwinf[3] = stack[3];
	hwinf[4] = stack[4];
	hwinf[5] = stack[5];
	hwinf[6] = stack[6];
	hwinf[7] = stack[7];
	hwinf[8] = SCB->CFSR;
	hwinf[9] = SCB->MMFAR;
	hwinf[10] = SCB->BFAR;
	hwinf[11] = SCB->HFSR;

	dbug("\r\n--- Usage Fault ---\r\n");
	dbug(" R0  = 0x%08lX\r\n", r0);
	dbug(" R1  = 0x%08lX\r\n", r1);
	dbug(" R2  = 0x%08lX\r\n", r2);
	dbug(" R3  = 0x%08lX\r\n", r3);
	dbug(" R12 = 0x%08lX\r\n", r12);
	dbug(" LR  = 0x%08lX\r\n", lr);
	dbug(" PC  = 0x%08lX\r\n", pc);
	dbug(" xPSR= 0x%08lX\r\n", psr);

	dbug("CFSR  = 0x%08lX\r\n", SCB->CFSR);
	dbug("MMFAR = 0x%08lX\r\n", SCB->MMFAR);
	dbug("BFAR  = 0x%08lX\r\n", SCB->BFAR);

	dbug("FFS(HWF): %08X - CFSR: %8X - ", SCB->HFSR, SCB->CFSR);
	NVIC_SystemReset();

	//CRASH_TRAP_Guru("FFS - WHAT NOW?", SCB->HFSR, SCB->CFSR);
	while (1) {
	}
}

void SVC_Handler(void) {

}

void DebugMon_Handler(void) {

}

void PendSV_Handler(void) {

}

void SysTick_Handler(void) {
	HAL_IncTick();
}

void EXTI4_IRQHandler(void) {
	EXTI4_Callback_t();
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);	// VSYNC pin
}

void DMA1_Stream2_IRQHandler(void) {
	HAL_DMA_IRQHandler(&hdma_dac1_ch1);
}

void DMA1_Stream4_IRQHandler(void) {
	HAL_DMA_IRQHandler(&hdma_dac1_ch2);
}

void EXTI9_5_IRQHandler(void) {
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_9);
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_7);
}

void TIM2_IRQHandler(void) {
	HAL_TIM_IRQHandler(&hTmrAudioSampler);
}

void SPI1_IRQHandler(void) {
	HAL_SPI_IRQHandler(&hspi1);
}

void SPI2_IRQHandler(void) {
	HAL_SPI_IRQHandler(&hspi2);
}

void USART1_IRQHandler(void) {
	HAL_UART_IRQHandler(&huart1);
}

void EXTI15_10_IRQHandler(void) {
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_10);
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_12);
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_14);
}

void TIM5_IRQHandler(void) {
	HAL_TIM_IRQHandler(&htim5);
}

void SPI3_IRQHandler(void) {
	HAL_SPI_IRQHandler(&hspi3);
}

void TIM6_DAC_IRQHandler(void) {
	HAL_DAC_IRQHandler(&hdac1);
	HAL_TIM_IRQHandler(&hTmrAudioDMARate);
}

void TIM7_IRQHandler(void) {
	HAL_TIM_IRQHandler(&htim7);
}

void DMA2_Stream0_IRQHandler(void) {
	HAL_DMA_IRQHandler(&hdma_spi5_tx);
}

void DMA2_Stream1_IRQHandler(void) {
	HAL_DMA_IRQHandler(&hdma_spi3_tx);
}

void DMA2_Stream2_IRQHandler(void) {
	HAL_DMA_IRQHandler(&hdma_spi2_tx);
}

void DMA2_Stream3_IRQHandler(void) {
	HAL_DMA_IRQHandler(&hdma_spi1_tx);
}

void DMA2_Stream4_IRQHandler(void) {
	HAL_DMA_IRQHandler(&hdma_spi4_tx);
}

void SPI4_IRQHandler(void) {
	HAL_SPI_IRQHandler(&hspi4);
}

void SPI5_IRQHandler(void) {
	HAL_SPI_IRQHandler(&hspi5);
}

void DMA2D_IRQHandler(void) {
	HAL_DMA2D_IRQHandler(&hdma2d);
}

void OTG_FS_IRQHandler(void) {
	HAL_PCD_IRQHandler(&hpcd_USB_OTG_FS);
}

__weak void HAL_GPEXT4_ScreenUpdate() {
}
__weak void SMS_ScreenFDate_IRQHandler() {
}

void SetEXTI4Callback() {
	EXTI4_Callback_t = HAL_GPEXT4_ScreenUpdate;
}

void SetEXTI4CallbackSMS() {
	EXTI4_Callback_t = SMS_ScreenFDate_IRQHandler;
}
