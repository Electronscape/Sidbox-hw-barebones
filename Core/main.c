/* ============================================================================
 *  Project:   SidBox 5.6 Firmware
 *  Module:    Core / Hardware Baseline
 *  Author:    Electronscape
 *  Created:   2025-DEC-21
 *
 *  Description:
 *  --------------------------------------------------------------------------
 *  Base firmware and hardware abstraction layer for the SidBox 5.6 platform.
 *
 *  This code provides the foundational system services, hardware interfaces,
 *  memory layout, and low-level drivers required by the SidBox runtime,
 *  including display, DMA, audio, storage, and system timing.
 *
 *  This project is designed for deterministic, low-latency operation on
 *  STM32H7-class hardware and serves as the execution environment for higher-
 *  level applications, emulators, and UI systems.
 *
 *  Notes:
 *  --------------------------------------------------------------------------
 *  - Performance-critical paths assume cache-aware DMA usage.
 *  - Timing-sensitive code may rely on VSYNC or hardware interrupts.
 *  - This file forms part of the OS baseline;
 *
 * ========================================================================== */



#include "main.h"
#include "audio/adc.h"
#include "audio/dac.h"
#include "dma.h"
#include "display/dma2d.h"
#include "filesys.h"
#include "sdmmc.h"
#include "spi.h"
#include "timers/tim.h"
#include <uart/uart.h>
#include "usb_device.h"
#include "gpio.h"
#include "ram/fmc.h"

#include "filesys.h"
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// kernel level systems
#include "mcuconf.h"
#include "interrupts.h"
#include "timers/timers.h"
#include "graphics/gfx.h"
#include <display/lcd.h>
#include <joyport/joyport.h>
#include "audio/audio.h"
#



extern uint8_t backpic[];
extern uint8_t testPic[];

extern
int mouse_x, mouse_y;

int main(void) {

	// PERIPHERAL AND HARDWARE INITIALISATIONS FOR CPU
	MPU_Config();
	SCB_EnableICache();
	HAL_Init();
	__HAL_RCC_SYSCFG_CLK_ENABLE();

	SystemClock_Config();
	PeriphCommonClock_Config();
	MX_GPIO_Init();


	init_language();		// init language pack
	lcd_load_default_clut();	// this goes here to load the colour table before the DMA2D starts, since it also loads the initial palette

	//Minimal_Clock_RTC_Enable();
	MX_DMA_Init();
	MX_FMC_Init();
	MX_DAC1_Init();
	MX_USART1_UART_Init();
	MX_SDMMC1_SD_Init();
	MX_FATFS_Init();
	//MX_USB_DEVICE_Init();
	//MX_SPI1_Init();
	//MX_SPI2_Init();
	//MX_SPI3_Init();
	//MX_SPI4_Init();
	//MX_SPI5_Init();
	//MX_TIM5_Init();
	//MX_TIM7_Init();
	//MX_DMA2D_Init();
	MX_TIM3_Init();
	//MX_ADC3_Init();
	/////////////////////////////////////////

	// SIDBOX actualy firmware!!

	//DMA2D_DisplayInit(SF_SCREENWIDTH, SF_SCREENHEIGHT, SF_SCREENWIDTH, SF_SCREENHEIGHT, DISPFLAG_DUALLAYER);
	DMA2D_DisplayInit(SF_SCREENWIDTH, SF_SCREENHEIGHT, 960, 640, DISPFLAG_DUALLAYER | DISPFLAG_SCROLLABLE);	// big background
	DMA2D_Callbacks_Init();	// for when /if we make use of the DMA2D transfer CLUT and Transfer to LCD are done.
	SetEXTI4Callback();	// the default call back for EXTI interrupt, THIS will change when we go into the emulator


	uint32_t *hwcsh = (uint32_t *)CRASHBOOT_MEM_LOCATION;	// store the something a crash occurs;
	// can check if there is something here, if there is DO something nice on screen ;)

	if(*hwcsh){
		int crsh = 0;
		if(*hwcsh == MAGICBOOT_CRASH_LEVEL1) crsh = 1;
		if(*hwcsh == MAGICBOOT_CRASH_LEVEL2) crsh = 1;
		if(*hwcsh == MAGICBOOT_CRASH_LEVEL3) crsh = 1;
		if(*hwcsh == MAGICBOOT_CRASH_LEVEL8) crsh = 1;

		if(crsh){
			dbug("%s!\n", lang_get(STR_CRASH_ALERT));
			*hwcsh = 0x00000000;	// RESET FFS CRASH

			// some graphics info to show // but for demo testing just a simple text on screen
			LCD_CrashDisplay();	// Startup the LCD port
			HWLCD_CLEAR_SCREEN(0);								// Clear the screen
			LCD_PIXEL_COLOUR_SET(31);
			bios_text(20, 20, lang_get(STR_CRASH_ALERT));
			bios_text(40, 40, lang_get(STR_CRASH_INFOWENTTOUART));
			bios_text(40, 80, lang_get(STR_CRASH_PRESSSCREEN));
			HAL_TIM_Base_Start(&hTmrBrightnessPWM);	// begin lcd bright PWM
			HAL_TIM_PWM_Start(&hTmrBrightnessPWM, TIM_CHANNEL_1);   // start pwm generation
			lcd_setbrightness(240);
			// if we cant get past this, then mr gremlin has moved in, got a family!

			uint8_t btnExit = 0;
			while(!btnExit){ btnExit = (JS_A_FIRE1 || JS_A_FIRE2);}	// no buttons pressed
			delayMs(10);
			while(btnExit){ btnExit = (JS_A_FIRE1 || JS_A_FIRE2);}	// while buttons ARE pressed

			dbug("rebooting...\n");
			delayMs(500);
			instReboot();
		}
	}

	*hwcsh = 0x00000000;	// RESET FFS CRASH

	//resetUART();
	flushCmd();


	dbug("%s", lang_get(STR_LONG_TEST));


	//uint8_t *backmem = (uint8_t *)(uintptr_t)0xD0600000;	// our external memory (dont care area right now)
	uint32_t size = 960u * 640u;        // 8bpp assumption

	volatile uint8_t *backmem = (volatile uint8_t *)(uintptr_t)0xD0000000;// + size;	// at the end of the EXTRAM userspace

	//LCD_Init_FMC(LCD_STARTUP);	// Startup the LCD port
	LCD_InitHW(220, 60);	// Startup the LCD port
	// now we can re-assign the backbuffers
	lcd_set_backbitmap(backmem, backmem, 960, 640);	// backmem used twice, as draw and suface buffers, its a non updating image, save memory no memory flipping
	gfx_clear_bitmap(backmem, 960, 640, 0);





	HWLCD_CLEAR_SCREEN(0);								// Clear the screen
	bios_text(0, 0, lang_get(STR_BIOS_START));

	HAL_TIM_Base_Start(&hTmrBrightnessPWM);	// begin lcd bright PWM
	HAL_TIM_PWM_Start(&hTmrBrightnessPWM, TIM_CHANNEL_1);   // start pwm generation
	lcd_setbrightness(240);

	delayMs(540);
	SCB_EnableDCache();
	SCB_EnableICache();
	lcd_vram_clear();

	dbug("   -= SIDBOX =- : build: "build"\r\n"); //(II6)
	dbug("######################################################\r\n\r\n");
	dbug("%s...\n", lang_get(STR_SDCARDINIT));

	startAudioDriver();
	StartPlayerDMA();
	//StartADCSmapler(44100);	// audio sampler demo

	if (isDiskPresent()) {
		DiskMount();
		FATFSCHECK();
	} else
		dbug("[!] %s\n", lang_get(STR_SDCARD_NOT_PRESENT));

	//int gfxl = gfx_loadbitmap("/christmasrle.ppb", backmem, 960, 640, true, true);
	//int gfxl = gfx_loadbitmap("/christmas2.ppb", backmem, 960, 640, false, false);

	uint32_t *cluts = lcd_get_clut();
	int gfxl = gfx_loadbitmap("/xmasg1.ppb", backmem, false, cluts);

	if(gfxl){
		dbug("Image loaded!\n");
	} else dbug("could not find image\n");
	SCB_CleanDCache_by_Addr((uint32_t*)backmem, (int32_t)size);  // size in BYTES

	lcd_update_palette();


	gfx_point_t points[4];

	points[0].x = 20; points[0].y = 20;
	points[1].x = 120; points[1].y = 80;
	points[2].x = 80; points[2].y = 120;
	points[3].x = 40; points[3].y = 10;
	//points[0].x = 20; points[0].x = 20;

	int sbx, sby, sx, sy;

	sbx = 2;
	sby = 2;
	sx = 30;
	sy = 40;

	int db = 1;

	lcd_usebitmap(&gfx_bbitmap_1);
	//gfx_blit_raw(backpic, 0, 0, 480, 320);

	lcd_usebitmap(&gfx_fbitmap_1);
	doLCDTest();

	int scrollx = 0, scrolly = 0;
	int scrollsx = 1, scrollsy = 1;

	while (1) {
		// DO OS STUFF
		doDiskHeartBeat();

		uart_buffercheck_timer();

		// buffer switching
		db = 1 - db;
		if(db){
			lcd_showfbitmap(&gfx_fbitmap_1);	// set the buffer to go to LCD output
			lcd_usebitmap(&gfx_fbitmap_2);		// set the draw buffer
		} else {
			lcd_showfbitmap(&gfx_fbitmap_2);
			lcd_usebitmap(&gfx_fbitmap_1);
		}

		if(scrollx < 0 || scrollx > 480) scrollsx = -scrollsx;
		if(scrolly < 0 || scrolly > 320) scrollsy = -scrollsy;

		scrollx+= scrollsx;
		scrolly+= scrollsy;

		api_scroll_backlayer(scrollx, scrolly);

		// logic test
		if(sx < -60 || sx > 470) sbx = -sbx;
		if(sy < -60 || sy > 310) sby = -sby;

		sx += sbx;
		sy += sby;

		points[2].x = sx;
		points[2].y = sy;


		gfx_cls();	// does what it says, clear the bitmap!
		gfx_setfpen(29);
		gfx_draw_circle(sx, sy, 30);
		gfx_draw_polyf(points, 4);

		gfx_blit_rle(testPic, sx, sy, 128, 128);

		gfx_setfpen(253);

		gfx_draw_circlef(mouse.x, mouse.y, 8);
		if(mouse.x > 400 && mouse.y > 300){
			report_crash();	// fake crash system
		}

		uint8_t joyin = readJoyPort();
		if(joyin){
			int i;
			gfx_setfpen(86);

			for(i = 0; i < 6; i++){
				if( joyin & (1 << i) ){
					gfx_draw_circlef(20 + (i * 40), 270, 15);
				}
			}



		}

		lcd_update();	// update screen
		asm("nop");
	}
}
