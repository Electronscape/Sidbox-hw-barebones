#ifndef __TIM_H__
#define __TIM_H__

#include "main.h"

extern TIM_HandleTypeDef hTmrAudioSampler;
extern TIM_HandleTypeDef hTmrBrightnessPWM;
extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef hTmrAudioDMARate;
extern TIM_HandleTypeDef htim7;

void MX_TIM2_Init(void);
void MX_TIM3_Init(void);
void MX_TIM5_Init(void);
void MX_TIM6_Init(void);
void MX_TIM7_Init(void);

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

#endif
