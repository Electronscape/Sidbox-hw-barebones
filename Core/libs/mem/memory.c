#include "main.h"


void SetMem(unsigned long addr, unsigned char value) {
	__IO unsigned char *dat;
	dat = SDRAM_BANK_ADDR + (long)addr;
	*dat = value;
}

unsigned char GetMem(unsigned long addr){
	__IO unsigned char *txt;
	txt = (char*) (SDRAM_BANK_ADDR + addr);
	return(*txt);
}

void SetMem16(unsigned long addr, unsigned short value) {
	__IO unsigned char *dat;
	dat = SDRAM_BANK_ADDR + (long)addr;
	*dat = value;
}

unsigned short GetMem16(unsigned long addr){
	__IO unsigned char *txt;
	txt = (char*) (SDRAM_BANK_ADDR + addr);
	return(*txt);
}

void show_ram(unsigned char *data, unsigned long len, unsigned long offset) {
	unsigned long i;
	unsigned long j;
	unsigned long k;
	//unsigned char step1;
	//int8   step2;

	dbug("\r\nShowing Block Length: %lx [%lu byte(s)]\r\n\r\n", len, len);

	j = 0L;

	for (i = 0; i < len; i++) {
		if ((i % 16L) == 0) {
			dbug("%08X |", offset + i);
		}

		dbug(" %02X", (unsigned char) data[i]);
		if ((i % 8) == 7L)
			dbug(" ");
		if (j >= 15) {
			dbug(" | ");
			for (k = (i - 15L); k <= i; k++) {
				if (!isalnum(data[k]))
					dbug(".");
				else
					dbug("%c", (unsigned char) data[k]);

				if ((k % 8L) == 7L)
					dbug(" ");
			}
			dbug("\r\n");
			j = 0L;

		} else {
			j++;
		}
	}
	dbug("\n");
}

void RAM_ClearEXTRam(uint32_t start, uint32_t length){	// this is the area 1 meg before the OS area at the end of the RAM
	long addr, endAddr;
	unsigned long *pram = (uint32_t*)(SDRAM_BANK_ADDR);
	pram += start;

	endAddr = length / 4;
	if(endAddr>(RAM_USERRAM_LENGTH/4)) endAddr = (RAM_USERRAM_LENGTH/4);

	for(addr=0; addr<endAddr; addr++){
		*pram++ = 0x00000000;
	}
}
