#include "main.h"
#include "cmd.h"

#include "timers/timers.h"
#include "filesys.h"
#include <string.h>




const command_entry_t commands[];			// delcaration of this variable
static uint8_t cmd_string[CMD_BUF_SIZE];	// cos things need to change.


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


static void CMDHelpOS(int argc, char *argv[]) {
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
		dbug("Usage: test param1 param2 param3 ... up to 7 params\n");
		return;
	}
    dbug("CMD: %d parameter(s)\n", argc - 1);

    for (int i = 1; i < argc; i++) {
        dbug("  param[%d] = \"%s\"\n", i - 1, argv[i]);
    }
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
	instReboot();
	UNUSED(argc);
	UNUSED(argv);
}


const command_entry_t commands[] = {		// definition of the variable
	{ "info",	CMDHelpOS },
	{ "hello", 	CMDHelloworld },
	{ "test",  	CMDParamsTest },
	{ "reboot", CMDRebootTest },

	// basic file instructions //
	{ "cat", 	CMDCatFile },
	{ NULL, NULL }
};
