#include "error_handler.h"
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
	validate_program_arguments(argc, argv);

	FILE *input_stream = get_input_stream(argc, argv);
	int command_count = 0;
	char **commands = read_input_commands(input_stream, &command_count);

	if (input_stream != stdin) 
		fclose(input_stream);

	// * Execution
	execute_commands(&commands, command_count);
	wait_for_children(command_count);

	// * Cleanup
	free_commands(&commands);

	return 0;
}

