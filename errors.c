#include "brainfuck.h"


const bf_error_info error_info[NUM_ERRORS] = 
{
	{ ERR_SUCCESS,				"No error" },
	{ ERR_CELL_OUT_OF_RANGE,	"Cell is out of range" },
	{ ERR_INVALID_SOURCE,		"Invalide source code" },
	{ ERR_IO_ERROR,				"I/O failure" },
	{ ERR_NO_MEMORY,			"Not enough memory" },
	{ ERR_STACK_OVERFLOW,		"Stack overflow" },
	{ ERR_STACK_UNDERFLOW,		"Stack underflow" },
	{ ERR_UNKNOWN_COMMAND,		"Unknown command" }
};


void print_error(bf_state* state, bf_error error)
{
	int index = 0;

	while (error_info[index].type != error)
		index++;

	if (state != NULL)
	{
		printf( "\nFatal error %d at position %d (\"%c\"): %s\n", 
			error, 
			state->currentCommand - state->firstCommand + 1,
			*state->currentCommand,
			error_info[index].messageText);
	} 
	else
	{
		printf("\nFatal error %d: %s\n", 
			error,
			error_info[index].messageText);
	}
}
