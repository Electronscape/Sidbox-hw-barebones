/*
 * ============================================================
 *  watchdog.h — Crash Signaling & Bootloader Control
 * ============================================================
 *
 *  Purpose
 *  -------
 *  Defines magic values, memory locations, and attributes used
 *  to communicate system state across resets and into the
 *  bootloader.
 *
 *  This mechanism allows the running application to signal:
 *    - abnormal termination (hardware, memory, bus faults)
 *    - software-level crashes
 *    - intentional reboot requests
 *    - clean shutdowns
 *
 *  On reset, the bootloader inspects these magic values to
 *  determine whether to:
 *    - continue booting the main application
 *    - remain in the bootloader
 *    - present diagnostic or recovery behavior
 *
 *  Crash Information
 *  -----------------
 *  Specific RAM locations are reserved to store crash reason
 *  codes and auxiliary diagnostic data. These values persist
 *  across resets and are cleared or overwritten on a clean
 *  boot.
 *
 *  Memory Placement
 *  ----------------
 *  The ATTR_MAGICBOOT attribute places critical flags into a
 *  dedicated linker section that is guaranteed to survive a
 *  reset long enough for the bootloader to inspect them.
 *
 *  Usage Notes
 *  ----------
 *  - Values defined here form a contract between application
 *    firmware and the bootloader.
 *
 * ============================================================
 */


#ifndef WATCHDOG_H

#define MAGICBOOTCODE				0xB00710AD		// the boot loader sees this, and will load into the bootloader and not our main application (OS)

#define MAGICBOOT_CRASH_LEVEL1		0xFF5C7A54		// HARDWARE FAULT, HARD FAULT - whatever, it's not good!
#define MAGICBOOT_CRASH_LEVEL2		0xFF5C7A55		// MEMORY FAULT / COLLISIONS
#define MAGICBOOT_CRASH_LEVEL3		0xFF5C7A56		// BUS FAULT / COLLISIONS
#define MAGICBOOT_CRASH_LEVEL8		0xFF5C7AAA		// SOFTWARE FAULT - should show a different friendlier warning

#define MAGICBOOT_REBOOT_CLEAN		0xFFFFFFFF		// a nice clean boot


#define CRASHBOOT_MEM_INFO			0x2001FFC0		// store the error bytes
#define CRASHBOOT_MEM_LOCATION		0x2001FFFC		// store (we crashed, what type of crash)





#define ATTR_MAGICBOOT __attribute__((section(".magicflag"))) //the magic code of MAGICBOOTCODE lets the boot loader NOT to continue to the application and stay inside the bootloader

void setBootloadMagic(uint8_t magicon);
void instReboot();
void report_crash();

#endif
