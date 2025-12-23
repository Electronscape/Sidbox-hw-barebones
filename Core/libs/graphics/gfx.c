#include <display/lcd.h>	// need a way to talk to the hardware ;)
#include "main.h"
#include <string.h>

#include "gfx.h"


// colour indexes

//

static uint8_t 		fpen = GFX_DEFAULT_FPEN;	// fore colour
static uint8_t 		bpen = GFX_DEFAULT_BPEN;	// back colour for things like fills (text background or filled primatives

static uint8_t 		cpen = GFX_DEFAULT_CPEN;	// current pen to use


void gfx_setfpen(uint8_t colourindex){ fpen = colourindex; cpen = fpen; }	// fpen is used mainly for text
void gfx_setbpen(uint8_t colourindex){ bpen = colourindex; }				// bpen is used for text, filled objects


// this is slightly unsafe, so make sure to set the action area
void gfx_cls(){	// clears the active draw buffer
	memset(gfx_drawbuffer->bitmap, 0x00, gfx_drawbuffer->memspacelen);
}

static inline uint32_t gfx_coord(int16_t x, int16_t y){
	return (uint32_t)x * (uint32_t)gfx_drawbuffer->height + (uint32_t)y;
}

static inline bool gfx_in_bounds(int16_t x, int16_t y){
    return (x >= 0 && y >= 0 && x < gfx_drawbuffer->width && y < gfx_drawbuffer->height);
}

void gfx_pixel(int16_t x, int16_t y){
	if (!gfx_in_bounds(x, y)) return;
	int index = gfx_coord(x, y);
	gfx_drawbuffer->bitmap[index] = cpen;
}

static void gfx_pixel_raw(int16_t x, int16_t y){
	int index = gfx_coord(x, y);
	gfx_drawbuffer->bitmap[index] = cpen;
}

static inline void gfx_hline(int16_t x0, int16_t y, int16_t x1){
    if (y < 0 || y >= gfx_drawbuffer->height) return;

    if (x0 > x1) { int16_t t = x0; x0 = x1; x1 = t; }
    if (x1 < 0 || x0 >= gfx_drawbuffer->width) return;
    if (x0 < 0) x0 = 0;
    if (x1 >= gfx_drawbuffer->width) x1 = gfx_drawbuffer->width- 1;

    for (int16_t x = x0; x <= x1; x++) gfx_pixel_raw(x, y);
}


static inline void gfx_vline(int16_t x, int16_t y0, int16_t y1){
    if (x < 0 || x >= gfx_drawbuffer->width) return;

    if (y0 > y1) { int16_t t = y0; y0 = y1; y1 = t; }
    if (y1 < 0 || y0 >= gfx_drawbuffer->height) return;
    if (y0 < 0) y0 = 0;
    if (y1 >= gfx_drawbuffer->height) y1 = gfx_drawbuffer->height - 1;

    for (int16_t y = y0; y <= y1; y++) gfx_pixel_raw(x, y);
}


////////////////// RECT PRIMATIVES ///////////////////////////////////////////////////////
void gfx_draw_rect(int16_t x, int16_t y, int16_t w, int16_t h) {
	if (w <= 0 || h <= 0) return;
	int16_t x1 = x + w - 1;
	int16_t y1 = y + h - 1;

	gfx_hline(x, y, x1);
	gfx_hline(x, y1, x1);
	gfx_vline(x, y, y1);
	gfx_vline(x1, y, y1);
}

void gfx_draw_rectf(int16_t x, int16_t y, int16_t w, int16_t h) {
	if (w <= 0 || h <= 0) return;
	int16_t x1 = x + w - 1;
	int16_t y1 = y + h - 1;

	for(int by = y; by < y1; by++)
		gfx_hline(x, by, x1);
}

void gfx_draw_rectfo(int16_t x, int16_t y, int16_t w, int16_t h) {
	uint8_t ocpen;
	if (w <= 0 || h <= 0) return;
	int16_t x1 = x + w - 1;
	int16_t y1 = y + h - 1;

	ocpen = cpen;

	// first fill in the area
	cpen = bpen;	// set the draw-pen to background colour first for colouring
	// fill interior only (bottom edge drawn by outline)
	for(int by = y; by < y1; by++)
		gfx_hline(x, by, x1);

	cpen = ocpen;	// set the draw-pen back to originally set before doing the background colour draw
	gfx_hline(x, y, x1);
	gfx_hline(x, y1, x1);
	gfx_vline(x, y, y1);
	gfx_vline(x1, y, y1);
}

///////////////// CIRCLES PRIMATIVES //////////////////////////////////////////////////////
static inline void gfx_circle_points(int16_t cx, int16_t cy, int16_t x, int16_t y) {
	gfx_pixel(cx + x, cy + y);
	gfx_pixel(cx - x, cy + y);
	gfx_pixel(cx + x, cy - y);
	gfx_pixel(cx - x, cy - y);

	gfx_pixel(cx + y, cy + x);
	gfx_pixel(cx - y, cy + x);
	gfx_pixel(cx + y, cy - x);
	gfx_pixel(cx - y, cy - x);
}

void gfx_draw_circle(int16_t cx, int16_t cy, int16_t r) {
	if (r < 0) return;
	if (r == 0) {
		gfx_pixel(cx, cy);	// just dot, no point in drawing the rest
		return;
	}

	int16_t x = 0;
	int16_t y = r;
	int16_t d = 1 - r;

	gfx_circle_points(cx, cy, x, y);

	while (x < y) {
		x++;
		if (d < 0) {
			d += (int16_t) (2 * x + 1);
		} else {
			y--;
			d += (int16_t) (2 * (x - y) + 1);
		}
		gfx_circle_points(cx, cy, x, y);
	}
}

void gfx_draw_circlef(int16_t cx, int16_t cy, int16_t r) {
	if (r < 0) return;
	if (r == 0) { gfx_pixel(cx, cy);
		return;
	}

	int16_t x = 0;
	int16_t y = r;
	int16_t d = 1 - r;

	// Fill with horizontal spans (fast-ish and simple)
	while (x <= y) {
		// These 4 spans cover the 8 octants
		gfx_hline(cx - y, cy + x, cx + y);
		gfx_hline(cx - y, cy - x, cx + y);
		gfx_hline(cx - x, cy + y, cx + x);
		gfx_hline(cx - x, cy - y, cx + x);

		x++;
		if (d < 0) {
			d += (int16_t) (2 * x + 1);
		} else {
			y--;
			d += (int16_t) (2 * (x - y) + 1);
		}
	}
}

void gfx_draw_circlefo(int16_t cx, int16_t cy, int16_t r) {
    uint8_t ocpen = cpen;
    cpen = bpen;
    gfx_draw_circlef(cx, cy, r);
    cpen = ocpen;
    gfx_draw_circle(cx, cy, r);
}

/////////// ELIPSES //////////////////
static inline void gfx_ellipse_points(int16_t cx, int16_t cy, int16_t x, int16_t y) {
	gfx_pixel(cx + x, cy + y);
	gfx_pixel(cx - x, cy + y);
	gfx_pixel(cx + x, cy - y);
	gfx_pixel(cx - x, cy - y);
}

void gfx_draw_ellipse(int16_t cx, int16_t cy, int16_t rx, int16_t ry) {
	if (rx < 0 || ry < 0) return;
	if (rx == 0 && ry == 0) {
		gfx_pixel(cx, cy);
		return;
	}
	if (rx == 0) {
		gfx_vline(cx, cy - ry, cy + ry);
		return;
	}
	if (ry == 0) {
		gfx_hline(cx - rx, cy, cx + rx);
		return;
	}

	// Use 32/64-bit to avoid overflow in decision params
	int32_t x = 0;
	int32_t y = ry;

	int64_t rx2 = (int64_t) rx * rx;
	int64_t ry2 = (int64_t) ry * ry;

	int64_t dx = 0;
	int64_t dy = 2 * rx2 * y;

	// Region 1
	int64_t p1 = ry2 - rx2 * ry + (rx2 / 4);
	while (dx < dy) {
		gfx_ellipse_points(cx, cy, (int16_t) x, (int16_t) y);
		x++;
		dx += 2 * ry2;
		if (p1 < 0) {
			p1 += dx + ry2;
		} else {
			y--;
			dy -= 2 * rx2;
			p1 += dx - dy + ry2;
		}
	}

	// Region 2
	// Use integer form (avoid floats): p2 = ry2*(x+0.5)^2 + rx2*(y-1)^2 - rx2*ry2
	// Multiply by 4 to keep it integer: P2 = 4*ry2*(2x+1)^2/4 + 4*rx2*(y-1)^2 - 4*rx2*ry2
	int64_t x2p1 = 2 * x + 1;                // (2x + 1)
	int64_t p2 = ry2 * x2p1 * x2p1 / 4 + rx2 * (y - 1) * (y - 1) - rx2 * ry2;

	while (y >= 0) {
		gfx_ellipse_points(cx, cy, (int16_t) x, (int16_t) y);
		y--;
		dy -= 2 * rx2;
		if (p2 > 0) {
			p2 += rx2 - dy;
		} else {
			x++;
			dx += 2 * ry2;
			p2 += dx - dy + rx2;
		}
	}
}

void gfx_draw_ellipsef(int16_t cx, int16_t cy, int16_t rx, int16_t ry) {
	if (rx < 0 || ry < 0)
		return;
	if (rx == 0 && ry == 0) {
		gfx_pixel(cx, cy);
		return;
	}
	if (rx == 0) {
		gfx_vline(cx, cy - ry, cy + ry);
		return;
	}
	if (ry == 0) {
		gfx_hline(cx - rx, cy, cx + rx);
		return;
	}

	int32_t x = 0;
	int32_t y = ry;

	int64_t rx2 = (int64_t) rx * rx;
	int64_t ry2 = (int64_t) ry * ry;

	int64_t dx = 0;
	int64_t dy = 2 * rx2 * y;

	// Region 1: fill scanlines across +/-x at y
	int64_t p1 = ry2 - rx2 * ry + (rx2 / 4);
	while (dx < dy) {
		gfx_hline(cx - (int16_t) x, cy + (int16_t) y, cx + (int16_t) x);
		gfx_hline(cx - (int16_t) x, cy - (int16_t) y, cx + (int16_t) x);

		x++;
		dx += 2 * ry2;
		if (p1 < 0) {
			p1 += dx + ry2;
		} else {
			y--;
			dy -= 2 * rx2;
			p1 += dx - dy + ry2;
		}
	}

	// Region 2
	int64_t x2p1 = 2 * x + 1;
	int64_t p2 = ry2 * x2p1 * x2p1 / 4 + rx2 * (y - 1) * (y - 1) - rx2 * ry2;

	while (y >= 0) {
		gfx_hline(cx - (int16_t) x, cy + (int16_t) y, cx + (int16_t) x);
		gfx_hline(cx - (int16_t) x, cy - (int16_t) y, cx + (int16_t) x);

		y--;
		dy -= 2 * rx2;
		if (p2 > 0) {
			p2 += rx2 - dy;
		} else {
			x++;
			dx += 2 * ry2;
			p2 += dx - dy + rx2;
		}
	}
}

void gfx_draw_ellipsefo(int16_t cx, int16_t cy, int16_t rx, int16_t ry) {
	uint8_t ocpen = cpen;
	cpen = bpen;
	gfx_draw_ellipsef(cx, cy, rx, ry);
	cpen = ocpen;
	gfx_draw_ellipse(cx, cy, rx, ry);
}

// line draw
void gfx_draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1) {
	int16_t dx = (x1 > x0) ? (x1 - x0) : (x0 - x1);
	int16_t sx = (x0 < x1) ? 1 : -1;
	int16_t dy = (y1 > y0) ? (y0 - y1) : (y1 - y0); // negative
	int16_t sy = (y0 < y1) ? 1 : -1;
	int16_t err = dx + dy; // dx - abs(dy)

	while (1) {
		gfx_pixel(x0, y0);
		if (x0 == x1 && y0 == y1)
			break;
		int16_t e2 = (int16_t) (2 * err);
		if (e2 >= dy) {
			err += dy;
			x0 += sx;
		}
		if (e2 <= dx) {
			err += dx;
			y0 += sy;
		}
	}
}


///////////////// POLYGON DRAW //////////////////////////

void gfx_draw_poly(const gfx_point_t *pts, int16_t n, bool closed) {
	if (!pts || n < 2) return;

	for (int16_t i = 0; i < n - 1; i++) {
		gfx_draw_line(pts[i].x, pts[i].y, pts[i + 1].x, pts[i + 1].y);
	}
	if (closed) {
		gfx_draw_line(pts[n - 1].x, pts[n - 1].y, pts[0].x, pts[0].y);
	}
}

static inline void gfx_isort_int16(int16_t *a, int16_t n) {
	for (int16_t i = 1; i < n; i++) {
		int16_t key = a[i];
		int16_t j = i - 1;
		while (j >= 0 && a[j] > key) {
			a[j + 1] = a[j];
			j--;
		}
		a[j + 1] = key;
	}
}

void gfx_draw_polyf(const gfx_point_t *pts, int16_t n) {
	if (!pts || n < 3)
		return;
	if (n > GFX_MAX_POLY_VERTS)
		return; // keep simple, no malloc

	// Find y bounds
	int16_t minY = pts[0].y, maxY = pts[0].y;
	for (int16_t i = 1; i < n; i++) {
		if (pts[i].y < minY)
			minY = pts[i].y;
		if (pts[i].y > maxY)
			maxY = pts[i].y;
	}
	// Clip y-range to screen (optional but saves work)
	if (maxY < 0 || minY >= LCD_HEIGHT)
		return;
	if (minY < 0)
		minY = 0;
	if (maxY >= LCD_HEIGHT)
		maxY = LCD_HEIGHT - 1;

	int16_t nodes[GFX_MAX_POLY_VERTS];

	for (int16_t y = minY; y <= maxY; y++) {
		int16_t numNodes = 0;

		// Build list of x-intersections with scanline y
		for (int16_t i = 0, j = n - 1; i < n; j = i++) {
			int16_t yi = pts[i].y;
			int16_t yj = pts[j].y;
			int16_t xi = pts[i].x;
			int16_t xj = pts[j].x;

			// Check if edge crosses the scanline (half-open rule avoids double-counting vertices)
			bool cond1 = (yi < y && yj >= y);
			bool cond2 = (yj < y && yi >= y);
			if ((cond1 || cond2) && numNodes < GFX_MAX_POLY_VERTS) {
				// Compute x intersection: xi + (y - yi) * (xj - xi) / (yj - yi)
				int32_t x = (int32_t) xi
						+ (int32_t) (y - yi) * (int32_t) (xj - xi)
								/ (int32_t) (yj - yi);
				nodes[numNodes++] = (int16_t) x;
			}
		}

		if (numNodes < 2)
			continue;

		gfx_isort_int16(nodes, numNodes);

		// Fill pairs
		for (int16_t k = 0; k < numNodes - 1; k += 2) {
			int16_t x0 = nodes[k];
			int16_t x1 = nodes[k + 1];
			gfx_hline(x0, y, x1);
		}
	}
}

/////////////// Primary Graphics Blitter (ish) ///////////////////////

// this is an RLE compressed image

void gfx_blit_rle(uint8_t *img, int16_t dx, int16_t dy, uint16_t src_w,	uint16_t src_h) {
	if (!img || !gfx_drawbuffer || !gfx_drawbuffer->bitmap)
		return;

	gfx_bitmap_t *dstbm = gfx_drawbuffer;
	uint8_t *dst = dstbm->bitmap;
	uint16_t stride = dstbm->stride;

	uint32_t total = (uint32_t) src_w * (uint32_t) src_h;
	uint32_t out = 0;

	uint16_t sx = 0;
	uint16_t sy = 0;

	while (out < total) {
		uint8_t count = *img++;
		uint8_t value = *img++;

		if (count == 0)
			break; // invalid or end marker

		while (count-- && out < total) {

			int16_t tx = dx + sx;
			int16_t ty = dy + sy;

			// FULL clipping check
			if (tx >= 0 && ty >= 0 && tx < dstbm->width && ty < dstbm->height) {
				uint32_t idx = (uint32_t) tx * stride + (uint32_t) ty;
				if (value)
					dst[idx] = value;	// has a transparent area
			}

			// advance source coords
			sx++;
			if (sx >= src_w) {
				sx = 0;
				sy++;
			}

			out++;
		}
	}
}

void gfx_blit_raw(uint8_t *src, int16_t dx, int16_t dy, uint16_t src_w,	uint16_t src_h) {
	if (!src || !gfx_drawbuffer || !gfx_drawbuffer->bitmap)
		return;

	gfx_bitmap_t *bm = gfx_drawbuffer;
	uint8_t *dst = bm->bitmap;
	uint16_t stride = bm->stride;

	int16_t x0 = dx, y0 = dy;
	int16_t x1 = dx + (int16_t) src_w - 1;
	int16_t y1 = dy + (int16_t) src_h - 1;

	if (x1 < 0 || y1 < 0 || x0 >= bm->width || y0 >= bm->height)
		return;

	if (x0 < 0)
		x0 = 0;
	if (y0 < 0)
		y0 = 0;
	if (x1 >= bm->width)
		x1 = bm->width - 1;
	if (y1 >= bm->height)
		y1 = bm->height - 1;

	for (int16_t y = y0; y <= y1; y++) {
		uint16_t sy = (uint16_t) (y - dy);
		uint32_t src_row = (uint32_t) sy * src_w;
		for (int16_t x = x0; x <= x1; x++) {
			uint16_t sx = (uint16_t) (x - dx);
			uint8_t p = src[src_row + sx];
			if (p) {
				dst[(uint32_t) x * stride + (uint32_t) y] = p;
			}
		}
	}
}




