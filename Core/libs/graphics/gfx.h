#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "main.h"
#include "display/lcd.h"

#define		GFX_DEFAULT_FPEN	27
#define		GFX_DEFAULT_BPEN	13
#define		GFX_DEFAULT_CPEN	GFX_DEFAULT_FPEN



#ifndef GFX_MAX_POLY_VERTS
#define GFX_MAX_POLY_VERTS 32
#endif


// Only Bridge between the HARDWARE to the LIB!!
//extern gfx_bitmap_t *gfx_draw_bitmap;	// for drawing
//extern gfx_bitmap_t *gfx_show_bitmap;	// for showing

extern gfx_bitmap_t *gfx_drawbuffer;	// buffers  - front layer
// HARDWARE to GFX library bridge
extern gfx_bitmap_t	gfx_fbitmap_1, gfx_fbitmap_2;
extern gfx_bitmap_t	gfx_bbitmap_1, gfx_bbitmap_2;


typedef struct {
    int16_t x;
    int16_t y;
} gfx_point_t;

void gfx_active_size(int16_t width, int16_t height);
void gfx_cls();

void gfx_setfpen(uint8_t colourindex);		// sets the active pen colour for the draw buffer
void gfx_setbpen(uint8_t colourindex);		// sets the back colour for anything filled or solid background

// draw primers //
void gfx_draw_rect(int16_t x, int16_t y, int16_t w, int16_t h);				// basic outline
void gfx_draw_rectf(int16_t x, int16_t y, int16_t w, int16_t h);			// filled rect	(using fpen)
void gfx_draw_rectfo(int16_t x, int16_t y, int16_t w, int16_t h);			// filled rect with an outline (using back pen colour: bpen)

void gfx_draw_circle(int16_t cx, int16_t cy, int16_t r);					// basic circle
void gfx_draw_circlef(int16_t cx, int16_t cy, int16_t r);					// filled circle (using fpen)
void gfx_draw_circlefo(int16_t cx, int16_t cy, int16_t r);					// filled circle with outline (using fpen, and bpen)

void gfx_draw_ellipse(int16_t cx, int16_t cy, int16_t rx, int16_t ry);		// Basic outline
void gfx_draw_ellipsef(int16_t cx, int16_t cy, int16_t rx, int16_t ry);		// filled using fpen
void gfx_draw_ellipsefo(int16_t cx, int16_t cy, int16_t rx, int16_t ry);	// filled using fpen and bpen

void gfx_draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1);			// typical line draw

void gfx_draw_poly(const gfx_point_t *pts, int16_t n, bool closed);
void gfx_draw_polyf(const gfx_point_t *pts, int16_t n);						// filled polygone draw

// loading images
void gfx_clear_bitmap(uint8_t *bitmap, uint32_t width, uint32_t height, uint8_t colour);
int gfx_loadbitmap(const char *filename, void *bitmap, uint32_t width, uint32_t height, bool lsload, bool rle);	// lsload (landscape load)

// blitting routines
void gfx_blit_rle(uint8_t *img, int16_t dx, int16_t dy, uint16_t src_w, uint16_t src_h);
void gfx_blit_raw(uint8_t *src, int16_t dx, int16_t dy, uint16_t src_w, uint16_t src_h);






#endif
