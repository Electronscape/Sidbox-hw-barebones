/*
 * ============================================================
 *  lcd.h — LCD Hardware Mapping + Driver Interface
 * ============================================================
 *
 *  Purpose
 *  -------
 *  Declares the hardware mapping, constants, and public driver
 *  API for the LCD module. This file is the “single source of
 *  truth” for how the MCU is wired to the display controller
 *  and what the driver exposes to the rest of the system.
 *
 *  Display
 *  -------
 *  Controller : ILI9488
 *  Module ID  : Z400IT001 (V0.0, 2019-10-01)
 *  Glass Mark : 040240C7
 *
 *  Electrical Interface
 *  --------------------
 *  Bus        : 16-bit parallel via FMC
 *  Control    : NE1 / Axx (RS) / NWE / NOE (see pin map below)
 *  Data       : D0..D15 (see pin map below)
 *
 *  Memory Map
 *  ----------
 *  The LCD is accessed through an FMC-mapped address which
 *  exposes two 16-bit registers:
 *    - LCD_REG : command/register select
 *    - LCD_OUT : data write/read
 *
 *  VRAM / Buffers
 *  --------------
 *  This header defines buffer sizing and any attributes used to
 *  place framebuffers in external RAM (if fitted). Actual VRAM
 *  allocation and ownership lives elsewhere (see vram module).
 *
 *  Usage Notes
 *  ----------
 *  - Do not place function definitions in this header.
 *  - Keep this file focused on interface + wiring + constants.
 *  - Higher-level drawing/graphics primitives should live in
 *    the graphics library, not here.
 *
 *  Pin Map Reference
 *  -----------------
 *  CONTROL:
 *    CS  = PC7  (FMC_NE1)
 *    RS  = PD13 (FMC_A18 / Register Select)
 *    WR  = PD5  (NWE)
 *    RD  = PD4  (NOE)
 *
 *  DATA:
 *   --- DATA PINS -----
 	 d0  =  PD14  =>  22
 	 d1  =  PD15  =>  21
 	 d2  =  PD0   =>  20
 	 d3  =  PD1   =>  19
 	 d4  =  PE7   =>  18
 	 d5  =  PE8   =>  17
 	 d6  =  PE9   =>  16
 	 d7  =  PE10  =>  15
 	 d8  =  PE11  =>  14
 	 d9  =  PE12  =>  13
	d10  =  PE13  =>  12
	d11  =  PE14  =>  11
	d12  =  PE15  =>  10
	d13  =  PD8   =>   9
	d14  =  PD9   =>   8
	d15  =  PD10  =>   7
 * ============================================================
*/





#ifndef HARDWARE_LCD_H_
#define HARDWARE_LCD_H_

#include "main.h"

// VARIABLE ATTRIBUTES
#define ATTR_EXTRAM_VRAM	__attribute__((section(".vram"), aligned(16)))


// DATA BUS
#define LCD_CTRL_PORT		GPIOI->BSRR		// Physical Pin out on PORT I
#define LCD_RESETCTRL_PORT	GPIOE->BSRR		// Physical Pin out on PORT E
//#define LCD_CTRLcc		GPIOA->BSRR	// not sure whatt his is yet


#define LCD_RESET			(LCD_RESETCTRL_PORT = 0x08)		// PortE Pin 3
#define LCD_RESET_			(LCD_RESETCTRL_PORT = 0x080000)	// PortE Pin 3
#define LCD_DATABUS			GPIOB->ODR

// LCD DISPLAY CONSTRAINTS
#define LCD_LANDSCAPE		// defined for telling system this is rendered sideways


#define LCD_WIDTH			480
#define LCD_HEIGHT			320
#define LCD_SCREEN_RAMSIZE	(LCD_WIDTH * LCD_HEIGHT)


typedef struct lcd_params_st {
    uint16_t lcd_width;
    uint16_t lcd_height;
    uint16_t lcd_id;
    uint8_t  lcd_direction;
    uint16_t wram_cmd;
    uint16_t set_x_cmd;
    uint16_t set_y_cmd;
} lcd_params_t;

typedef struct lcd_fmc_address_st {
    uint16_t lcd_reg;	// register/command
    uint16_t lcd_ram;	// lcd-data
} lcd_fmc_address_t;

lcd_params_t lcd_params;		// parameters for the LCD - not implemented yet!

#define LCD_VSYNCIN				(!!(GPIOE->IDR & 0x10))		// VSYNC pulse from LCD

#define LCD_BANK_ADDR			0x60000000
#define LCD_BASE    			((uint32_t)(LCD_BANK_ADDR | 0x0007FFFE))	// memory address on the FMC bus
#define LCD         			((lcd_fmc_address_t*)LCD_BASE)			// just a pointer to the FMC bus the LCD sits on

#define LCD_OUT					LCD->lcd_ram
#define LCD_REG					LCD->lcd_reg

// default base setups
#define	LCD_STARTUP				220, 1		// basic bios boot
#define LCD_SCREEN_60hz			220, 60		// for 60Hz refresh
#define LCD_SCREEN_50hz			220, 50		// for 50hz refresh
#define LCD_SCREEN_OS			220, 25		// 25hz refresh - mainly used for the OS

// DRIVER CONFIG BYTES -------------------------------------------------------------------------------------
// ###################################################################################################### //
// ###################################################################################################### //
// LCD DRIVER BYTES ----------------------------------------------------------------------------------------
#define ILI9488_CMD_NOP                             0x00
#define ILI9488_CMD_SOFTWARE_RESET                  0x01
#define ILI9488_CMD_READ_DISP_ID                    0x04
#define ILI9488_CMD_READ_DSI_ERRORS                 0x05
#define ILI9488_CMD_READ_DISP_STATUS                0x09
#define ILI9488_CMD_READ_DISP_POWER_MODE            0x0A
#define ILI9488_CMD_READ_DISP_MADCTRL               0x0B
#define ILI9488_CMD_READ_DISP_PIXEL_FORMAT          0x0C
#define ILI9488_CMD_READ_DISP_IMAGE_MODE            0x0D
#define ILI9488_CMD_READ_DISP_SIGNAL_MODE           0x0E
#define ILI9488_CMD_READ_DISP_SELF_DIAGNOSTIC       0x0F
#define ILI9488_CMD_SLEEP_IN                        0x10
#define ILI9488_CMD_SLEEP_OUT                       0x11
#define ILI9488_CMD_PARTIAL_MODE_ON                 0x12
#define ILI9488_CMD_NORMAL_DISP_MODE_ON             0x13
#define ILI9488_CMD_DISP_INVERSION_OFF              0x20
#define ILI9488_CMD_DISP_INVERSION_ON               0x21
#define ILI9488_CMD_ALL_PIXELS_OFF                  0x22
#define ILI9488_CMD_ALL_PIXELS_ON                   0x23
#define ILI9488_CMD_DISPLAY_OFF                     0x28
#define ILI9488_CMD_DISPLAY_ON                      0x29
#define ILI9488_CMD_COLUMN_ADDRESS_SET              0x2A
#define ILI9488_CMD_PAGE_ADDRESS_SET                0x2B
#define ILI9488_CMD_MEMORY_WRITE                    0x2C
#define ILI9488_CMD_MEMORY_READ                     0x2E
#define ILI9488_CMD_PARTIAL_AREA                    0x30
#define ILI9488_CMD_VERT_SCROLL_DEFINITION          0x33
#define ILI9488_CMD_TEARING_EFFECT_LINE_OFF         0x34
#define ILI9488_CMD_TEARING_EFFECT_LINE_ON          0x35
#define ILI9488_CMD_MEMORY_ACCESS_CONTROL           0x36
#define ILI9488_CMD_VERT_SCROLL_START_ADDRESS       0x37
#define ILI9488_CMD_IDLE_MODE_OFF                   0x38
#define ILI9488_CMD_IDLE_MODE_ON                    0x39
#define ILI9488_CMD_INTERFACE_PIXEL_FORMAT          0x3A
#define ILI9488_CMD_WRITE_MEMORY_CONTINUE           0x3C
#define ILI9488_CMD_READ_MEMORY_CONTINUE            0x3E
#define ILI9488_CMD_WRITE_TEAR_SCANLINE             0x44
#define ILI9488_CMD_READ_TEAR_SCANLINE              0x45
#define ILI9488_CMD_WRITE_DISPLAY_BRIGHTNESS        0x51
#define ILI9488_CMD_READ_DISPLAY_BRIGHTNESS         0x52
#define ILI9488_CMD_WRITE_CTRL_DISPLAY              0x53
#define ILI9488_CMD_READ_CTRL_DISPLAY               0x54
#define ILI9488_CMD_WRITE_CONTENT_ADAPT_BRIGHTNESS  0x55
#define ILI9488_CMD_READ_CONTENT_ADAPT_BRIGHTNESS   0x56
#define ILI9488_CMD_WRITE_MIN_CAB_LEVEL             0x5E
#define ILI9488_CMD_READ_MIN_CAB_LEVEL              0x5F
#define ILI9488_CMD_READ_BRIGHTNESS_DIAG_RESULT     0x68
#define ILI9488_CMD_READ_ID1                        0xDA
#define ILI9488_CMD_READ_ID2                        0xDB
#define ILI9488_CMD_READ_ID3                        0xDC

/* Level 2 Commands (from the display Datasheet) */
#define ILI9488_CMD_INTERFACE_MODE_CONTROL          0xB0
#define ILI9488_CMD_FRAME_RATE_CONTROL_NORMAL       0xB1
#define ILI9488_CMD_FRAME_RATE_CONTROL_IDLE_8COLOR  0xB2
#define ILI9488_CMD_FRAME_RATE_CONTROL_PARTIAL      0xB3
#define ILI9488_CMD_DISPLAY_INVERSION_CONTROL       0xB4
#define ILI9488_CMD_BLANKING_PORCH_CONTROL          0xB5
#define ILI9488_CMD_DISPLAY_FUNCTION_CONTROL        0xB6
#define ILI9488_CMD_ENTRY_MODE_SET                  0xB7
#define ILI9488_CMD_COLOR_ENHANCEMENT_CTRL_1        0xB9
#define ILI9488_CMD_COLOR_ENHANCEMENT_CTRL_2        0xBA
#define ILI9488_CMD_HS_LANES_CONTROL                0xBE
#define ILI9488_CMD_POWER_CONTROL_1                 0xC0
#define ILI9488_CMD_POWER_CONTROL_2                 0xC1
#define ILI9488_CMD_POWER_CONTROL_3                 0xC2
#define ILI9488_CMD_POWER_CONTROL_4                 0xC3
#define ILI9488_CMD_POWER_CONTROL_5                 0xC4
#define ILI9488_CMD_VCOM_CONTROL_1                  0xC5
#define ILI9488_CMD_CABC_CONTROL_1                  0xC6
#define ILI9488_CMD_CABC_CONTROL_2                  0xC8
#define ILI9488_CMD_CABC_CONTROL_3                  0xC9
#define ILI9488_CMD_CABC_CONTROL_4                  0xCA
#define ILI9488_CMD_CABC_CONTROL_5                  0xCB
#define ILI9488_CMD_CABC_CONTROL_6                  0xCC
#define ILI9488_CMD_CABC_CONTROL_7                  0xCD
#define ILI9488_CMD_CABC_CONTROL_8                  0xCE
#define ILI9488_CMD_CABC_CONTROL_9                  0xCF
#define ILI9488_CMD_NVMEM_WRITE                     0xD0
#define ILI9488_CMD_NVMEM_PROTECTION_KEY            0xD1
#define ILI9488_CMD_NVMEM_STATUS_READ               0xD2
#define ILI9488_CMD_READ_ID4                        0xD3
#define ILI9488_CMD_ADJUST_CONTROL_1                0xD7
#define ILI9488_CMD_READ_ID4_CHECK                  0xD8
#define ILI9488_CMD_POSITIVE_GAMMA_CTRL             0xE0
#define ILI9488_CMD_NEGATIVE_GAMMA_CTRL             0xE1
#define ILI9488_CMD_DIGITAL_GAMMA_CONTROL_1         0xE2
#define ILI9488_CMD_DIGITAL_GAMMA_CONTROL_2         0xE3
#define ILI9488_CMD_SET_IMAGE_FUNCTION              0xE9
#define ILI9488_CMD_ADJUST_CONTROL_2                0xF2
#define ILI9488_CMD_ADJUST_CONTROL_3                0xF7
#define ILI9488_CMD_ADJUST_CONTROL_4                0xF8
#define ILI9488_CMD_ADJUST_CONTROL_5                0xF9
#define ILI9488_CMD_SPI_READ_CMD_SETTING            0xFB
#define ILI9488_CMD_ADJUST_CONTROL_6                0xFC
#define ILI9488_CMD_ADJUST_CONTROL_7                0xFF



//LCD_COM(0x36,(1<<6)|(1<<3));//0 degree MY=0,MX=0,MV=0,ML=0,BGR=1,MH=0
//LCD_COM(0x36,(1<<3)|(1<<4)|(1<<5));//90 degree MY=0,MX=1,MV=1,ML=1,BGR=1,MH=0
//LCD_COM(0x36, dat);//90 degree MY=0,MX=1,MV=1,ML=1,BGR=1,MH=0
//LCD_COM(0x36, (1<<7) | (1<<6) | (1<<5)| (1<<3));//180 degree MY=0,MX=0,MV=0,ML=0,BGR=1,MH=0


//LCD_COM(0x36, (1<<7) | (1<<6) | (1<<5) | (1<<3));//270 degree MY=1,MX=1,MV=1,ML=0,BGR=1,MH=0
//LCD_COM(0x36, dat);//270 degree MY=1,MX=1,MV=1,ML=0,BGR=1,MH=0

//-------------------------- CHIP HW layer configs ------------//
#define USE_HAL_DMA2D_REGISTER_CALLBACKS 1U

//-------------------------- externals ------------------------//
extern 			DMA2D_HandleTypeDef hdma2d;	// this is used for the DMA2D hardware
extern volatile DMA2D_CLUTCfgTypeDef clut1;

#define SHARED_CLUT_ADDR  (0x0801E000u)	// this is from the BOOTLOADER area - should never change
#define SHARED_LOGO_ADDR  (0x0801E200u)	// this is where the logo is stored - is used for rendering the DIRECT TO SCREEN though

#define shared_clut ((const uint16_t *)(uintptr_t)SHARED_CLUT_ADDR)	// find this in the sidboxbl5.6.map LOOK UP logo or CLUT
#define shared_logo ((const uint8_t  *)(uintptr_t)SHARED_LOGO_ADDR)



typedef struct {
	uint8_t		*bitmap;		// base pointer to video memory area
	uint16_t	width;			// bitmap width
	uint16_t	height;			// bitmap height
	uint16_t	stride;			// pixels per column/ "scan line")
	uint32_t	memspacelen;	// the total ramspace length
} gfx_bitmap_t;

void LCD_InitHW(unsigned short dat, char fps);
void LCD_CrashDisplay();

void setLCDBrightness(int bright);

void LCD_PIXEL_COLOUR_SET(uint8_t colourid);
void HWLCD_CLEAR_SCREEN(uint8_t colourID);
void bios_text(long x, long y, const char *textptr);

void lcd_setbrightness(int bright);
void lcd_load_default_clut();					// load the default Colour palette from ROM to CLUT-colour_ram
void lcd_update(void);								// a simple update lcd, call this to put what ever is in the draw buffers to the screen

// program flow controls
void lcd_showbitmap2(gfx_bitmap_t *showfront, gfx_bitmap_t *showback);
void lcd_showfbitmap(gfx_bitmap_t *showfront);
void lcd_showbbitmap(gfx_bitmap_t *showback);
void lcd_usebitmap(gfx_bitmap_t *drawbitmap);


void doLCDTest();								// a test pattern system


void lcd_load_new_clut(uint32_t *newclut);		// new colour table to graphics system
uint32_t *lcd_get_clut();						// return the CLUT ARRAY as pointer
void lcd_vram_clear();		// CLEAR initial VRAM buffers


#endif
