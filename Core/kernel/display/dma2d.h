#ifndef __DMA2D_H__
#define __DMA2D_H__


#include "main.h"
#include "api_gfx.h"

extern DMA2D_HandleTypeDef hdma2d;

#define APIDisplayMode			DMA2D_DisplayInit
#define DMA2D_DEFAULT_WIDTH		SF_SCREENWIDTH
#define DMA2D_DEFAULT_HEIGHT	SF_SCREEHEIGHT

void MX_DMA2D_Init(void);

// api version
void DMA2D_DisplayInit(int fgwidth, int fgheight, int bgwidth, int bgheight, int flags);


#endif /* __DMA2D_H__ */


