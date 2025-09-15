#include "error_handler.h"
#include "parser.h"

/**
 * error_handler.c
 *
 * @author c24nen
 * @version 2025.09.25
 */

void validate(int condition, char ***cmds_ptr, char ***args_ptr)
{
	if (!condition)
	{
		perror("An error has occured");

		if (cmds_ptr != NULL)
			free_commands(cmds_ptr);

		if (cmds_ptr != NULL)
			free_args(args_ptr);
	
		exit(EXIT_FAILURE);
	}
}

void validate_program_arguments(const int argc, const char **argv)
{
	if (argc > 2)
	{
		fprintf(stderr, "Too many arguments provided. \nUsage: %s [input_file:optional]", argv[0]);
		exit(EXIT_FAILURE);
	}
}

