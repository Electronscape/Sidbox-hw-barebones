/*
 * filesys.h
 *
 *  Created on: Oct 02, 2021
 *      Author: neochrome
 */

// JUST INCLUDE THIS FILE HEAD to your files if you need access to the file system
// currently only 1 file can be opened at a time!

#ifndef FILESYS_H
#define FILESYS_H

#include "main.h"

#include "ff.h"
#include "ff_gen_drv.h"
#include "sd_diskio.h" /* defines SD_Driver as external */

// hardware pin sys
#define SDCARD_DETECTED			(!(GPIOC->IDR & 0x0800))	// Card detect pin			/C11

extern FATFS 		fs;
extern FIL 			fil;
extern FRESULT 		fres;

extern char SDPath[4]; /* SD logical drive path */
void MX_FATFS_Init(void);
void MX_FATFS_DeInit(void);

extern const char *DISKCODEs[];
const char* diskcode_str(FRESULT r);

void DiskMount();

void doDiskHeartBeat(void);
bool isDiskMounted();
bool isDiskPresent();


void FATFSCHECK();

#endif
