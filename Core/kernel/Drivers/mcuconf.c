#include "main.h"
//#include "adc.h"
//#include "dac.h"
//#include "dma.h"
//#include "dma2d.h"
//#include "fatfs.h"
//#include "sdmmc.h"
//#include "spi.h"
//#include "tim.h"
//#include "usart.h"
//#include "usb_device.h"
//#include "gpio.h"
//#include "fmc.h"


void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

	while (!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {
	}

	HAL_PWR_EnableBkUpAccess();
	__HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_HIGH);
	__HAL_RCC_PLL_PLLSOURCE_CONFIG(RCC_PLLSOURCE_HSE);
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48 | RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_LSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
	RCC_OscInitStruct.LSEState = RCC_LSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 1;
	RCC_OscInitStruct.PLL.PLLN = 112;	// this is ment to be 112
	RCC_OscInitStruct.PLL.PLLP = 2;
	RCC_OscInitStruct.PLL.PLLQ = 8;
	RCC_OscInitStruct.PLL.PLLR = 2;
	RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;
	RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
	RCC_OscInitStruct.PLL.PLLFRACN = 6144;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 | RCC_CLOCKTYPE_D3PCLK1
			| RCC_CLOCKTYPE_D1PCLK1;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
	RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK) {
		Error_Handler();
	}
}

void PeriphCommonClock_Config(void) {
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = { 0 };
	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC
			| RCC_PERIPHCLK_SDMMC;
	PeriphClkInitStruct.PLL2.PLL2M = 1;
	PeriphClkInitStruct.PLL2.PLL2N = 18;
	PeriphClkInitStruct.PLL2.PLL2P = 2;
	PeriphClkInitStruct.PLL2.PLL2Q = 1;
	PeriphClkInitStruct.PLL2.PLL2R = 1;
	PeriphClkInitStruct.PLL2.PLL2RGE = RCC_PLL2VCIRANGE_3;
	PeriphClkInitStruct.PLL2.PLL2VCOSEL = RCC_PLL2VCOMEDIUM;
	PeriphClkInitStruct.PLL2.PLL2FRACN = 6144;
	PeriphClkInitStruct.SdmmcClockSelection = RCC_SDMMCCLKSOURCE_PLL2;
	PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_PLL2;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) {
		Error_Handler();
	}
}

void MPU_Config(void) {
	SCB_InvalidateDCache();
	SCB_InvalidateICache();

	HAL_MPU_Disable();

	MPU_Region_InitTypeDef MPU_InitStruct = { 0 };

	// FMC/LCD region
	MPU_InitStruct.Enable = MPU_REGION_ENABLE;
	MPU_InitStruct.Number = MPU_REGION_NUMBER0;
	MPU_InitStruct.BaseAddress = 0x60000000;
	MPU_InitStruct.Size = MPU_REGION_SIZE_8MB;
	MPU_InitStruct.SubRegionDisable = 0x0;
	MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL1;	// MPU_TEX_LEVEL0
	MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
	MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
	MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
	MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;	//MPU_ACCESS_CACHEABLE
	MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
	HAL_MPU_ConfigRegion(&MPU_InitStruct);

	// SDRAM region (applet code)
	MPU_InitStruct.Enable = MPU_REGION_ENABLE;  // <- CRITICAL LINE
	MPU_InitStruct.Number = MPU_REGION_NUMBER1;
	MPU_InitStruct.BaseAddress = 0xD0000000;
	MPU_InitStruct.Size = MPU_REGION_SIZE_8MB;
	MPU_InitStruct.SubRegionDisable = 0x00;
	MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL1;
	MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
	MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
	MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
	MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
	MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
	HAL_MPU_ConfigRegion(&MPU_InitStruct);

	__DSB();
	__ISB();
	SCB_CleanInvalidateDCache();
	HAL_MPU_Enable(MPU_HFNMI_PRIVDEF);
}

void Error_Handler(void) {
	__disable_irq();
	dbug("ERROR HANDLER!\r\n");

	//register uint32_t _msp __asm("msp");
	uint32_t *stack = (uint32_t*) __get_MSP();
	uint32_t *hwinf = (uint32_t*) 0x2001FFC0;	// store the error bytes
	uint32_t *hwcsh = (uint32_t*) 0x2001FFFC;// store the something shit happened flat
	//uint32_t *stack = (uint32_t *)_msp;

	*hwcsh = 0xFF5C7A55;	// FFS CRASH

	uint32_t r0 = stack[0];
	uint32_t r1 = stack[1];
	uint32_t r2 = stack[2];
	uint32_t r3 = stack[3];
	uint32_t r12 = stack[4];
	uint32_t lr = stack[5];  // Link Register
	uint32_t pc = stack[6];  // Program Counter at crash
	uint32_t psr = stack[7];  // Program Status Register

	if (pc == 0x00) {
		// didnt reboot properly just restart
		*hwcsh = 0x00000000;	// just clear it
		NVIC_SystemReset();
	}

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
}

