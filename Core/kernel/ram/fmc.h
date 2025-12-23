#ifndef __FMC_H
#define __FMC_H


#include "main.h"


extern SRAM_HandleTypeDef hsram1;
extern SDRAM_HandleTypeDef hsdram1;


void MX_FMC_Init(void);
void HAL_SRAM_MspInit(SRAM_HandleTypeDef* hsram);
void HAL_SRAM_MspDeInit(SRAM_HandleTypeDef* hsram);
void HAL_SDRAM_MspInit(SDRAM_HandleTypeDef* hsdram);
void HAL_SDRAM_MspDeInit(SDRAM_HandleTypeDef* hsdram);

#endif /*__FMC_H */
