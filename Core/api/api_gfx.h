#ifndef API_GFX_H
#define API_GFX_H



// LCD DEFINES ------
#define FPS_25hz	0
#define FPS_40hz	1
#define FPS_50hz	2
#define FPS_60hz	3

// GRAPHICS ARCHITECTURE //
#define SF_SCREENWIDTH  			480	// default
#define SF_SCREENHEIGHT 			320
#define SF_STRIDE      				SF_SCREENHEIGHT  // Assuming 320 bytes per column in DBBufferDraw

#define GFX_PIXELNORM				0	// 0 single pixel
#define GFX_PIXELDBL				1	// 1 double width, height size

// DISPLAY FLAGS	-- INIT DISPLAY FLAGS	(DISPFLAG_DISPLAYON | DISPFLAG_DUALLAYER | DISPFLAG_BUFFERWAIT)
#define DISPFLAG_DISPLAYAUTO		0x01	// auto refresh - dont need to call to render (its experimental)
#define DISPFLAG_DUALLAYER			0x02	// enable dual layer mode, otherwise single layer
#define DISPFLAG_SCROLLABLE			0x04	// enable the screen to scroll too
//#define DISPFLAG_VSYNC_EN			0x08	// enable the screen with vsync wait (otherwise its up to you to use vsync)
#define DISPFLAG_BUFFERWAIT			0x80	// (experimental fun)
#define DISPFLAG_DEFAULTS			0x00	// nothing special

// use this on the api_lcdrender(flags)
#define DISPRM_1LAYER				0x00
#define DISPRM_2LAYER				0x01
#define DISPRM_VBLANKWAIT			0x80




#endif
