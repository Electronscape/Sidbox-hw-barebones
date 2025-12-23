/*

	Joystick port, the hardare buttons port, the MOUSE controller systems will be here

*/
#include <stdlib.h>

#include <joyport/joyport.h>

#include "main.h"




static const signed int QEM[16] = {	// this one is great if needing to detect glitches
	0, -1,  1,  2,
	1,  0,  2, -1,
   -1,  2,  0,  1,
	2,  1, -1,  0
};

static char MouseEncoder = Amiga;

static signed char PLS_X, PLS_XQ, PLS_DX, PLS_DXO, PLS_DXN;
static signed char PLS_Y, PLS_YQ, PLS_DY, PLS_DYO, PLS_DYN;



// interrupts
static volatile uint8_t	js_port_bits;

int mouse_x, mouse_y;
MouseState mouse = {0};
void Mouse_updateRawDeltas(){
	switch(MouseEncoder){
		case Amiga:
			PLS_Y  = JS_A_UP;
			PLS_YQ = JS_A_LEFT;
			PLS_X  = JS_A_DOWN;
			PLS_XQ = JS_A_RIGHT;
		break;

		case AtariST:
			PLS_X  = JS_A_DOWN;
			PLS_XQ = JS_A_UP;
			PLS_Y  = JS_A_RIGHT;
			PLS_YQ = JS_A_LEFT;
		break;
	}


	PLS_DXO = PLS_DXN;	PLS_DXN = PLS_X * 2 + PLS_XQ;
	PLS_DYO = PLS_DYN;	PLS_DYN = PLS_Y * 2 + PLS_YQ;

	// decode QEM, clamp jumps like you do now
    signed int dx = QEM[PLS_DXO * 4 + PLS_DXN];
    signed int dy = QEM[PLS_DYO * 4 + PLS_DYN];

     if (abs(dx) == 2) dx = 0;
     if (abs(dy) == 2) dy = 0;

     mouse.raw_dx += dx;
     mouse.raw_dy += dy;
}



void MouseUpdateVBL(){ // called on every VBL IRQ
	float accel = 1.5f;

    // Calculate accelerated deltas
	// Apply scaled delta using float
    float dx = (float)mouse.raw_dx * accel;
    float dy = (float)mouse.raw_dy * accel;

    mouse.fx  += dx;
    mouse.fy  += dy;
    mouse.cfx += dx;
    mouse.cfy += dy;

    // Convert to integer positions
    mouse.x  = (int)mouse.fx;
    mouse.y  = (int)mouse.fy;
    mouse.cx = (int)mouse.cfx;
    mouse.cy = (int)mouse.cfy;

    // mouse deltas
    MOUSE_XD += (int)dx;
    MOUSE_YD += (int)dy;

    // delta control prevent weird wrapping
    if(MOUSE_XD < -0x1FFFFFFF) MOUSE_XD = -0x1FFFFFFF;
    if(MOUSE_XD >  0x1FFFFFFF) MOUSE_XD =  0x1FFFFFFF;
    if(MOUSE_YD < -0x1FFFFFFF) MOUSE_YD = -0x1FFFFFFF;
    if(MOUSE_YD >  0x1FFFFFFF) MOUSE_YD =  0x1FFFFFFF;

    // Clamp to screen
    if (mouse.x < 0) 	mouse.fx  = mouse.x  = 0;
    if (mouse.x > 479)  mouse.fx  = mouse.x  = 479;
    if (mouse.y < 0) 	mouse.fy  = mouse.y  = 0;
    if (mouse.y > 319)  mouse.fy  = mouse.y  = 319;

    if (mouse.cx < 0) 	mouse.cfx = mouse.cx = 0;
    if (mouse.cx > 319) mouse.cfx = mouse.cx = 319;
    if (mouse.cy < 0)   mouse.cfy = mouse.cy = 0;
    if (mouse.cy > 239) mouse.cfy = mouse.cy = 239;

    // Reset raw deltas
    mouse.raw_dx = 0;
    mouse.raw_dy = 0;

}




void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	// process externap pin change interrupts

	js_port_bits = BTN_NULL;

	uint32_t gpiod_val = GPIOD->IDR;
	uint32_t gpiog_val = GPIOG->IDR;

	// Use bit masks directly
	if (!(gpiod_val & (1 << 7)))  js_port_bits |= BTN_FIRE2;   // D7
	if (!(gpiog_val & (1 << 14))) js_port_bits |= BTN_FIRE;    // G14
	if (!(gpiog_val & (1 << 13))) js_port_bits |= BTN_UP;      // G13
	if (!(gpiog_val & (1 << 12))) js_port_bits |= BTN_DOWN;    // G12
	if (!(gpiog_val & (1 << 10))) js_port_bits |= BTN_LEFT;    // G10
	if (!(gpiog_val & (1 << 9)))  js_port_bits |= BTN_RIGHT;   // G9

	Mouse_updateRawDeltas();	// mouse direct register changes on pin changes interrupts
	UNUSED(GPIO_Pin);
}





uint8_t readJoyPort(){
	return js_port_bits;
	/*
	return	(JS_A_FIRE1 << HW_BUTTON_FIRE)  |
			(JS_A_FIRE2 << HW_BUTTON_FIRE2) |
			(JS_A_UP    << HW_BUTTON_UP)    |
			(JS_A_DOWN  << HW_BUTTON_DOWN)  |
			(JS_A_LEFT  << HW_BUTTON_LEFT)  |
			(JS_A_RIGHT << HW_BUTTON_RIGHT)	;
	*/
}

