#include "parser.h"
#include "process_handler.h"

/**
 * mexec.c
 *
 * Responsible for running the main program.
 * 
 * @returns 0 on successfull exit, otherwise EXIT_FAILURE.
 */

int main(const int argc, const char **argv)
{
	// * Setup
	if (argc > 2)
	{
		fprintf(stderr, "Too many arguments. \nUsage: %s [input_file (optional)]", argv[0]);
		exit(EXIT_FAILURE);
	}

	FILE *input_stream = get_input_stream(argc, argv);
	if (input_stream == NULL)
		exit(EXIT_FAILURE);

	int command_count = 0;
	char **commands = read_input_commands(input_stream, &command_count);
	if (commands == NULL)
		exit(EXIT_FAILURE);

	if (input_stream != stdin) 
		fclose(input_stream);
	
	// * Execution
	if (execute_commands(&commands, command_count) == 1)
		exit(EXIT_FAILURE);

	if (wait_for_children(command_count) == 1)
		exit(EXIT_FAILURE);

	// * Cleanup
	free_commands(&commands);

	return 0;
}

