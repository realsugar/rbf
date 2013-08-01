
#include "BrainFuck.h"

void usage()
{
	const char* text =
    "\nReal BrainFuck Interpreter v1.0"\
    "\n"\
    "\n    Usage: brainfuck <sourcepath>"\
    "\n";
    
	printf(text);
}

int main(int argc, char** argv)
{
	bf_state* state = NULL;
	bf_error status;
    
	if (2 != argc)
	{
		usage();
		return 0;
	}
    
	status = new_interpreter(argv[1], &state);
	if (ERR_SUCCESS != status)
	{
		print_error(state, status);
		free_interpreter(state);
		return status;
	}
    
	status = run_interpreter(state);
	if (ERR_SUCCESS != status)
		print_error(state, status);
    
	free_interpreter(state);
	return status;
}

