#include "tim.h"

TIM_HandleTypeDef hTmrAudioSampler;		// timer 2
TIM_HandleTypeDef hTmrBrightnessPWM;	// timer 3
TIM_HandleTypeDef htim5;
TIM_HandleTypeDef hTmrAudioDMARate;		// timer 6
TIM_HandleTypeDef htim7;

void MX_TIM2_Init(void) {
	TIM_ClockConfigTypeDef sClockSourceConfig = { 0 };
	TIM_MasterConfigTypeDef sMasterConfig = { 0 };
	hTmrAudioSampler.Instance = TIM2;
	hTmrAudioSampler.Init.Prescaler = 49;
	hTmrAudioSampler.Init.CounterMode = TIM_COUNTERMODE_UP;
	hTmrAudioSampler.Init.Period = 44100;
	hTmrAudioSampler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	hTmrAudioSampler.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&hTmrAudioSampler) != HAL_OK) {
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&hTmrAudioSampler, &sClockSourceConfig) != HAL_OK) {
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&hTmrAudioSampler, &sMasterConfig) != HAL_OK) {
		Error_Handler();
	}
}

void MX_TIM3_Init(void) {
	TIM_ClockConfigTypeDef sClockSourceConfig = { 0 };
	TIM_MasterConfigTypeDef sMasterConfig = { 0 };
	TIM_OC_InitTypeDef sConfigOC = { 0 };

	hTmrBrightnessPWM.Instance = TIM3;
	hTmrBrightnessPWM.Init.Prescaler = 8;
	hTmrBrightnessPWM.Init.CounterMode = TIM_COUNTERMODE_UP;
	hTmrBrightnessPWM.Init.Period = 255;
	hTmrBrightnessPWM.Init.ClockDivision = TIM_CLOCKDIVISION_DIV4;
	hTmrBrightnessPWM.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	if (HAL_TIM_Base_Init(&hTmrBrightnessPWM) != HAL_OK) {
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&hTmrBrightnessPWM, &sClockSourceConfig) != HAL_OK) {
		Error_Handler();
	}
	if (HAL_TIM_PWM_Init(&hTmrBrightnessPWM) != HAL_OK) {
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&hTmrBrightnessPWM, &sMasterConfig) != HAL_OK) {
		Error_Handler();
	}
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 255;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	if (HAL_TIM_PWM_ConfigChannel(&hTmrBrightnessPWM, &sConfigOC, TIM_CHANNEL_1) != HAL_OK) {
		Error_Handler();
	}
	HAL_TIM_MspPostInit(&hTmrBrightnessPWM);

}

void MX_TIM5_Init(void) {
	TIM_ClockConfigTypeDef sClockSourceConfig = { 0 };
	TIM_MasterConfigTypeDef sMasterConfig = { 0 };
	htim5.Instance = TIM5;
	htim5.Init.Prescaler = 49;
	htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim5.Init.Period = 44000 - 1;
	htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV4;
	htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim5) != HAL_OK) {
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim5, &sClockSourceConfig) != HAL_OK) {
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig) != HAL_OK) {
		Error_Handler();
	}
}

void MX_TIM6_Init(void) {
	TIM_MasterConfigTypeDef sMasterConfig = { 0 };
	hTmrAudioDMARate.Instance = TIM6;
	hTmrAudioDMARate.Init.Prescaler = 0;
	hTmrAudioDMARate.Init.CounterMode = TIM_COUNTERMODE_UP;
	hTmrAudioDMARate.Init.Period = 2750;
	hTmrAudioDMARate.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&hTmrAudioDMARate) != HAL_OK) {
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&hTmrAudioDMARate, &sMasterConfig)
			!= HAL_OK) {
		Error_Handler();
	}
}

void MX_TIM7_Init(void) {
	TIM_MasterConfigTypeDef sMasterConfig = { 0 };
	htim7.Instance = TIM7;
	htim7.Init.Prescaler = 440;
	htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim7.Init.Period = 2;
	htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	if (HAL_TIM_Base_Init(&htim7) != HAL_OK) {
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) != HAL_OK) {
		Error_Handler();
	}
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *tim_baseHandle) {
	if (tim_baseHandle->Instance == TIM2) {
		__HAL_RCC_TIM2_CLK_ENABLE();
		HAL_NVIC_SetPriority(TIM2_IRQn, IRQ_AUDIOPOLL);
		HAL_NVIC_EnableIRQ(TIM2_IRQn);

	} else if (tim_baseHandle->Instance == TIM3) {
		__HAL_RCC_TIM3_CLK_ENABLE();

	} else if (tim_baseHandle->Instance == TIM5) {
		__HAL_RCC_TIM5_CLK_ENABLE();
		HAL_NVIC_SetPriority(TIM5_IRQn, IRQ_WATCHDOG);
		HAL_NVIC_EnableIRQ(TIM5_IRQn);

	} else if (tim_baseHandle->Instance == TIM6) {
		__HAL_RCC_TIM6_CLK_ENABLE();
		HAL_NVIC_SetPriority(TIM6_DAC_IRQn, IRQ_DAC);
		HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);

	} else if (tim_baseHandle->Instance == TIM7) {
		__HAL_RCC_TIM7_CLK_ENABLE();
		HAL_NVIC_SetPriority(TIM7_IRQn, IRQ_EMULATOR_TMRS);
		HAL_NVIC_EnableIRQ(TIM7_IRQn);

	}
}

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *timHandle) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	if (timHandle->Instance == TIM3) {
		__HAL_RCC_GPIOA_CLK_ENABLE();
		/**TIM3 GPIO Configuration	 PA6     ------> TIM3_CH1 */
		GPIO_InitStruct.Pin = LCD_BRIGHT_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
		GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
		HAL_GPIO_Init(LCD_BRIGHT_GPIO_Port, &GPIO_InitStruct);
	}
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef *tim_baseHandle) {
	if (tim_baseHandle->Instance == TIM2) {
		__HAL_RCC_TIM2_CLK_DISABLE();
		HAL_NVIC_DisableIRQ(TIM2_IRQn);

	} else if (tim_baseHandle->Instance == TIM3) {
		__HAL_RCC_TIM3_CLK_DISABLE();

	} else if (tim_baseHandle->Instance == TIM5) {
		__HAL_RCC_TIM5_CLK_DISABLE();
		HAL_NVIC_DisableIRQ(TIM5_IRQn);

	} else if (tim_baseHandle->Instance == TIM6) {
		__HAL_RCC_TIM6_CLK_DISABLE();

	} else if (tim_baseHandle->Instance == TIM7) {
		__HAL_RCC_TIM7_CLK_DISABLE();
		HAL_NVIC_DisableIRQ(TIM7_IRQn);
	}
}

