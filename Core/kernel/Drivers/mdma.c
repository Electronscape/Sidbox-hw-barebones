#include "mdma.h"

MDMA_HandleTypeDef hmdma_mdma_channel40_sw_0;

void MX_MDMA_Init(void) {

	/* MDMA controller clock enable */
	__HAL_RCC_MDMA_CLK_ENABLE();
	/* Local variables */

	/* Configure MDMA channel MDMA_Channel0 */
	/* Configure MDMA request hmdma_mdma_channel40_sw_0 on MDMA_Channel0 */
	hmdma_mdma_channel40_sw_0.Instance = MDMA_Channel0;
	hmdma_mdma_channel40_sw_0.Init.Request = MDMA_REQUEST_SW;
	hmdma_mdma_channel40_sw_0.Init.TransferTriggerMode = MDMA_BUFFER_TRANSFER;
	hmdma_mdma_channel40_sw_0.Init.Priority = MDMA_PRIORITY_LOW;
	hmdma_mdma_channel40_sw_0.Init.Endianness = MDMA_LITTLE_ENDIANNESS_PRESERVE;
	hmdma_mdma_channel40_sw_0.Init.SourceInc = MDMA_SRC_INC_BYTE;
	hmdma_mdma_channel40_sw_0.Init.DestinationInc = MDMA_DEST_INC_BYTE;
	hmdma_mdma_channel40_sw_0.Init.SourceDataSize = MDMA_SRC_DATASIZE_BYTE;
	hmdma_mdma_channel40_sw_0.Init.DestDataSize = MDMA_DEST_DATASIZE_BYTE;
	hmdma_mdma_channel40_sw_0.Init.DataAlignment = MDMA_DATAALIGN_PACKENABLE;
	hmdma_mdma_channel40_sw_0.Init.BufferTransferLength = 480;
	hmdma_mdma_channel40_sw_0.Init.SourceBurst = MDMA_SOURCE_BURST_SINGLE;
	hmdma_mdma_channel40_sw_0.Init.DestBurst = MDMA_DEST_BURST_SINGLE;
	hmdma_mdma_channel40_sw_0.Init.SourceBlockAddressOffset = 960 - 480;
	hmdma_mdma_channel40_sw_0.Init.DestBlockAddressOffset = 0;
	if (HAL_MDMA_Init(&hmdma_mdma_channel40_sw_0) != HAL_OK) {
		Error_Handler();
	}
}
