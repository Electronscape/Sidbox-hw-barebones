/*
	EXT RAM MODULE, allowing for using external 8MEG RAM in this case

*/
#ifndef EXTRAM_H
#define EXTRAM_H

#include "main.h"

// VARIABLE ATTRIBUTES
#define ATTR_EXTRAM							__attribute__((section(".extram"), aligned(32)))

//#define REFRESH_COUNT                    ((uint32_t)0x0603)   /* SDRAM refresh counter (100Mhz SD clock) */
#define SDRAM_TIMEOUT                    	((uint32_t)0xFFFF)

#define FMC_CLK_FREQ 						100000000UL // Example FMC clock freq (100 MHz)

#define SDRAM_REFRESH_PERIOD 				64 // 64ms
#define SDRAM_ROW_NUM 						8192

#define REFRESH_COUNT 						((FMC_CLK_FREQ * SDRAM_REFRESH_PERIOD) / (SDRAM_ROW_NUM * 1000) - 20)

// FMC SDRAM Mode definition register defines
#define SDRAM_MODEREG_BURST_LENGTH_1             ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_LENGTH_2             ((uint16_t)0x0001)
#define SDRAM_MODEREG_BURST_LENGTH_4             ((uint16_t)0x0002)
#define SDRAM_MODEREG_BURST_LENGTH_8             ((uint16_t)0x0004)
#define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL      ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_TYPE_INTERLEAVED     ((uint16_t)0x0008)
#define SDRAM_MODEREG_CAS_LATENCY_2              ((uint16_t)0x0020)
#define SDRAM_MODEREG_CAS_LATENCY_3              ((uint16_t)0x0030)
#define SDRAM_MODEREG_OPERATING_MODE_STANDARD    ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE     ((uint16_t)0x0200)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern SRAM_HandleTypeDef hsram1;
extern SDRAM_HandleTypeDef hsdram1;

void SDRAM_Initialization_Sequence(SDRAM_HandleTypeDef *hsdram);








#endif
