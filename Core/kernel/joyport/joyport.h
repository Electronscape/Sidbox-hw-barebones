#ifndef JOYPORT_H
#define JOYPORT_H

#include "main.h"

/* -----------------------------------------------------------------------------------------

	1 - VPULSE 	: UP
	2 - HPULSE 	: DOWN
	3 - VQPULSE : LEFT
	4 - HQPULSE : RIGHT
	5 - middle  button
	6 - MOUSE LEFT (button 1)
	7 - +5V
	8 - GND
	9 - MOUSE RIGHT (button 2)

 * MALE VIEW - on the sidbox side - PINS
    -------------
    \ 1 2 3 4 5 /
     \ 6 7 8 9 /
      --------

 * FEMALE VIEW - on the joystick/mouse it self - HOLES
    -------------
    \ 5 4 3 2 1 /
     \ 9 8 7 6 /
      --------
----------------------------------------------------------------------------------------- */





#define BTN_NULL	0x00
#define BTN_FIRE 	0x01
#define BTN_FIRE2 	0x02
#define BTN_UP 		0x04
#define BTN_DOWN 	0x08
#define BTN_LEFT 	0x10
#define BTN_RIGHT 	0x20

#define HW_BUTTON_FIRE		0
#define HW_BUTTON_FIRE2   	1 // this is physically not connected to any of the buttons on the sidbox
#define HW_BUTTON_UP		2
#define HW_BUTTON_DOWN		3
#define HW_BUTTON_LEFT		4
#define HW_BUTTON_RIGHT		5


// Released state
#define JS_A_FIRE1_		(!!(GPIOG->IDR & 0x4000))	// HARD BUTTONS FIRE / OK	//G14
#define JS_A_FIRE2_		(!!(GPIOD->IDR & 0x0080))	// HARD BUTTONS FIRE / OK2	//D7
#define JS_A_UP_  		(!!(GPIOG->IDR & 0x2000))	// HARD BUTTONS UP			//G13
#define JS_A_DOWN_		(!!(GPIOG->IDR & 0x1000))	// HARD BUTTONS DOWN		//G12
#define JS_A_LEFT_		(!!(GPIOG->IDR & 0x0400))	// HARD BUTTONS LEFT		//G10
#define JS_A_RIGHT_		(!!(GPIOG->IDR & 0x0200))	// HARD BUTTONS RIGHT		//G9

//	Pressed Down
#define JS_A_FIRE1		(!(GPIOG->IDR & 0x4000))	// HARD BUTTONS FIRE / OK	//G14	// PIN 6
#define JS_A_FIRE2		(!(GPIOD->IDR & 0x0080))	// HARD BUTTONS FIRE / OK2	//D7	// PIN 9
#define JS_A_UP			(!(GPIOG->IDR & 0x2000))	// HARD BUTTONS UP			//G13	// PIN 1
#define JS_A_DOWN		(!(GPIOG->IDR & 0x1000))	// HARD BUTTONS DOWN		//G12	// PIN 2
#define JS_A_LEFT		(!(GPIOG->IDR & 0x0400))	// HARD BUTTONS LEFT		//G10	// PIN 3
#define JS_A_RIGHT		(!(GPIOG->IDR & 0x0200))	// HARD BUTTONS RIGHT		//G9	// PIN 4


//extern volatile uint8_t	js_port_bits;




enum MouseEncoded {
	Amiga,
	AtariST
};


typedef struct {
    int x, y;           // LCD current cursor position
    int cx, cy;			// CRT x,y
    int raw_dx, raw_dy; // accumulated raw delta counts since last sample
    int acc_dx, acc_dy; // accelerated deltas applied this frame

    int lx, ly, ix, iy;	// last vs is, location

    float fx, fy;		// LCD subpixel precision
    float cfx, cfy;		// CRT subpixel prec
} MouseState;

#define LCD_MOUSE_X		mouse.x
#define LCD_MOUSE_Y		mouse.y

#define	MOUSE_XD		mouse.acc_dx
#define	MOUSE_YD		mouse.acc_dy

#define	LCD_MOUSE_X_ 	mouse.lx
#define	LCD_MOUSE_Y_ 	mouse.ly

#define LCD_MOUSE_XX	mouse.ix
#define LCD_MOUSE_YY	mouse.iy

extern MouseState mouse;


void MouseUpdateVBL();
uint8_t readJoyPort();















#endif
