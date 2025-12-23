/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#include "stm32h7xx_hal.h"
#include "watchdog.h"


#include "../build_number.h"
#include "lang/lang.h"
#include "uart/uart.h"


void Error_Handler(void);


#define LCD_RESET_Pin GPIO_PIN_3
#define LCD_RESET_GPIO_Port GPIOE
#define LCD_VSYNC_Pin GPIO_PIN_4
#define LCD_VSYNC_GPIO_Port GPIOE
#define VDI_Bright_Pin GPIO_PIN_6
#define VDI_Bright_GPIO_Port GPIOE
#define SDCARD_SELECTCD_Pin GPIO_PIN_10
#define SDCARD_SELECTCD_GPIO_Port GPIOI
#define VDI_SYNCPULSE_Pin GPIO_PIN_9
#define VDI_SYNCPULSE_GPIO_Port GPIOF
#define VDI_BLUE_Pin GPIO_PIN_1
#define VDI_BLUE_GPIO_Port GPIOC
#define TP_CS_Pin GPIO_PIN_2
#define TP_CS_GPIO_Port GPIOC
#define TP_IRQ_Pin GPIO_PIN_3
#define TP_IRQ_GPIO_Port GPIOC
#define TP_MISO_Pin GPIO_PIN_0
#define TP_MISO_GPIO_Port GPIOA
#define TP_MOSI_Pin GPIO_PIN_1
#define TP_MOSI_GPIO_Port GPIOA
#define TP_CLK_Pin GPIO_PIN_2
#define TP_CLK_GPIO_Port GPIOA
#define SDCARD_LED_STAT_Pin GPIO_PIN_4
#define SDCARD_LED_STAT_GPIO_Port GPIOH
#define LCD_BRIGHT_Pin GPIO_PIN_6
#define LCD_BRIGHT_GPIO_Port GPIOA
#define VDI_GREEN_Pin GPIO_PIN_7
#define VDI_GREEN_GPIO_Port GPIOA
#define TX1_OUT_Pin GPIO_PIN_9
#define TX1_OUT_GPIO_Port GPIOA
#define R1X_IN_Pin GPIO_PIN_10
#define R1X_IN_GPIO_Port GPIOA
#define BATT_DETECT_Pin GPIO_PIN_1
#define BATT_DETECT_GPIO_Port GPIOI
#define SBOOTR_Pin GPIO_PIN_2
#define SBOOTR_GPIO_Port GPIOI
#define SDCARD_DETECT_Pin GPIO_PIN_11
#define SDCARD_DETECT_GPIO_Port GPIOC
#define VDI_RED_Pin GPIO_PIN_6
#define VDI_RED_GPIO_Port GPIOD
#define BTN_OK2_Pin GPIO_PIN_7
#define BTN_OK2_GPIO_Port GPIOD
#define BTN_RIGHT_Pin GPIO_PIN_9
#define BTN_RIGHT_GPIO_Port GPIOG
#define BTN_LEFT_Pin GPIO_PIN_10
#define BTN_LEFT_GPIO_Port GPIOG
#define BTN_DOWN_Pin GPIO_PIN_12
#define BTN_DOWN_GPIO_Port GPIOG
#define BTN_UP_Pin GPIO_PIN_13
#define BTN_UP_GPIO_Port GPIOG
#define BTN_OK_Pin GPIO_PIN_14
#define BTN_OK_GPIO_Port GPIOG
#define TAPE_OUT_Pin GPIO_PIN_7
#define TAPE_OUT_GPIO_Port GPIOI

#define RGB24(r,g,b)    (r<<16)|(g<<8)|b        // 0..31, 0..63, 0..31

//#define SDCARDSELECT			(!!(GPOG->ODR

#define ALIGN32(x) (((x) + 31) & ~31)
#define ALIGNSCR32(x) (((x) + CACHE_LINE_SIZE - 1) & ~(CACHE_LINE_SIZE - 1))

typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned long dword;
typedef uint8_t bool;
//#define bool uint8_t

#define false 		0
#define true 		1

#define UBYTE unsigned char
#define ULONG unsigned long

extern TIM_HandleTypeDef htim4;
extern DMA_HandleTypeDef hdma_tim4_ch1;
extern TIM_HandleTypeDef hTmrBrightnessPWM;
extern DMA_HandleTypeDef hdma_tim3_ch1;

// IRQ SETTINGS! GOD THERE ARE A FEW!!
// HAL_NVIC_SetPriorityGrouping
// grouping 2
#if(0)
#define IRQ_UART			0, 1	// * NEVER OFF
#define IRQ_VBL				1, 0	// * NEVER OFF
#define IRQ_GFX				1, 2	// * NEVER OFF, was 3, 3

#define IRQ_CRTDMA_1		2, 2
#define IRQ_CRTDMA_2		2, 1
#define IRQ_CRTDMA_3		2, 1
#define IRQ_CRTDMA_4		2, 1
#define IRQ_CRTDMA_5		2, 1
#define IRQ_CRTDMA_6		2, 1
#define IRQ_CRTDMA_7		2, 1

#define IRQ_DAC				3, 0	// was 1, 2
#define IRQ_EMULATOR_TMRS	3, 1	// was 1, 0 -- in the Emulator envonirnment, usually other IRQ are turned off!!
// or, but not both
#define IRQ_TMR16			3, 1
#define IRQ_TMR17			3, 2

#define IRQ_CRTSPI_1		3, 3
#define IRQ_CRTSPI_2		3, 3
#define IRQ_CRTSPI_3		3, 3
#define IRQ_CRTSPI_4		3, 3
#define IRQ_CRTSPI_5		3, 3	// was 0, 0

#define IRQ_MOUSESAMP		4, 0

#define IRQ_JOYPORTQ 		4, 1	// * NEVER OFF
#define IRQ_JOYPORTR 		5, 2    // * NEVER OFF

#define IRQ_WATCHDOG		6, 0	// * NEVER OFF
#define IRQ_AUDIOPOLL		6, 1

#define IRQ_SYSTEMRESTART	7, 0	// * NEVER OFF
#endif
//////////////////////////////////////////




#define IRQ_UART			0, 0	// * NEVER OFF
#define IRQ_VBL				2, 0	// * NEVER OFF
#define IRQ_DAC				1, 0	// was 1, 2
#define IRQ_AUDIOPOLL		1, 1

#define IRQ_GFX				3, 0	// * NEVER OFF, was 3, 3


#define IRQ_CRTDMA_1		4, 0
#define IRQ_CRTDMA_2		4, 0
#define IRQ_CRTDMA_3		4, 0
#define IRQ_CRTDMA_4		4, 0
#define IRQ_CRTDMA_5		4, 0
#define IRQ_CRTDMA_6		4, 0
#define IRQ_CRTDMA_7		4, 0

#define IRQ_CRTSPI_1		4, 1
#define IRQ_CRTSPI_2		4, 1
#define IRQ_CRTSPI_3		4, 1
#define IRQ_CRTSPI_4		4, 1
#define IRQ_CRTSPI_5		4, 1	// was 0, 0


#define IRQ_TMR16			5, 1
#define IRQ_TMR17			5, 2

#define IRQ_EMULATOR_TMRS	6, 1	// was 1, 0 -- in the Emulator envonirnment, usually other IRQ are turned off!!
// or, but not both

#define IRQ_MOUSESAMP		6, 1
#define IRQ_JOYPORTQ 		6, 0	// * NEVER OFF
#define IRQ_JOYPORTR 		7, 1    // * NEVER OFF


#define IRQ_WATCHDOG		7, 0	// * NEVER OFF
#define IRQ_SYSTEMRESTART	7, 0	// * NEVER OFF


#define CONFIG_BIT_KEYBEEP	0x01
#define CONFIG_BIT_CRTOUT 	0x02
#define CONFIG_BIT_USBFILE	0x04

#define min(X,Y) ((X) < (Y) ? (X) : (Y))
#define max(X,Y) ((X) > (Y) ? (Y) : (X))


////// APPLET SYSTEMS ////////////
#define API_MAX_FILES_FSFIL		4


#endif /* __MAIN_H */
