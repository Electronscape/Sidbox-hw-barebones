#include <display/lcd.h>
#include "dma2d.h"


DMA2D_HandleTypeDef hdma2d;


/*
void MX_DMA2D_Init(void) {
	hdma2d.Instance = DMA2D;
	hdma2d.Init.Mode = DMA2D_M2M_BLEND;
	hdma2d.Init.ColorMode = DMA2D_OUTPUT_RGB565;
	hdma2d.Init.OutputOffset = 0;

	hdma2d.LayerCfg[1].InputOffset = 0;
	hdma2d.LayerCfg[1].InputColorMode = DMA2D_INPUT_L8;
	hdma2d.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
	hdma2d.LayerCfg[1].InputAlpha = 0x2f;
	hdma2d.LayerCfg[1].AlphaInverted = DMA2D_REGULAR_ALPHA;
	hdma2d.LayerCfg[1].RedBlueSwap = DMA2D_RB_REGULAR;
	hdma2d.LayerCfg[1].ChromaSubSampling = DMA2D_NO_CSS;
	if (HAL_DMA2D_Init(&hdma2d) != HAL_OK) {
		Error_Handler();
	}
	if (HAL_DMA2D_ConfigLayer(&hdma2d, 1) != HAL_OK) {
		Error_Handler();
	}

	// background
	hdma2d.LayerCfg[0].InputOffset = 0;
	hdma2d.LayerCfg[0].InputColorMode = DMA2D_INPUT_L8;
	hdma2d.LayerCfg[0].AlphaMode = DMA2D_REPLACE_ALPHA;
	hdma2d.LayerCfg[0].InputAlpha = 0x2f;
	hdma2d.LayerCfg[0].AlphaInverted = DMA2D_REGULAR_ALPHA;
	hdma2d.LayerCfg[0].RedBlueSwap = DMA2D_RB_REGULAR;
	hdma2d.LayerCfg[0].ChromaSubSampling = DMA2D_NO_CSS;
	if (HAL_DMA2D_ConfigLayer(&hdma2d, 0) != HAL_OK) {
		Error_Handler();
	}

	clut1.pCLUT = lcd_get_clut();
	clut1.CLUTColorMode = DMA2D_CCM_ARGB8888;
	clut1.Size = 255;

	HAL_DMA2D_EnableCLUT(&hdma2d, 1);
	HAL_DMA2D_PollForTransfer(&hdma2d, 100);
	HAL_DMA2D_CLUTLoad(&hdma2d, clut1, 1);
	HAL_DMA2D_PollForTransfer(&hdma2d, 100);

	HAL_DMA2D_EnableCLUT(&hdma2d, 0);
	HAL_DMA2D_PollForTransfer(&hdma2d, 100);
	HAL_DMA2D_CLUTLoad(&hdma2d, clut1, 0);
	HAL_DMA2D_PollForTransfer(&hdma2d, 100);
}
*/

void HAL_DMA2D_MspInit(DMA2D_HandleTypeDef *dma2dHandle) {
	if (dma2dHandle->Instance == DMA2D) {
		__HAL_RCC_DMA2D_CLK_ENABLE();
		HAL_NVIC_SetPriority(DMA2D_IRQn, IRQ_GFX);
		HAL_NVIC_EnableIRQ(DMA2D_IRQn);
	}
}

void HAL_DMA2D_MspDeInit(DMA2D_HandleTypeDef *dma2dHandle) {
	if (dma2dHandle->Instance == DMA2D) {
		__HAL_RCC_DMA2D_CLK_DISABLE();
		HAL_NVIC_DisableIRQ(DMA2D_IRQn);
	}
}




///////// custom Driver systems ////////////////



void DMA2D_DisplayInit(int fgwidth, int fgheight, int bgwidth, int bgheight, int flags){
	// call these to ensure the pointers are set and top left coords
	//APIShowfg(0,0);
	//APIShowbg(0,0);
	#if(DEVDEBUG)
	dbug("DisplayMODE Changed: FWidth:%lu FHeight:%lu, BWidth:%lu BHeight:%lu FLAGS:%08X\r\n",
			fgwidth, fgheight, bgwidth, bgheight, flags);

	if (((uintptr_t)DBBufferDraw & 31) == 0) {
		dbug("DBBufferDraw is aligned! Addr: %08lX\n", (uint32_t)DBBufferDraw);
	} else


	if (((uintptr_t)DBBufferActive & 31) == 0) {
		dbug("DBBufferActive is aligned! Addr: %08lX\n", (uint32_t)DBBufferActive);
	}
	#endif
	// 2. Abort any transfer just to be sure (safe fallback)
	HAL_DMA2D_Abort(&hdma2d);
	HAL_DMA2D_DeInit(&hdma2d);

	// Global Parts;
	hdma2d.Instance = DMA2D;
	if(flags & DISPFLAG_DUALLAYER)
		hdma2d.Init.Mode = DMA2D_M2M_BLEND;
	else
		hdma2d.Init.Mode = DMA2D_M2M_PFC;	// basic front only no blending

	hdma2d.Init.ColorMode = DMA2D_OUTPUT_RGB565;
	hdma2d.Init.OutputOffset = 0;

	if(flags & DISPFLAG_DUALLAYER){
		if(flags & DISPFLAG_SCROLLABLE) {
			lcd_update = lcd_update_2LayerScrollable;
			hdma2d.LayerCfg[1].InputOffset = fgheight - LCD_HEIGHT; //hdma2d.LayerCfg[1].InputOffset = fgwidth - 480;//0;//320 - ;
			hdma2d.LayerCfg[0].InputOffset = bgheight - LCD_HEIGHT; //hdma2d.LayerCfg[0].InputOffset = fgwidth - 480;//0;//320 - ;
		} else {
			lcd_update = lcd_update_2LayerBasic;
			hdma2d.LayerCfg[1].InputOffset = 0;
			hdma2d.LayerCfg[0].InputOffset = 0;
		}
		// foreground -----------------------------------------------------
		hdma2d.LayerCfg[1].InputColorMode = DMA2D_INPUT_L8;
		hdma2d.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
		hdma2d.LayerCfg[1].InputAlpha = 0x2f;
		hdma2d.LayerCfg[1].AlphaInverted = DMA2D_REGULAR_ALPHA;
		hdma2d.LayerCfg[1].RedBlueSwap = DMA2D_RB_REGULAR;
		hdma2d.LayerCfg[1].ChromaSubSampling = DMA2D_NO_CSS;
		if (HAL_DMA2D_Init(&hdma2d) != HAL_OK) {
			Error_Handler();
		}
		if (HAL_DMA2D_ConfigLayer(&hdma2d, 1) != HAL_OK) Error_Handler(); // handle config failure

		// background -----------------------------------------------------
		hdma2d.LayerCfg[0].InputColorMode = DMA2D_INPUT_L8;
		hdma2d.LayerCfg[0].AlphaMode = DMA2D_REPLACE_ALPHA;
		hdma2d.LayerCfg[0].InputAlpha = 0x2f;
		hdma2d.LayerCfg[0].AlphaInverted = DMA2D_REGULAR_ALPHA;
		hdma2d.LayerCfg[0].RedBlueSwap = DMA2D_RB_REGULAR;
		hdma2d.LayerCfg[0].ChromaSubSampling = DMA2D_NO_CSS;
		if (HAL_DMA2D_ConfigLayer(&hdma2d, 0) != HAL_OK) Error_Handler(); // handle config failure
	} else {
		if(flags & DISPFLAG_SCROLLABLE){
			lcd_update = lcd_update_1LayerScrollable;
			hdma2d.LayerCfg[1].InputOffset = bgheight - LCD_HEIGHT;	//hdma2d.LayerCfg[1].InputOffset = fgwidth - 480;//0;//320 - ;
			hdma2d.LayerCfg[0].InputOffset = bgheight - LCD_HEIGHT;	//hdma2d.LayerCfg[0].InputOffset = fgwidth - 480;//0;//320 - ;
		} else {
			lcd_update = lcd_update_1LayerBasic;
			hdma2d.LayerCfg[1].InputOffset = 0;		//hdma2d.LayerCfg[1].InputOffset = fgwidth - 480;//0;//320 - ;
			hdma2d.LayerCfg[0].InputOffset = 0;		//hdma2d.LayerCfg[0].InputOffset = fgwidth - 480;//0;//320 - ;
		}
		// no blending setup
		// foreground -----------------------------------------------------
		hdma2d.LayerCfg[1].InputColorMode = DMA2D_INPUT_L8;
		hdma2d.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
		hdma2d.LayerCfg[1].InputAlpha = 0x2f;
		hdma2d.LayerCfg[1].AlphaInverted = DMA2D_REGULAR_ALPHA;
		hdma2d.LayerCfg[1].RedBlueSwap = DMA2D_RB_REGULAR;
		hdma2d.LayerCfg[1].ChromaSubSampling = DMA2D_NO_CSS;
		if (HAL_DMA2D_Init(&hdma2d) != HAL_OK) {
			Error_Handler();
		}
		if (HAL_DMA2D_ConfigLayer(&hdma2d, 1) != HAL_OK) Error_Handler(); // handle config failure

		// background -----------------------------------------------------
		hdma2d.LayerCfg[0].InputColorMode = DMA2D_INPUT_L8;
		hdma2d.LayerCfg[0].AlphaMode = DMA2D_REPLACE_ALPHA;
		hdma2d.LayerCfg[0].InputAlpha = 0x2f;
		hdma2d.LayerCfg[0].AlphaInverted = DMA2D_REGULAR_ALPHA;
		hdma2d.LayerCfg[0].RedBlueSwap = DMA2D_RB_REGULAR;
		hdma2d.LayerCfg[0].ChromaSubSampling = DMA2D_NO_CSS;
		if (HAL_DMA2D_ConfigLayer(&hdma2d, 0) != HAL_OK) Error_Handler(); // handle config failure
	}

	clut1.pCLUT = lcd_get_clut();
	clut1.CLUTColorMode = DMA2D_CCM_ARGB8888;
	clut1.Size = 255;

	HAL_DMA2D_EnableCLUT(&hdma2d, 1);
	HAL_DMA2D_PollForTransfer(&hdma2d, 100);
	HAL_DMA2D_CLUTLoad(&hdma2d, clut1, 1);
	HAL_DMA2D_PollForTransfer(&hdma2d, 100);

	HAL_DMA2D_EnableCLUT(&hdma2d, 0);
	HAL_DMA2D_PollForTransfer(&hdma2d, 100);
	HAL_DMA2D_CLUTLoad(&hdma2d, clut1, 0);
	HAL_DMA2D_PollForTransfer(&hdma2d, 100);
}

























