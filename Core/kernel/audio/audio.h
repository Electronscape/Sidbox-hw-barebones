#ifndef AUDIODRV_H
#define AUDIODRV_H



#define DAC_MAX     4095u
#define DAC_MID     2048u
#define DAC_AMP     2047u   // peak amplitude without clipping


#define ATTR_32ALIGN	volatile __attribute__((aligned(32)))

#define ATTR_DMA_RAM	__attribute__((aligned(32), section(".RAMDMA")))	// RAM D3   : AUDIO BUFFER
#define ATTR_RAM_TC		__attribute__((aligned(32), section(".RAMTC")))		// RAM DTCM :

#define 	DEF_DMA_SOUND_BUFFER_HALF_LENGTH	(4096)	// half the size of the full buffer memory

//	this default setting for the memory used for audio FULL LENGTH - usefull memory width
//  double this, since the full dma triggers a half point interrupt and a full one! (double buffering!)
#define		DEF_DMA_SOUND_BUFFER_FULL_LENGTH	(DEF_DMA_SOUND_BUFFER_HALF_LENGTH * 2)
#define 	DEF_MIX_FREQUENCY 					44100

#define		DAC_SYSTEM_OFF						0x000	// 0 - no voltage out (maybe for power saving?
#define		DAC_CENTER_DC						0x800	// 2048 - about 1.7volts on the dac


// general sample rate timer
extern TIM_HandleTypeDef hTmrAudioSampler;		// audio sample rate timer (roughly set to 44.1khz on boot) - can be used for sampling or playback

// DMA AUDIO GLOBALS //
extern DAC_HandleTypeDef hdac1;			// digital to audio output converter module
extern DMA_HandleTypeDef hdma_dac1_ch1;	// left channel
extern DMA_HandleTypeDef hdma_dac1_ch2;	// right channel
extern TIM_HandleTypeDef hTmrAudioDMARate;			// the DMA clock timer, deals with the Sampling rate through DMA

// INTERRUPT BASED AUDIO CAPTURE //
extern ADC_HandleTypeDef hadc3;		// audio capture moule



void processSampler();
void setDMAAudioFreq(uint32_t freq);
void startAudioDriver();

void StopPlayerDMA();
void StartPlayerDMA();











#endif
