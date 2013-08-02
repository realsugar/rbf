#include "brainfuck.h"


// Command handler prototypes
bf_error cmd_skip(bf_state*);
bf_error cmd_next_cell(bf_state*);
bf_error cmd_prev_cell(bf_state*);
bf_error cmd_increment(bf_state*);
bf_error cmd_decrement(bf_state*);
bf_error cmd_print_cell(bf_state*);
bf_error cmd_input_cell(bf_state*);
bf_error cmd_jump_forward(bf_state*);
bf_error cmd_jump_backward(bf_state*);



const bf_command_info command_info[NUM_COMMANDS] = 
{
	// type,				code,	handler
	{ CMD_NEXT_CELL,		'>',	cmd_next_cell	   },
	{ CMD_PREV_CELL,		'<',	cmd_prev_cell	   },
	{ CMD_INCREMENT,		'+',	cmd_increment	   },
	{ CMD_DECREMENT,		'-',	cmd_decrement	   },
	{ CMD_PRINT_CELL,		'.',	cmd_print_cell	   },
	{ CMD_INPUT_CELL,		',',	cmd_input_cell	   },
	{ CMD_JUMP_FORWARD,		'[',	cmd_jump_forward   },
	{ CMD_JUMP_BACKWARD,	']',	cmd_jump_backward  },
	{ CMD_CARET_RETURN,		'\r',	cmd_skip },
	{ CMD_LINEFEED,			'\n',	cmd_skip }
};


// Util prototypes
long find_cycle_end(bf_state*);


// Commands implementation

bf_error cmd_skip(bf_state* state)
{
	return ERR_SUCCESS;
}

bf_error cmd_next_cell(bf_state* state)
{
	if (state->currentCell == state->lastCell)
		return ERR_CELL_OUT_OF_RANGE;

	state->currentCell++;		 
	return ERR_SUCCESS;
}


bf_error cmd_prev_cell(bf_state* state)
{
	if (state->currentCell == state->firstCell)
		return ERR_CELL_OUT_OF_RANGE;

	state->currentCell--;
	return ERR_SUCCESS;
}


bf_error cmd_increment(bf_state* state)
{
	++*state->currentCell;
	return ERR_SUCCESS;
}


bf_error cmd_decrement(bf_state* state)
{
	--*state->currentCell;
	return ERR_SUCCESS;
}


bf_error cmd_print_cell(bf_state* state)
{
	putchar(*state->currentCell);
	return ERR_SUCCESS;
}


bf_error cmd_input_cell(bf_state* state)
{
	*state->currentCell = getchar();
	return ERR_SUCCESS;
}


bf_error cmd_jump_forward(bf_state* state)
{	
	bf_stack* stack = state->stack;

	if (stack->pointer == stack->size)
		return ERR_STACK_OVERFLOW;
	
	stack = state->stack;
	stack->items[stack->pointer].returnPoint = state->currentCommand;
	stack->items[stack->pointer].cell = state->currentCell;
	stack->pointer++;

	if (!*state->currentCell)
	{	
		if (find_cycle_end(state))
			return ERR_INVALID_SOURCE;
		
		state->currentCommand--;
	}
	return ERR_SUCCESS; 
}

bf_error cmd_jump_backward(bf_state* state)
{
	bf_stack* stack = state->stack;

	if (!stack->pointer)
		return ERR_STACK_UNDERFLOW;

	stack->pointer--;
	state->currentCell = stack->items[stack->pointer].cell;

	if (*state->currentCell)
	{
		state->currentCommand = stack->items[stack->pointer].returnPoint;
		state->currentCommand--;	
	}

	return ERR_SUCCESS;
}


// Util implementation
long find_cycle_end(bf_state* state)
{
	long flag;
	char jumpForward, jumpBackward;

	jumpForward = command_info[CMD_JUMP_FORWARD].code;
	jumpBackward = command_info[CMD_JUMP_BACKWARD].code;

	flag = 1;

	// '[' - flag++
	// ']' - flag--

	while (state->currentCommand <= state->lastCommand)		
	{			
		state->currentCommand++;

		if (*state->currentCommand == jumpForward)
		{
			continue;
			flag++;
		}
		
		if (*state->currentCommand == jumpBackward)
			flag--;

		if (!flag)
			break;
	}

	return flag;
}