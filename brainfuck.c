#include "BrainFuck.h"


// Util prototypes
bf_error load_source(char *sourcePath, char** buffer, long* bufferSize);
bf_error allocate_stack(bf_state *state);


// Main interpeter functions implementaion

bf_error new_interpreter(char* sourcePath, bf_state** stateBuffer)
{
	bf_error status;
	bf_state* state = (bf_state*) calloc(1, sizeof(bf_state));
	if (NULL == state)
		return ERR_NO_MEMORY;	

	status = load_source(sourcePath, &state->firstCommand, &state->codeSize);
	if (ERR_SUCCESS != status) 
	{
		free_interpreter(state);
		return status;
	}

	state->lastCommand = state->firstCommand +  state->codeSize - 1;
	state->currentCommand = state->firstCommand;

	status = allocate_stack(state);
	if (ERR_SUCCESS != status)
		return status;

	state->firstCell = (bf_cell*) calloc(1, MAX_CELLS);
	if (NULL == state->firstCell) 
		return ERR_NO_MEMORY;
	
	state->lastCell = state->firstCell + MAX_CELLS - 1;
	state->currentCell = state->firstCell;	

	*stateBuffer = state;
	return ERR_SUCCESS;
}


void free_interpreter(bf_state* state)
{
	if (NULL == state)
		return;

	if (NULL != state->stack)
	{
		if (NULL != state->stack->items)
			free(state->stack->items);

		free(state->stack);
	}

	if (NULL != state->firstCell)
		free(state->firstCell);

	if (NULL != state->firstCommand)
		free(state->firstCommand);

	free(state);
}


bf_error run_interpreter(bf_state* state)
{
	int index;
	bf_error status = ERR_SUCCESS;

	for ( ;; )
	{		
		for (index = 0; index < NUM_COMMANDS; index++)
		{
			if (*state->currentCommand == command_info[index].code) 
			{
				status = command_info[index].handler(state);
				break;
			}
		}

		if (ERR_SUCCESS != status)
			break;

		if (index == NUM_COMMANDS)
		{
			status = ERR_UNKNOWN_COMMAND;
			break;
		}

		if (state->currentCommand == state->lastCommand)
		{
			status = ERR_SUCCESS;
			break;
		}

		state->currentCommand++;
	}

	return status;
}


// Util implementation

bf_error load_source(char *sourcePath, char** buffer, long* bufferSize)
{
	FILE *fp;
	long size;

	fp = fopen(sourcePath, "rb");
	if (NULL == fp)
		return ERR_IO_ERROR;

	size = fsize(fp);	
	if (size < MIN_SOURCE_SIZE || size > MAX_SOURCE_SIZE)
	{
		fclose(fp);
		return ERR_INVALID_SOURCE;
	}

	*bufferSize = size; 

	*buffer = (char*) calloc(1, *bufferSize);
	if (NULL == *buffer)
		return ERR_NO_MEMORY;

	size = (long) fread(*buffer, 1, *bufferSize, fp);
	fclose(fp);

	if (size != *bufferSize)
	{		
		free(*buffer);
		*buffer = NULL;
		return ERR_IO_ERROR;
	}

	return ERR_SUCCESS;
}


long calculate_stack_size(bf_state* state, long* stackSize)
{
	long flag;
	char* command;
	char jumpForward, jumpBackward;	

	jumpForward = command_info[CMD_JUMP_FORWARD].code;
	jumpBackward = command_info[CMD_JUMP_BACKWARD].code;

	*stackSize = flag = 0;

	for (command = state->firstCommand; 
		 command <= state->lastCommand; 
		 command++)
	{
		if (jumpForward == *command)
		{
			flag++;
			++*stackSize;
			continue;
		} 

		if (jumpBackward == *command)
			flag--;
	}

	return flag;
}


bf_error allocate_stack(bf_state *state)
{
	long stackSize;

	if (calculate_stack_size(state, &stackSize))
		return ERR_INVALID_SOURCE;

	// No cycles in code - no need to allocate stack
	if (!stackSize)
		return ERR_SUCCESS;

	state->stack = (bf_stack*) calloc(1, sizeof(bf_stack));
	if (NULL == state->stack)
		return ERR_NO_MEMORY;	

	state->stack->size = stackSize;
	state->stack->items = (bf_stack_item *) calloc(stackSize, sizeof(bf_stack_item));
	if (NULL == state->stack->items)
	{
		free(state->stack);
		state->stack = NULL;
		return ERR_NO_MEMORY;
	}

	state->stack->pointer = 0;
	return ERR_SUCCESS;
}
