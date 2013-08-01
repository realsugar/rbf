#ifndef __BRAINFUCK__
#define __BRAINFUCK__


#include <stdio.h>
#include <stdlib.h>


#define     MAX_CELLS           65536


#define		MIN_SOURCE_SIZE		1
#define		MAX_SOURCE_SIZE		524288


typedef unsigned char   bf_cell;


typedef struct {
    bf_cell     *cell;
    char        *returnPoint;
} bf_stack_item;


typedef struct {
    long            size;
    long            pointer;
    bf_stack_item  *items;  
} bf_stack;


typedef struct {
    bf_stack    *stack;

    // Memory cells
    bf_cell     *firstCell;
    bf_cell     *lastCell;      // optimization
    bf_cell     *currentCell;

    // Code
    char        *firstCommand;
    char        *lastCommand;   // optimization
	char		*currentCommand;
    long        codeSize;    
} bf_state;      


typedef enum {
    CMD_NEXT_CELL,          // >
    CMD_PREV_CELL,          // <
    CMD_INCREMENT,          // +
    CMD_DECREMENT,          // -
    CMD_PRINT_CELL,         // .
    CMD_INPUT_CELL,         // ,
    CMD_JUMP_FORWARD,       // [
    CMD_JUMP_BACKWARD,      // ]
	CMD_CARET_RETURN,		// \r
	CMD_LINEFEED,			// \n
    NUM_COMMANDS
} bf_command;


typedef enum {
    ERR_SUCCESS,
    ERR_UNKNOWN_COMMAND,
    ERR_CELL_OUT_OF_RANGE,	
	ERR_STACK_OVERFLOW,
	ERR_STACK_UNDERFLOW,
	ERR_IO_ERROR,
	ERR_NO_MEMORY,
	ERR_INVALID_SOURCE,	
    NUM_ERRORS
} bf_error;


typedef bf_error (*bf_command_handler) (bf_state*);


typedef struct {
    bf_command          type;
    char                code;
    bf_command_handler  handler;
} bf_command_info;

extern const bf_command_info command_info[NUM_COMMANDS];


typedef struct {
	bf_error		type;
	const char*		messageText;
} bf_error_info;

extern const bf_error_info error_info[NUM_ERRORS];


bf_error new_interpreter(char* sourcePath, bf_state** stateBuffer);
bf_error run_interpreter(bf_state* state);
void free_interpreter(bf_state* state);

void print_error(bf_state* state, bf_error error);


// Utils
long fsize(FILE *fp);

#endif // __BRAINFUCK__
