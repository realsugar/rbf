#include "brainfuck.h"


long fsize(FILE *fp)
{
	long result, currentPos;
	
	currentPos = ftell(fp);
	fseek(fp, 0, SEEK_END);
	result = ftell(fp);
	fseek(fp, currentPos, SEEK_SET);

	return result;
}

