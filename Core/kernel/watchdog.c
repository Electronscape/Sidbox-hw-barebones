/*
 * ============================================================
 *  watchdog.c — Reset Control & Crash Reporting
 * ============================================================
 *
 *  Purpose
 *  -------
 *  Implements the runtime logic for signaling reboot intent
 *  and crash conditions to the bootloader, then performing a
 *  controlled system reset.
 *
 *  This module is responsible for:
 *    - Writing boot intent flags to a persistent linker section
 *    - Recording crash state information in reserved RAM
 *    - Ensuring cache and memory state is coherent before reset
 *    - Forcing a clean, deterministic system restart
 *
 *  Bootloader Interaction
 *  ----------------------
 *  Prior to reset, a magic flag is written to a dedicated
 *  memory section that survives long enough for the bootloader
 *  to inspect it. Based on this value, the bootloader may:
 *    - Continue normal application startup
 *    - Enter recovery / bootloader mode
 *    - Present crash diagnostics or warnings
 *
 *  Crash Handling
 *  --------------
 *  When a fatal software or hardware condition is detected,
 *  this module records a crash reason code in predefined RAM
 *  locations before triggering a reset. These values allow
 *  post-reset diagnostics without relying on a live system.
 *
 *  Reset Safety
 *  ------------
 *  Interrupts are disabled and CPU caches are flushed or
 *  invalidated to guarantee that all diagnostic data reaches
 *  memory before the reset occurs. Synchronization barriers
 *  are used to enforce correct ordering.
 *
 *  Design Notes
 *  ------------
 *  - This file intentionally contains no recovery logic.
 *  - All decisions based on reboot or crash state are made
 *    by the bootloader.
 *
 * ============================================================
*/

#include "main.h"
#include "watchdog.h"



ATTR_MAGICBOOT static uint32_t reboot_magic_flag;	// tells system we want to be in bootloader

void setBootloadMagic(uint8_t magicon){
	if(!magicon)	// 0 (usually means good just software reboot request
		reboot_magic_flag = MAGICBOOT_REBOOT_CLEAN;	// no bootloader!
	else
		reboot_magic_flag = MAGICBOOTCODE;
}

void instReboot(){
	setBootloadMagic(0);
	__disable_irq();  // Disable interrupts

    SCB_CleanDCache();    // Flush data cache to memory
    SCB_InvalidateICache(); // Optional: Invalidate instruction cache

    SCB_DisableDCache(); // Optional: Disable D-Cache
    SCB_DisableICache(); // Optional: Disable I-Cache

    __DSB();  // Data Synchronization Barrier
    __ISB();  // Instruction Synchronization Barrier

    NVIC_SystemReset();  // Trigger reset
}

void report_crash(){
	uint32_t *hwinf = (uint32_t *)CRASHBOOT_MEM_INFO;	// store the error bytes
	uint32_t *hwcsh = (uint32_t *)CRASHBOOT_MEM_LOCATION;	// store the something shit happened flat

	*hwcsh = MAGICBOOT_CRASH_LEVEL8;	// FFS CRASH

	__disable_irq();  // Disable interrupts

	SCB_CleanDCache();    // Flush data cache to memory
	SCB_InvalidateICache(); // Optional: Invalidate instruction cache

	SCB_DisableDCache(); // Optional: Disable D-Cache
	SCB_DisableICache(); // Optional: Disable I-Cache

	__DSB();  // Data Synchronization Barrier
	__ISB();  // Instruction Synchronization Barrier

	NVIC_SystemReset();  // Trigger reset
}
