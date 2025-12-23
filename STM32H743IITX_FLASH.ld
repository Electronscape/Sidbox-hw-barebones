/*
******************************************************************************
**
**  File        : LinkerScript.ld
**
**  Author      : STM32CubeIDE
**
**  Abstract    : Linker script for STM32H7 series
**                2048Kbytes FLASH and 192Kbytes RAM
**
**                Set heap size, stack size and stack location according
**                to application requirements.
**
**                Set memory bank area and size if external memory is used.
**
**  Target      : STMicroelectronics STM32
**
**  Distribution: The file is distributed as is, without any warranty
**                of any kind.
**
*****************************************************************************
** @attention
**
** Copyright (c) 2019 STMicroelectronics.
** All rights reserved.
**
** This software component is licensed by ST under BSD 3-Clause license,
** the "License"; You may not use this file except in compliance with the
** License. You may obtain a copy of the License at:
**                        opensource.org/licenses/BSD-3-Clause
**
****************************************************************************
*/

/* Entry Point */
ENTRY(Reset_Handler)

/* Highest address of the user mode stack */
_estack = ORIGIN(RAM_MAIN) + LENGTH(RAM_MAIN);	/* end of "RAM_D1" Ram type memory */
/* Generate a link error if heap and stack don't fit into RAM */
_Min_Heap_Size = 0x100;     /* required amount of heap  */
_Min_Stack_Size = 0x400; 	/* required amount of stack */
_sstack = _estack - _Min_Stack_Size;  /* start of stack */

/* Specify the memory areas */
MEMORY
{
  FLASH    (rx)  : ORIGIN = 0x08020000, LENGTH = 2048K - 64K
  RAM_MAIN (xrw) : ORIGIN = 0x24000000, LENGTH = 512K - 16
  RAM_D2   (xrw) : ORIGIN = 0x30000000, LENGTH = 288K
  RAM_D3   (xrw) : ORIGIN = 0x38000000, LENGTH = 64K
  DTCMRAM  (xrw) : ORIGIN = 0x20000000, LENGTH = 128K
  ITCMRAM  (xrw) : ORIGIN = 0x00000000, LENGTH = 64K
  EXTRAM   (xrw) : ORIGIN = 0xD0000000, LENGTH = 6M			/* keep this at full memory width */
  EXTVRAM  (xrw) : ORIGIN = 0xD0600000, LENGTH = 1M			/* 1 MEG of VRAM for purely the display buffers */
  OS	   (xrw) : ORIGIN = 0xD0700000, LENGTH = 1M			/* our chip ram, for our OS */
  /* Add a separate 4-byte region for the magic flag */
  RAM_MAGIC (xrw) : ORIGIN = 0x2407FFF0, LENGTH = 16
}

/* Define output sections */
SECTIONS
{


	/* Fast RAMTC data section (DTCM) */
	.RAMTC_data (NOLOAD) :
	{
	    . = ALIGN(4);
	    *(.RAMTC_data)
	    . = ALIGN(4);
	} >DTCMRAM
	
	/* Fast RAMTC code section (DTCM or ITCM if available) */
	.RAMTC_code :
	{
	    . = ALIGN(4);
	    *(.RAMTC_code)
	    . = ALIGN(4);
	} >DTCMRAM

	.ICMRAM(NOLOAD) :
    {
        . = ALIGN(1);
    	_sdata = .;        /* create a global symbol at data start */
    	*(.ICMRAM)           /* .data sections */
    	*(.ICMRAM*)          /* .data* sections */
    	. = ALIGN(1);
    	_edata = .;        /* define a global symbol at data end */
    } >ITCMRAM
    

	.RAMTC(NOLOAD) :
	{
        . = ALIGN(1);
    	_sdata = .;        /* create a global symbol at data start */
    	*(.RAMTC)           /* .data sections */
    	*(.RAMTC*)          /* .data* sections */
    	//. = ALIGN(4);
     	//*(.code_in_ram)
    	. = ALIGN(1);
    	_edata = .;        /* define a global symbol at data end */
    } >DTCMRAM

	.CHIPRAM(NOLOAD) :
    {
        . = ALIGN(1);
    	_sdata = .;        /* create a global symbol at data start */
    	*(.CHIPRAM)           /* .data sections */
    	*(.CHIPRAM*)          /* .data* sections */
    	. = ALIGN(1);
    	_edata = .;        /* define a global symbol at data end */
    } >OS

	.FASTRAM(NOLOAD) :
    {
        . = ALIGN(1);
    	_sdata = .;        /* create a global symbol at data start */
    	*(.FASTRAM)           /* .data sections */
    	*(.FASTRAM*)          /* .data* sections */
    	. = ALIGN(1);
    	_edata = .;        /* define a global symbol at data end */
    } >EXTRAM
   
  	.CRTRAM(NOLOAD) :
    {
        . = ALIGN(1);
    	_sdata = .;        /* create a global symbol at data start */
    	*(.CRTRAM)           /* .data sections */
    	*(.CRTRAM*)          /* .data* sections */
    	. = ALIGN(1);
    	_edata = .;        /* define a global symbol at data end */
    } >RAM_D2
    
	.RAMDMA(NOLOAD) :
	{
        . = ALIGN(1);
    	_sdata = .;        /* create a global symbol at data start */
    	*(.RAMDMA)           /* .data sections */
    	*(.RAMDMA*)          /* .data* sections */
    	. = ALIGN(1);
    	_edata = .;        /* define a global symbol at data end */
    } >RAM_D3

  /* The startup code goes first into FLASH */
  .isr_vector :
  {
    . = ALIGN(4);
    KEEP(*(.isr_vector)) /* Startup code */
    . = ALIGN(4);
  } >FLASH

  /* The program code and other data goes into FLASH */
  .text :
  {
    . = ALIGN(4);
    *(.text)           /* .text sections (code) */
    *(.text*)          /* .text* sections (code) */
    *(.glue_7)         /* glue arm to thumb code */
    *(.glue_7t)        /* glue thumb to arm code */
    *(.eh_frame)

    KEEP (*(.init))
    KEEP (*(.fini))

    . = ALIGN(4);
    _etext = .;        /* define a global symbols at end of code */
  } >FLASH


	.extram(NOLOAD) :
    {
        . = ALIGN(4);
    	_sdata = .;        /* create a global symbol at data start */
    	*(.extram)           /* .data sections */
    	*(.extram*)          /* .data* sections */

    	. = ALIGN(1);
    	_edata = .;        /* define a global symbol at data end */
    } >EXTRAM
    
    
    .vram (NOLOAD) :
	{
	  . = ALIGN(32);
	  __vram_start__ = .;
	  *(.vram)
	  *(.vram*)
	  . = ALIGN(32);
	  __vram_end__ = .;
	} >EXTVRAM

  /* Constant data goes into FLASH */
  .rodata :
  {
    . = ALIGN(4);
    *(.rodata)         /* .rodata sections (constants, strings, etc.) */
    *(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
    . = ALIGN(4);
  } >FLASH

  .ARM.extab   : { *(.ARM.extab* .gnu.linkonce.armextab.*) } >FLASH
  .ARM : {
    __exidx_start = .;
    *(.ARM.exidx*)
    __exidx_end = .;
  } >FLASH
  
  /* Put this inside SECTIONS */
	.magicflag (NOLOAD) :
	{
	    KEEP(*(.magicflag))
	} >RAM_MAGIC

  .preinit_array     :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } >FLASH
  
  .init_array :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
  } >FLASH
  
  .fini_array :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array*))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } >FLASH

  /* used by the startup to initialize data */
  _sidata = LOADADDR(.data);

  /* Initialized data sections goes into RAM, load LMA copy after code */
  .data :
  {
    . = ALIGN(4);
    _sdata = .;        /* create a global symbol at data start */
    *(.data)           /* .data sections */
    *(.data*)          /* .data* sections */
    *(.RamFunc)        /* .RamFunc sections */
    *(.RamFunc*)       /* .RamFunc* sections */
    *(.code_in_ram)
    

    . = ALIGN(4);
    _edata = .;        /* define a global symbol at data end */
  } >RAM_MAIN AT> FLASH

  /* Uninitialized data section */
  . = ALIGN(4);
  .bss :
  {
    /* This is used by the startup in order to initialize the .bss secion */
    _sbss = .;         /* define a global symbol at bss start */
    __bss_start__ = _sbss;
    *(.bss)
    *(.bss*)
    *(COMMON)

    . = ALIGN(4);
    _ebss = .;         /* define a global symbol at bss end */
    __bss_end__ = _ebss;
  } >RAM_MAIN

  /* User_heap_stack section, used to check that there is enough RAM left */
  ._user_heap_stack :
  {
    . = ALIGN(8);
    PROVIDE ( end = . );
    PROVIDE ( _end = . );
    . = . + _Min_Heap_Size;
    . = . + _Min_Stack_Size;
    . = ALIGN(8);
  } >RAM_MAIN

  /* Remove information from the standard libraries */
  /DISCARD/ :
  {
    libc.a ( * )
    libm.a ( * )
    libgcc.a ( * )
  }

  .ARM.attributes 0 : { *(.ARM.attributes) }
}


