#include "fmc.h"
#include "ram/extram.h"
#include "timers/timers.h"

SRAM_HandleTypeDef hsram1;
SDRAM_HandleTypeDef hsdram1;

void MX_FMC_Init(void) {

	FMC_NORSRAM_TimingTypeDef Timing = { 0 };
	FMC_SDRAM_TimingTypeDef SdramTiming = { 0 };

	//* Perform the SRAM1 memory initialization sequence
	hsram1.Instance = FMC_NORSRAM_DEVICE;
	hsram1.Extended = FMC_NORSRAM_EXTENDED_DEVICE;
	/* hsram1.Init */
	hsram1.Init.NSBank = FMC_NORSRAM_BANK1;
	hsram1.Init.DataAddressMux = FMC_DATA_ADDRESS_MUX_DISABLE;
	hsram1.Init.MemoryType = FMC_MEMORY_TYPE_SRAM;
	hsram1.Init.MemoryDataWidth = FMC_NORSRAM_MEM_BUS_WIDTH_16;
	hsram1.Init.BurstAccessMode = FMC_BURST_ACCESS_MODE_DISABLE;
	hsram1.Init.WaitSignalPolarity = FMC_WAIT_SIGNAL_POLARITY_HIGH;
	hsram1.Init.WaitSignalActive = FMC_WAIT_TIMING_BEFORE_WS;
	hsram1.Init.WriteOperation = FMC_WRITE_OPERATION_ENABLE;
	hsram1.Init.WaitSignal = FMC_WAIT_SIGNAL_DISABLE;
	hsram1.Init.ExtendedMode = FMC_EXTENDED_MODE_DISABLE;
	hsram1.Init.AsynchronousWait = FMC_ASYNCHRONOUS_WAIT_DISABLE;
	hsram1.Init.WriteBurst = FMC_WRITE_BURST_ENABLE;
	hsram1.Init.ContinuousClock = FMC_CONTINUOUS_CLOCK_SYNC_ASYNC;
	hsram1.Init.WriteFifo = FMC_WRITE_FIFO_ENABLE;
	hsram1.Init.PageSize = FMC_PAGE_SIZE_NONE;
	/* Timing */
	Timing.AddressSetupTime = 1;
	Timing.AddressHoldTime = 1;
	Timing.DataSetupTime = 2;
	Timing.BusTurnAroundDuration = 1;
	Timing.CLKDivision = 1;
	Timing.DataLatency = 1;
	Timing.AccessMode = FMC_ACCESS_MODE_A;
	/* ExtTiming */

	// Perform the SDRAM1 memory initialization sequence - EXTERNAL RAM
	hsdram1.Instance = FMC_SDRAM_DEVICE;
	/* hsdram1.Init */
	hsdram1.Init.SDBank = FMC_SDRAM_BANK2;
	hsdram1.Init.ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_8;
	hsdram1.Init.RowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_12;
	hsdram1.Init.MemoryDataWidth = FMC_SDRAM_MEM_BUS_WIDTH_16;
	hsdram1.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;
	hsdram1.Init.CASLatency = FMC_SDRAM_CAS_LATENCY_3;
	hsdram1.Init.WriteProtection = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
	hsdram1.Init.SDClockPeriod = FMC_SDRAM_CLOCK_PERIOD_2;
	hsdram1.Init.ReadBurst = FMC_SDRAM_RBURST_ENABLE;
	hsdram1.Init.ReadPipeDelay = FMC_SDRAM_RPIPE_DELAY_2;
	/* SdramTiming */
	SdramTiming.LoadToActiveDelay = 2;
	SdramTiming.ExitSelfRefreshDelay = 9;
	SdramTiming.SelfRefreshTime = 5;
	SdramTiming.RowCycleDelay = 8;
	SdramTiming.WriteRecoveryTime = 3;
	SdramTiming.RPDelay = 3;
	SdramTiming.RCDDelay = 3;

	if (HAL_SDRAM_Init(&hsdram1, &SdramTiming) != HAL_OK) {
		dbug("HW STARTUP FAILED SRAM_INIT(external ram)\r\n");
		NVIC_SystemReset();
		Error_Handler();
	}
	delayMs(10);

	if (HAL_SRAM_Init(&hsram1, &Timing, NULL) != HAL_OK) {
		dbug("HW STARTUP FAILED SDRAM_INIT(lcd 16bit bus)\r\n");

		NVIC_SystemReset();
		Error_Handler();
	}

	delayMs(10);
	SDRAM_Initialization_Sequence(&hsdram1);// init the RAM on board / 8meg byte ram
}

static uint32_t FMC_Initialized = 0;
static uint32_t FMC_DeInitialized = 0;


static void HAL_FMC_MspInit(void) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	if (FMC_Initialized) {
		return;
	}
	FMC_Initialized = 1;
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = { 0 };
	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_FMC;
	PeriphClkInitStruct.FmcClockSelection = RCC_FMCCLKSOURCE_D1HCLK;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) {
		Error_Handler();
	}
	__HAL_RCC_FMC_CLK_ENABLE();

	/** FMC GPIO Configuration
	 PF0   ------> FMC_A0
	 PF1   ------> FMC_A1
	 PF2   ------> FMC_A2
	 PF3   ------> FMC_A3
	 PF4   ------> FMC_A4
	 PF5   ------> FMC_A5
	 PH5   ------> FMC_SDNWE
	 PF11   ------> FMC_SDNRAS
	 PF12   ------> FMC_A6
	 PF13   ------> FMC_A7
	 PF14   ------> FMC_A8
	 PF15   ------> FMC_A9
	 PG0   ------> FMC_A10
	 PG1   ------> FMC_A11
	 PE7   ------> FMC_D4
	 PE8   ------> FMC_D5
	 PE9   ------> FMC_D6
	 PE10   ------> FMC_D7
	 PE11   ------> FMC_D8
	 PE12   ------> FMC_D9
	 PE13   ------> FMC_D10
	 PE14   ------> FMC_D11
	 PE15   ------> FMC_D12
	 PH6   ------> FMC_SDNE1
	 PH7   ------> FMC_SDCKE1
	 PD8   ------> FMC_D13
	 PD9   ------> FMC_D14
	 PD10   ------> FMC_D15
	 PD13   ------> FMC_A18
	 PD14   ------> FMC_D0
	 PD15   ------> FMC_D1
	 PG4   ------> FMC_BA0
	 PG5   ------> FMC_BA1
	 PG8   ------> FMC_SDCLK
	 PC7   ------> FMC_NE1
	 PD0   ------> FMC_D2
	 PD1   ------> FMC_D3
	 PD4   ------> FMC_NOE
	 PD5   ------> FMC_NWE
	 PG15   ------> FMC_SDNCAS
	 PE0   ------> FMC_NBL0
	 PE1   ------> FMC_NBL1
	 */
	/* GPIO_InitStruct */
	GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3
			| GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13
			| GPIO_PIN_14 | GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF12_FMC;

	HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

	/* GPIO_InitStruct */
	GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF12_FMC;

	HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

	/* GPIO_InitStruct */
	GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5
			| GPIO_PIN_8 | GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF12_FMC;

	HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

	/* GPIO_InitStruct */
	GPIO_InitStruct.Pin = GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10
			| GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14
			| GPIO_PIN_15 | GPIO_PIN_0 | GPIO_PIN_1;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF12_FMC;

	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	/* GPIO_InitStruct */
	GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_13
			| GPIO_PIN_14 | GPIO_PIN_15 | GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4
			| GPIO_PIN_5;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF12_FMC;

	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	/* GPIO_InitStruct */
	GPIO_InitStruct.Pin = GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF9_FMC;

	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

void HAL_SRAM_MspInit(SRAM_HandleTypeDef *sramHandle) {
	UNUSED(sramHandle);
	HAL_FMC_MspInit();
}

void HAL_SDRAM_MspInit(SDRAM_HandleTypeDef *sdramHandle) {
	UNUSED(sdramHandle);
	HAL_FMC_MspInit();
}


static void HAL_FMC_MspDeInit(void) {
	if (FMC_DeInitialized) {
		return;
	}
	FMC_DeInitialized = 1;
	/* Peripheral clock enable */
	__HAL_RCC_FMC_CLK_DISABLE();

	/** FMC GPIO Configuration
	 PF0   ------> FMC_A0
	 PF1   ------> FMC_A1
	 PF2   ------> FMC_A2
	 PF3   ------> FMC_A3
	 PF4   ------> FMC_A4
	 PF5   ------> FMC_A5
	 PH5   ------> FMC_SDNWE
	 PF11   ------> FMC_SDNRAS
	 PF12   ------> FMC_A6
	 PF13   ------> FMC_A7
	 PF14   ------> FMC_A8
	 PF15   ------> FMC_A9
	 PG0   ------> FMC_A10
	 PG1   ------> FMC_A11
	 PE7   ------> FMC_D4
	 PE8   ------> FMC_D5
	 PE9   ------> FMC_D6
	 PE10   ------> FMC_D7
	 PE11   ------> FMC_D8
	 PE12   ------> FMC_D9
	 PE13   ------> FMC_D10
	 PE14   ------> FMC_D11
	 PE15   ------> FMC_D12
	 PH6   ------> FMC_SDNE1
	 PH7   ------> FMC_SDCKE1
	 PD8   ------> FMC_D13
	 PD9   ------> FMC_D14
	 PD10   ------> FMC_D15
	 PD13   ------> FMC_A18
	 PD14   ------> FMC_D0
	 PD15   ------> FMC_D1
	 PG4   ------> FMC_BA0
	 PG5   ------> FMC_BA1
	 PG8   ------> FMC_SDCLK
	 PC7   ------> FMC_NE1
	 PD0   ------> FMC_D2
	 PD1   ------> FMC_D3
	 PD4   ------> FMC_NOE
	 PD5   ------> FMC_NWE
	 PG15   ------> FMC_SDNCAS
	 PE0   ------> FMC_NBL0
	 PE1   ------> FMC_NBL1
	 */

	HAL_GPIO_DeInit(GPIOF,
			GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4
					| GPIO_PIN_5 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13
					| GPIO_PIN_14 | GPIO_PIN_15);

	HAL_GPIO_DeInit(GPIOH, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);

	HAL_GPIO_DeInit(GPIOG,
			GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_8
					| GPIO_PIN_15);

	HAL_GPIO_DeInit(GPIOE,
			GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11
					| GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15
					| GPIO_PIN_0 | GPIO_PIN_1);

	HAL_GPIO_DeInit(GPIOD,
			GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_13 | GPIO_PIN_14
					| GPIO_PIN_15 | GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4
					| GPIO_PIN_5);

	HAL_GPIO_DeInit(GPIOC, GPIO_PIN_7);
}

void HAL_SRAM_MspDeInit(SRAM_HandleTypeDef *sramHandle) {
	UNUSED(sramHandle);
	HAL_FMC_MspDeInit();
}

void HAL_SDRAM_MspDeInit(SDRAM_HandleTypeDef *sdramHandle) {
	UNUSED(sdramHandle);
	HAL_FMC_MspDeInit();
}

