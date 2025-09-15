#include "error_handler.h"

#include <stdlib.h>
#include <sys/wait.h>
void validate_non_null_pointer(void *pointer)
{
	if (pointer == NULL)
	{
		perror("ERROR: Expected non-null pointer\n");
		exit(EXIT_FAILURE);
	}
}

void validate_non_negative(int return_code)
{
	if (return_code < 0)
	{
		perror("ERROR: Expected return code above or equal to 0\n");
		exit(EXIT_FAILURE);
	}
}

void validate_process_status(int status)
{
	if (!WIFEXITED(status))
	{
		fputs("ERROR: Process ended abnormally\n", stderr);
		exit(EXIT_FAILURE);
	}
}


