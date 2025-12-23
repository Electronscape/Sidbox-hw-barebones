#include "main.h"
#include <string.h>

#include "filesys.h"
#include "sdmmc.h"

#include "timers/timers.h"

// SYSTEM MONITORING
//static bool bDiskPresented = false;		// is the disk in the slot?
static bool bDiskMounted 		= false;	// track if the disk is mounted
static bool bDiskMountedLast 	= false;	// last known check
static bool bDiskError			= false;
static bool bDiskErrorL			= false;
static int  iDiskHeartBeat  	= 0;		// check every something ticks to see if disk status changed

const char *DISKCODEs[] = {
	"OK",                   // 0
	"DISK_ERR",             // 1
	"INT_ERR",              // 2
	"NOT_READY",            // 3
	"NO_FILE",              // 4
	"NO_PATH",              // 5
	"INVALID_NAME",         // 6
	"DENIED",               // 7
	"EXIST",                // 8
	"INVALID_OBJECT",       // 9
	"WRITE_PROTECTED",      // 10
	"INVALID_DRIVE",        // 11
	"NOT_ENABLED",          // 12
	"NO_FILESYSTEM",        // 13
	"MKFS_ABORTED",         // 14
	"TIMEOUT",              // 15
	"LOCKED",               // 16
	"NOT_ENOUGH_CORE",      // 17
	"TOO_MANY_OPEN_FILES",  // 18
	"INVALID_PARAMETER",    // 19

	"INVALIDFILENUMBER"     // 20 (custom API reliant)
};

const char* diskcode_str(FRESULT r) {
	static const char *unk = "FR_UNKNOWN";
	if ((unsigned) r < (sizeof(DISKCODEs) / sizeof(DISKCODEs[0])))
		return DISKCODEs[r];
	return unk;
}

char SDPath[4]; 	// SD logical drive path

void MX_FATFS_Init(void) {
	//retSD =
	FATFS_LinkDriver(&SD_Driver, SDPath);
}

void MX_FATFS_DeInit(void) {
	//retSD =
	FATFS_UnLinkDriver(SDPath);
}


FATFS fs;
FIL fil;
FRESULT fres = FR_NOT_READY;


bool isDiskMounted(){ return bDiskMounted; }
bool isDiskPresent(){ return !!SDCARD_DETECTED; }


void DiskMount(){
	HAL_StatusTypeDef	initRes;
	uint8_t strBuf[512];	// small amount need it just for the file peeking
	char *strRet;

	bDiskMounted = false;

	strcpy(SDPath, "0:/");	// mount the disk path
	MX_FATFS_Init();		// hardware init SPI and Restart the SD-protocols

	strRet = NULL;

	// default clock speed, 400hz is about a standard SDCARD baseline rate
	MODIFY_REG(SDMMC1->CLKCR, SDMMC_CLKCR_CLKDIV, 2 << SDMMC_CLKCR_CLKDIV_Pos);
	initRes = HAL_SD_Init(&hsd1);	//
	if (initRes != HAL_OK) {
		//sprintf(message, "SD Init failed!\r\n");
		strRet = lang_get(STR_SDINITFAILED);
		goto FailedMount;
	}

	initRes = (HAL_SD_InitCard(&hsd1));
	if (initRes != HAL_OK) {
		strRet = lang_get(STR_SDCARDFAILED);
		goto FailedMount;
	}

	// Manually bump speed after InitCard
	MODIFY_REG(SDMMC1->CLKCR, SDMMC_CLKCR_CLKDIV, 1 << SDMMC_CLKCR_CLKDIV_Pos);

	fres = f_mount(&fs, "0:/", 1);
	if (fres != FR_OK) {
		dbug("%s: %s\n", lang_get(STR_SDCARDMOUNT_FAILED), diskcode_str(fres));
		strRet = NULL;
		goto FailedMount;
	}

	if(fres != FR_OK){
		sprintf(strBuf, "* %s!\nCODE: %lu \n%s \n", lang_get(STR_SDCARDMOUNT_NOTCOMPLETE), fres, diskcode_str(fres));
		goto FailedMount;
	}

	dbug("%s\n", lang_get(STR_SDCARDMOUNT_SUCCESS));

	bDiskError = false;
	bDiskMounted = true;
	return;

FailedMount:	// ----------------------- failed --------------------
	if(strRet) dbug("%s\n", strRet);
	bDiskMounted = false;
	bDiskError = true;
}

void UnmountDisk(){
    FRESULT fres = f_mount(NULL, "0:/", 0);
    if (fres != FR_OK) {
        dbug("%s: %s\n", lang_get(STR_SDCARD_UNMOUNTFAIL), diskcode_str(fres));
    } else
    	dbug("%s: %s\n", lang_get(STR_SDCARD_UNMOUNTED), diskcode_str(fres));

    memset(&fil, 0, sizeof(fil));
    memset(&fs, 0, sizeof(fs));

    MX_FATFS_DeInit();

	if (HAL_SD_DeInit(&hsd1) != HAL_OK)
		dbug("%s\n", lang_get(STR_SDCARD_IO_FAIL));

	memset(&fs, 0, sizeof(fs));
	GPIOI->BSRR = 0x0100 << 16;
	dbug("%s\n", lang_get(STR_SDCARD_REMOVED));
	bDiskMounted = false;	// no disk in drive
}

void doDiskHeartBeat(void) {
	static uint8_t present_stable = 0;
	static bool mount_attempted = false;

	if (++iDiskHeartBeat <= 60) return;
	iDiskHeartBeat = 0;

	bool present = isDiskPresent();

	// debounce present a bit
	if (present) {
		if (present_stable < 2)
			present_stable++;
	} else
		present_stable = 0;

	if (!present) {
		// card removed: reset all retry gating
		if (bDiskMounted) UnmountDisk();
		bDiskError = false;
		mount_attempted = false;
		bDiskMountedLast = bDiskMounted;
		bDiskErrorL = bDiskError;
		return;
	}

	// Present but not stable yet: do nothing
	if (present_stable < 2) return;

	// Present + stable
	if (bDiskMounted) {
		// already mounted; nothing to do
		mount_attempted = false;
	} else {
		// not mounted
		if (!mount_attempted && !bDiskError) {
			mount_attempted = true;
			DiskMount();  // should set bDiskMounted or bDiskError
		}
	}

	bDiskMountedLast = bDiskMounted;
	bDiskErrorL = bDiskError;
}


void FATFSCHECK(){
	uint8_t strBuf[512];
	UINT bytesret;
	fres = f_open(&fil, "/autorun.inf", FA_READ | FA_OPEN_EXISTING); // init the card file system, cludgey, but works
	if(fres == FR_OK){
		f_read(&fil, strBuf, 512, &bytesret);
		if(bytesret){	// something was there, print it out
			if (bytesret >= sizeof(strBuf)) bytesret = sizeof(strBuf) - 1;
			strBuf[bytesret] = 0;

			dbug("%s\n\n", strBuf);
		}
		delayMs(10);
		f_close(&fil);
	}
}

