#include "main.h"

#include "audio.h"

#include "timers/tim.h"

// DMA Audio output system ---------------------------------------------------
ATTR_32ALIGN uint16_t *DMAAudioBufferR, *DMAAudioBufferL;					// buildup
ATTR_DMA_RAM uint16_t dma_audio_out_r[DEF_DMA_SOUND_BUFFER_FULL_LENGTH];	// the DMA to DAC hardware audio buffer
ATTR_DMA_RAM uint16_t dma_audio_out_l[DEF_DMA_SOUND_BUFFER_FULL_LENGTH];	// the DMA to DAC hardware audio buffer
static uint32_t lAudioBufferLength	= 0;	// audio buffer current length // since our buffer MIGHT change sizes for different players
static uint32_t lAudioBufferOffset  = 0;	// the audio offset buffer
static uint32_t lAudioBufferHalf	= 0;	// the set half buffer length


// ADC system ----------------------------------------------------------------
static ADC_ChannelConfTypeDef hadc3_ChanConf;	// configuration structure for the ADCs
static int SMPin1, SMPin2;						// the sample buffer



// 12-bit DAC range: 0..4095
static void AudioGenerateSineWave(int16_t freq, uint16_t *buffer) {
	// If you haven't set gAudioSampleRate yet, pick a sane default
	uint32_t fs = (DEF_MIX_FREQUENCY != 0) ? DEF_MIX_FREQUENCY : 44100u;

	// phase step in radians/sample
	float phase = 0.0f;
	float step = 2.0f * 3.14159265358979323846f * (float) freq / (float) fs;

	for (uint32_t i = 0; i < lAudioBufferLength; i++) {
		float s = sinf(phase);                 // -1..+1
		int32_t v = (int32_t) DAC_MID + (int32_t) (s * (float) DAC_AMP);

		// clamp just in case
		if (v < 0)
			v = 0;
		if (v > (int32_t) DAC_MAX)
			v = DAC_MAX;

		buffer[i] = (uint16_t) v;

		phase += step;
		if (phase >= 2.0f * 3.14159265358979323846f)
			phase -= 2.0f * 3.14159265358979323846f;
	}

	uint32_t bytes = lAudioBufferLength * sizeof(buffer[0]);
	SCB_CleanDCache_by_Addr((uint32_t*)buffer, bytes);
}


void HAL_DAC_ConvHalfCpltCallbackCh1(DAC_HandleTypeDef *hdac) {	// DMA HALF WAY THROUGH
	UNUSED(hdac);

	lAudioBufferOffset = 0;	// push the rendering buffer at the start, since it continues to play from half way point
}

void HAL_DAC_ConvCpltCallbackCh1(DAC_HandleTypeDef *hdac) {		// DMA COMPLETED WHOLE WAY THROUGH
	UNUSED(hdac);

	lAudioBufferOffset = lAudioBufferHalf;	// set the buffer location at the halfway point while the dma plays at the beginning
}

static void clearAudioBuffers(){
    uint16_t center = DAC_CENTER_DC;

    for (uint32_t i = 0; i < lAudioBufferLength; i++) {
        dma_audio_out_r[i] = center;
        dma_audio_out_l[i] = center;
    }

    uint32_t bytes = lAudioBufferLength * sizeof(dma_audio_out_r[0]);
    SCB_CleanDCache_by_Addr((uint32_t*)dma_audio_out_r, (int32_t)bytes);
    SCB_CleanDCache_by_Addr((uint32_t*)dma_audio_out_l, (int32_t)bytes);
}

void StopPlayerDMA() {
	clearAudioBuffers();

    // Stop DMA but keep DAC enabled
    HAL_DAC_Stop_DMA(&hdac1, DAC1_CHANNEL_1);
    HAL_DAC_Stop_DMA(&hdac1, DAC1_CHANNEL_2);

    // Set DAC output to 0x800
	DAC1->DHR12R1 = DAC_CENTER_DC;  // Channel 1
	DAC1->DHR12R2 = DAC_CENTER_DC;  // Channel 2

	lAudioBufferOffset = 0;
}

void StartPlayerDMA() {
	lAudioBufferHalf = (lAudioBufferLength >> 1);	// the half size position

	HAL_DAC_Start_DMA(&hdac1, DAC1_CHANNEL_1, (uint32_t *)dma_audio_out_l, lAudioBufferLength, DAC_ALIGN_12B_R);
	HAL_DAC_Start_DMA(&hdac1, DAC1_CHANNEL_2, (uint32_t *)dma_audio_out_r, lAudioBufferLength, DAC_ALIGN_12B_R);

	lAudioBufferOffset = 0;
}


void setDMAAudioFreq(uint32_t freq) {
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	hTmrAudioDMARate.Instance = TIM6;
	hTmrAudioDMARate.Init.Prescaler = 0;
	hTmrAudioDMARate.Init.CounterMode = TIM_COUNTERMODE_UP;
	// dont edit this, the math works on this CPU speed
	hTmrAudioDMARate.Init.Period = (225000000.0f) / (float) freq;;	// Buss speed , normal half the CORE clock speed
	hTmrAudioDMARate.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	HAL_TIM_Base_Init(&hTmrAudioDMARate);
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	HAL_TIMEx_MasterConfigSynchronization(&hTmrAudioDMARate, &sMasterConfig);
}

void startAudioDriver(){
	dbug("%s..\n", lang_get(STR_AUDIO_HARDWARE_START));
	lAudioBufferLength = DEF_DMA_SOUND_BUFFER_FULL_LENGTH;
	lAudioBufferHalf   = DEF_DMA_SOUND_BUFFER_HALF_LENGTH;	// the half size position
	lAudioBufferOffset = 0;

	// TEST AUDIO
	clearAudioBuffers();	// clear the audio

	AudioGenerateSineWave(441, dma_audio_out_l);
	AudioGenerateSineWave(883, dma_audio_out_r);
	//MX_TIM6_Init();	// <---- the following code below
	TIM_MasterConfigTypeDef sMasterConfig = { 0 };
	hTmrAudioDMARate.Instance = TIM6;
	hTmrAudioDMARate.Init.Prescaler = 0;
	hTmrAudioDMARate.Init.CounterMode = TIM_COUNTERMODE_UP;
	hTmrAudioDMARate.Init.Period = 2750;	/// hard code 44100hz (ish)
	hTmrAudioDMARate.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&hTmrAudioDMARate) != HAL_OK) {
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&hTmrAudioDMARate, &sMasterConfig) != HAL_OK) {
		Error_Handler();
	}



	int res = HAL_TIM_Base_Start(&hTmrAudioDMARate);
	if(res) dbug("%s\n", lang_get(STR_AUDIO_TMR_START_FAIL));
	else    dbug("%s\n", lang_get(STR_AUDIO_TMR_START_OK));





	setDMAAudioFreq(DEF_MIX_FREQUENCY);
}





void setSamplerFreq(long freq) {
	//TIM_ClockConfigTypeDef sClockSourceConfig = { 0 };
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	hTmrAudioSampler.Instance = TIM2;
	hTmrAudioSampler.Init.Prescaler = 0;
	hTmrAudioSampler.Init.CounterMode = TIM_COUNTERMODE_UP;
	hTmrAudioSampler.Init.Period = (225000000.0f) / (float) freq;;	// Buss speed , normal half the CORE clock speed
	hTmrAudioSampler.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	HAL_TIM_Base_Init(&hTmrAudioSampler);
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	HAL_TIMEx_MasterConfigSynchronization(&hTmrAudioSampler, &sMasterConfig);
}

void StartADCSmapler(int recordingFrequency) {
	// make sure the audio play back DMA is NOT running before starting this
	StopPlayerDMA();
	HAL_DAC_Start(&hdac1, DAC1_CHANNEL_1);
	HAL_DAC_Start(&hdac1, DAC1_CHANNEL_2);
	if (HAL_ADCEx_Calibration_Start(&hadc3, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED) != HAL_OK) {
		dbug("Engineer: ADC not ok!\n");

	} else {
		dbug("Engineer: ADC CALIBATION good\n");

	}

	if (HAL_ADC_Start(&hadc3) != HAL_OK) {
		dbug("Engineer: ! ADC failed to start!\n");
	}

	setSamplerFreq(recordingFrequency);

	// sampler interupt, doesnt work with DMA without interference with lcd
	// which calls processSampler
	// this is found -> HAL_TIM_PeriodElapsedCallback()
	HAL_TIM_Base_Start_IT(&hTmrAudioSampler);
}


void StopADCSmapler(){
	HAL_DAC_Stop(&hdac1, DAC1_CHANNEL_1);
	HAL_DAC_Stop(&hdac1, DAC1_CHANNEL_2);
	HAL_TIM_Base_Stop_IT(&hTmrAudioSampler);			// Stop the Timer2
}

void processSampler(){	// requesty of recording is controller by the Timer
	SCB_CleanInvalidateDCache(); // for this to work in DMA, you must have the audio buffer in TCM memory and NOT volatile! otherwise it just hangs

	HAL_ADC_Start(&hadc3);
	HAL_ADC_PollForConversion(&hadc3, 10);
	SMPin1 = (unsigned char) (HAL_ADC_GetValue(&hadc3) >> 8) ;

	HAL_ADC_PollForConversion(&hadc3, 10);
	SMPin2 = (unsigned char) (HAL_ADC_GetValue(&hadc3) >> 8) ;

	// just send it back to the DAC right away
	HAL_DAC_SetValue(&hdac1, DAC1_CHANNEL_1, DAC_ALIGN_12B_R, SMPin2<<4);
	HAL_DAC_SetValue(&hdac1, DAC1_CHANNEL_2, DAC_ALIGN_12B_R, SMPin1<<4);
}























