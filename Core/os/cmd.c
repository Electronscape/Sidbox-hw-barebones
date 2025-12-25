#include "main.h"
#include "cmd.h"

#include "timers/timers.h"
#include "filesys.h"
#include <string.h>




const command_entry_t commands[];			// delcaration of this variable
static uint8_t cmd_string[CMD_BUF_SIZE];	// cos things need to change.

#define MAX_FILES 512

typedef struct {
    char name[128];
    uint32_t size;
    uint8_t attrib;
} FileEntry;

FileEntry entries[MAX_FILES];
DIR dir;
FILINFO finfo;



////////// command line processing //////////////////////////////////

int parse_args(char *input, char *argv[], int max_args) {
	int argc = 0;
	char *p = input;
	while (*p && argc < max_args) {
		// Skip leading spaces
		while (*p == ' ') p++;
		if (*p == '\0') break;

		if (*p == '"') {
			p++;
			argv[argc++] = p;
			while (*p && *p != '"') p++;
			if (*p == '"') *p++ = '\0';
		} else {
			argv[argc++] = p;
			while (*p && *p != ' ') p++;
			if (*p) *p++ = '\0';
		}
	}
	return argc;
}

static void cmd_normalize(char *s) {
	// Strip CR/LF
	for (char *p = s; *p; ++p) {
		if (*p == '\r' || *p == '\n') {
			*p = '\0';
			break;
		}
	}

	// Trim leading spaces (in-place by shifting)
	while (*s == ' ') s++;
	// If we advanced, shift left
	// (optionalinstead return the pointer, but keeping it simple)
}

/* 2) Tokenize (your existing parse_args, just returns argc) */
static int cmd_tokenize(char *input, char *argv[], int max_args) {
	int argc = 0;
	char *p = input;

	while (*p && argc < max_args) {
		while (*p == ' ') p++;
		if (*p == '\0') break;

		if (*p == '"') {
			p++;
			argv[argc++] = p;
			while (*p && *p != '"') p++;
			if (*p == '"') *p++ = '\0';
		} else {
			argv[argc++] = p;
			while (*p && *p != ' ') p++;
			if (*p) *p++ = '\0';
		}
	}
	return argc;
}

static void CMDUnknownCommand(int argc, char *argv[]);

static int cmd_dispatch(int argc, char *argv[]) {
	if (argc <= 0) return -1;
	const char *cmd = argv[0];
	for (int i = 0; commands[i].name != NULL; ++i) {
		if (strcmp(cmd, (const char*) commands[i].name) == 0) {
			commands[i].func(argc, argv);
			return 0;
		}
	}
	CMDUnknownCommand(argc, argv);
	return -1;
}


void cmd_entry(uint8_t *linein) {
	// Copy safely & ensure termination
	strncpy(cmd_string, (const char*)linein, CMD_BUF_SIZE - 1);
	cmd_string[CMD_BUF_SIZE - 1] = '\0';
	cmd_normalize(cmd_string);
	//dbug("TERM: %s\n", cmd_string);
	char *argv[MAX_ARGS];
	int argc = cmd_tokenize(cmd_string, argv, MAX_ARGS);

	if (argc == 0) {
		dbug("no command entered\r\n");
		flushCmd();
		return;
	}

	cmd_dispatch(argc, argv);
	flushCmd();
}

////////////// our commands //////////////////////////////////

static void CMDInfoOS(int argc, char *argv[]) {
	dbug("-> demo command response\n");
	UNUSED(argc);
	UNUSED(argv);
}

static void CMDUnknownCommand(int argc, char *argv[]) {
	dbug("Unknown command: ");
	dbug("\"%s\"\n",argv[0]);
	UNUSED(argc);
}

static void CMDHelloworld(int argc, char *argv[]) {
	dbug("-> Hello world!\n");
	UNUSED(argc);
	UNUSED(argv);
}

static void CMDParamsTest(int argc, char *argv[]) {
	if (argc < 2) {
		dbug("Usage: stest param1 param2 param3 ... up to 7 params\n");
		return;
	}
    dbug("CMD: %d parameter(s)\n", argc - 1);

    for (int i = 1; i < argc; i++) {
        dbug("  param[%d] = \"%s\"\n", i - 1, argv[i]);
    }
}

static void CMDBrightness(int argc, char *argv[]){
	if (argc < 2) {
		dbug("Usage: bright [0..100]  in percent\n");
		return;
	}
	int brtn;
	int newbrt;

	brtn = atoi(argv[1]);
	if(brtn<0 ) brtn = 0;
	if(brtn>99) brtn = 100;
	newbrt = (((float)brtn / (float)100.0f) * 255);


	dbug("%s: %lu%%\n", lang_get(STR_NEW_LCD_BRIGHTLEVEL), brtn);
	lcd_setbrightness(newbrt);
}

// standard commands for files
static void CMDCatFile(int argc, char *argv[]) {
	FRESULT fres;
	UINT bytesret;
	uint8_t strBuffer[512];	// only allow for 512 bytes
	if (argc < 2) {
		dbug("Usage: cat <filename>\n");
		return;
	}
	if(isDiskMounted()){

		fres = f_open(&fil, argv[1], FA_READ | FA_OPEN_EXISTING);
		if(fres == FR_OK){
			f_read(&fil, strBuffer, 512, &bytesret);
			if(bytesret){	// something was there, print it out
				if (bytesret >= sizeof(strBuffer)) bytesret = sizeof(strBuffer) - 1;
				strBuffer[bytesret] = 0;

				dbug("%s\n\n", strBuffer);
			}
			delayMs(10);
			f_close(&fil);
		} else
			dbug("%s \"%s\" (%s)\n", lang_get(STR_FS_FILE_NOTEXISTS), argv[1], diskcode_str(fres));


	} else
		dbug("%s.\n", lang_get(STR_FS_DISKNOTREADY));
}

static void CMDRebootTest(int argc, char *argv[]) {
	if (argc < 2) {
		dbug("Usage: <any>\n");
		return;
	}
	instReboot();
	UNUSED(argc);
	UNUSED(argv);
}



int compare_entries(const void *a, const void *b) {
    const FileEntry *fa = (const FileEntry *)a;
    const FileEntry *fb = (const FileEntry *)b;

    int isDirA = (fa->attrib & AM_DIR) != 0;
    int isDirB = (fb->attrib & AM_DIR) != 0;

    if (isDirA != isDirB)
        return isDirB - isDirA;

    return strcasecmp(fa->name, fb->name);
}

static void print_size_k(uint32_t bytes) {
    uint32_t kb = (bytes + 1023) / 1024;
    dbug("%6ldk ", kb);
}

void CMDDirList(int argc, char *argv[]) {
	char res;
	char cmdline[_MAX_LFN];
	char accept;
	if (argc < 2) {
		dbug("Usage \".\", \"/\", \"subdir\", \"/subdir/\", \"subdir/\" \n");
		return;
	}

    // Accept: ".", "/", "subdir", "/subdir/", "subdir/" etc.
    const char *arg = argv[1];

    if (strcmp(arg, ".") == 0) {
        // current working directory
        f_getcwd(cmdline, _MAX_LFN);
    } else if (arg[0] == '/') {
        // absolute path
        strncpy(cmdline, arg, _MAX_LFN - 1);
        cmdline[_MAX_LFN - 1] = '\0';
    } else {
        // relative path: CWD + "/" + arg
        char cwd[_MAX_LFN];
        f_getcwd(cwd, _MAX_LFN);

        // start with cwd
        strncpy(cmdline, cwd, _MAX_LFN - 1);
        cmdline[_MAX_LFN - 1] = '\0';

        // add slash if needed (and if there's room)
        size_t len = strlen(cmdline);
        if (len > 0 && cmdline[len - 1] != '/' && len < (_MAX_LFN - 1)) {
            cmdline[len++] = '/';
            cmdline[len] = '\0';
        }

        // append arg
        strncat(cmdline, arg, _MAX_LFN - 1 - strlen(cmdline));
    }

    // Trim trailing slashes (except if path is just "/")
    size_t n = strlen(cmdline);
    while (n > 1 && cmdline[n - 1] == '/') {
        cmdline[--n] = '\0';
    }




	if (f_opendir(&dir, cmdline) != FR_OK) {
		dbug("Failed to open directory!\r\n");
		return;
	}

	//memset(entries, 0x00, sizeof(entries) * MAX_FILES);
    int count = 0, fileCount = 0, dirCount = 0;
    uint32_t totalSize = 0;


    while (count < MAX_FILES) {
        memset(&finfo, 0, sizeof(FILINFO));
        if (f_readdir(&dir, &finfo) != 0 || finfo.fname[0] == 0)
            break;

        if (strcmp(finfo.fname, ".") == 0 || strcmp(finfo.fname, "..") == 0)
            continue;

        strncpy(entries[count].name, finfo.fname, sizeof(entries[count].name) - 1);
        entries[count].name[sizeof(entries[count].name)-1] = '\0';  // SAFETY
        entries[count].size = finfo.fsize;
        entries[count].attrib = finfo.fattrib;
        count++;
    }
    f_closedir(&dir);



    qsort(entries, count, sizeof(FileEntry), compare_entries);


    dbug("**DIR_LIST**\r\n");
    dbug("path: %s\r\n", cmdline);

    // directories first
    for (int i = 0; i < count; i++) {
		if (entries[i].attrib & AM_DIR) {
			dbug("    DIR [%s]\r\n", entries[i].name);
			dirCount++;
		}
	}
    delayMs(1);

    // files next
	for (int i = 0; i < count; i++) {
        if (!(entries[i].attrib & AM_DIR)) {
            print_size_k(entries[i].size);
            //dbug("%s\r\n", entries[i].name);
            dbug("%-32s\r\n", entries[i].name);  // Aligned columns
            fileCount++;
            totalSize += entries[i].size;
        }
    }
    dbug("----------------------------\r\n");
    dbug("%d File(s)\r\n", fileCount);
    dbug("%d Dir(s)\r\n", dirCount);
    dbug("Size %luk\r\n", (totalSize + 1023) / 1024);
    dbug("DIR_OK:>\r\n");
}

void CMDOSChangeDir(int argc, char *argv[]) {
	if (argc < 2) {
		dbug("Usage: cd <path>\n");
		return;
	}

	char path[_MAX_LFN];

	// Copy argument safely
	strncpy(path, argv[1], _MAX_LFN - 1);
	path[_MAX_LFN - 1] = '\0';

	// ---- strip surrounding quotes if present ----
	size_t len = strlen(path);
	if (len >= 2 && path[0] == '"' && path[len - 1] == '"') {
		memmove(path, path + 1, len - 2);
		path[len - 2] = '\0';
	}

	// ---- build final path ----
	char cmdline[_MAX_LFN];

	if (strcmp(path, ".") == 0) {
		f_getcwd(cmdline, _MAX_LFN);
	} else if (path[0] == '/') {
		// absolute path
		strncpy(cmdline, path, _MAX_LFN - 1);
		cmdline[_MAX_LFN - 1] = '\0';
	} else {
		// relative path
		char cwd[_MAX_LFN];
		f_getcwd(cwd, _MAX_LFN);

		strncpy(cmdline, cwd, _MAX_LFN - 1);
		cmdline[_MAX_LFN - 1] = '\0';

		size_t clen = strlen(cmdline);
		if (clen > 0 && cmdline[clen - 1] != '/' && clen < (_MAX_LFN - 1)) {
			cmdline[clen++] = '/';
			cmdline[clen] = '\0';
		}

		strncat(cmdline, path, _MAX_LFN - 1 - strlen(cmdline));
	}

	// Trim trailing slashes (except "/")
	size_t n = strlen(cmdline);
	while (n > 1 && cmdline[n - 1] == '/') {
		cmdline[--n] = '\0';
	}

	// ---- change directory ----
	FRESULT fr = f_chdir(cmdline);
	if (fr == FR_OK) {
		dbug("NEW Directory: %s\r\n", cmdline);
	} else {
		dbug("no such dir.\r\n");
	}
}





void CMDTestViewRam(int argc, char* argv[]){
	if (argc < 2) {
        dbug("Usage: view <location>. Eg view 0x200 or view 512\n");
        return;
    }

    int address = strtol(argv[1], NULL, 0);
    //--------------------------------------------------------------//
    uint16_t addr;
    static uint8_t buffer[512];

    if(address>0x7FFFF0){
    	dbug("Attempted memory out of range, $000000 - $7FFFF0\r\n");
    	return;
    }

	address = address & 0xfffffff0;
	unsigned char *txt;
	txt = SDRAM_BANK_ADDR + address;
	dbug("memory block=$%08X...\n", txt);

	for (addr = 0; addr < 512; addr++) {
		buffer[addr] = (unsigned char) *txt;
		txt++;
	}

	txt = SDRAM_BANK_ADDR + address;
	show_ram(&buffer, (unsigned long) 512, txt);
}

void CMDRamPoke(int argc, char* argv[]){
	if (argc < 2) {
        dbug("Usage: poke <location hex/dec> <8bit value hex/dec>. eg. poke 0x200 0xff or poke 512 255\n");
        return;
    }

    int address = strtol(argv[1], NULL, 0);
    unsigned char value = strtol(argv[2], NULL, 0);
    //--------------------------------------------------------------//

    if(address>0x7FFFF0){
    	dbug("Attempted memory out of range, $000000 - $7FFFF0\r\n");
    	return;
    }
	SetMem(address, value);
	dbug("POKE $%08X = $%04X\r\n", address, value);

}

void CMDRamPeek(int argc, char* argv[]){
	if (argc < 2) {
        dbug("Usage: peek <location hex/dec>. eg. peek 0x200 or peek 512\n");
        return;
    }

    int address = strtol(argv[1], NULL, 0);

    //--------------------------------------------------------------//
	unsigned short tRam;
	if(address>0x7FFFF0){
    	dbug("Attempted memory out of range, $000000 - $7FFFF0\r\n");
    	return;
    }

	tRam = GetMem(address);
	dbug("PEEK $%08lX = hex(0x%02X), dec(%u)\r\n",
	     (unsigned long)address,
	     (unsigned int)tRam,
	     (unsigned int)tRam);

}

#include <stdlib.h>
#include <stdint.h>

void CMRamClear(int argc, char *argv[]) {
	if (argc < 3) {
		dbug("Usage: ramclr <start> <length>\n");
		dbug("          |ramclr 0 0 %s\n", lang_get(STR_CMD_RAMCLR_HELP1));
		dbug("          |%s\n", lang_get(STR_CMD_RAMCLR_HELP2));
		return;
	}

	char *ep;

	    uint32_t start_b = strtoul(argv[1], &ep, 0);
	    if (*ep != '\0') { dbug("Invalid start\r\n"); return; }

	    uint32_t len_b = strtoul(argv[2], &ep, 0);
	    if (*ep != '\0') { dbug("Invalid length\r\n"); return; }

	    // Special: 0 0 => clear all
	    if (start_b == 0 && len_b == 0) {
	        dbug("Clearing RAM (ALL)... ");
	        RAM_ClearEXTRam(0, 0xFFFFFFFFu);   // clamps internally
	        dbug("done.\r\n");
	        return;
	    }

	    if (len_b == 0) {
	        dbug("Length is 0 (nothing to do)\r\n");
	        return;
	    }

	    // Compute inclusive end byte (watch overflow)
	    uint32_t end_b = start_b + (len_b - 1u);
	    if (end_b < start_b) { // overflow
	        dbug("Range overflow\r\n");
	        return;
	    }

	    // Convert to word indices for RAM_ClearEXTRam(start, endExclusive)
	    uint32_t start_w = start_b >> 2;
	    uint32_t end_w   = (end_b >> 2) + 1u;     // inclusive end -> exclusive word end

	    if ((start_b & 3u) || (len_b & 3u)) {
	        dbug("Note: rounding to words [%lu..%lu)\r\n",
	             (unsigned long)start_w, (unsigned long)end_w);
	    }

	    dbug("Clearing RAM: start=0x%08lX len=0x%08lX -> words [%lu..%lu]... ",
	         (unsigned long)start_b,
	         (unsigned long)len_b,
	         (unsigned long)start_w,
	         (unsigned long)end_w);

	    RAM_ClearEXTRam(start_w, len_b);

	    dbug("done.\r\n");

}




static void CMDHelp(int argc, char *argv[]) {
	// If user typed: help            -> list usage for all commands
	// If user typed: help <cmd>      -> list usage for that one command
	int slen = strlen(lang_get(STR_CMD_HELP_INTRO));

	for(int i = 0; i < slen; i++) dbug("-"); dbug("\n");
	dbug("*****  %s *****\n", lang_get(STR_CMD_HELP_WORD));
	dbug("%s\n", lang_get(STR_CMD_HELP_INTRO));
	for(int i = 0; i < slen; i++) dbug("-"); dbug("\n");
	const char *target = (argc >= 2 && argv && argv[1]) ? argv[1] : NULL;
	for (int i = 0; commands[i].name != NULL; ++i) {
		// Don't call help from help (infinite recursion / spam)
		if (strcmp(commands[i].name, "help") == 0) continue;

		if (!target || strcmp(target, commands[i].name) == 0) {
			char *fake_argv[] = { (char*) commands[i].name, NULL };
			dbug("%-8s  ", commands[i].name);
			commands[i].func(1, fake_argv);
		}
	}
}



const command_entry_t commands[] = {		// definition of the variable
	{ "help",   	CMDHelp },
	{ "info",		CMDInfoOS },
	{ "hello", 		CMDHelloworld },
	{ "test",  		CMDParamsTest },
	{ "bright",		CMDBrightness },
	{ "reboot",		CMDRebootTest },
	// RAM FUNCTION TESTS
	{ "view",		CMDTestViewRam },
	{ "poke", 		CMDRamPoke },
	{ "peek", 		CMDRamPeek },
	{ "ramclr",		CMRamClear },

	// basic file instructions //
	{ "cat", 		CMDCatFile },
	{ "dir",		CMDDirList },
	{ "ls",			CMDDirList },
	{ "cd",         CMDOSChangeDir },

	{ NULL, NULL }
};
