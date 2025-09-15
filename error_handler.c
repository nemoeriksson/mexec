#include "error_handler.h"
#include "parser.h"

void validate(int condition, char ***cmds_ptr, char ***args_ptr)
{
	if (condition)
	{
		perror("An error has occured");

		if (cmds_ptr != NULL)
			free_commands(cmds_ptr);

		if (cmds_ptr != NULL)
			free_args(args_ptr);
	
		exit(EXIT_FAILURE);
	}
}


