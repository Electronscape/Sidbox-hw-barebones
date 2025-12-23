#include <display/lcd.h>
#include "dma2d.h"

DMA2D_HandleTypeDef hdma2d;

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
