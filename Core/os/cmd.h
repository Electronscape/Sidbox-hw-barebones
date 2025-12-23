#ifndef COMMANDLINE_H
#define COMMANDLINE_H


#define MAX_ARGS  		8
#define CMD_BUF_SIZE 	4096

typedef void (*command_func_t)(int argc, char *argv[]);
typedef struct {
	uint8_t *name;
	command_func_t func;
} command_entry_t;


//extern const command_entry_t commands[];
void cmd_entry(uint8_t *linein);


#endif
