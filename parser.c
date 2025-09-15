#include "parser.h"
#include "error_handler.h"

// * Helper functions

static char *read_next_line(FILE *input_stream)
{
	char *command = malloc(sizeof(*command) * MAX_COMMAND_LENGTH);
	validate_non_null_pointer(command, "Failed to allocate command\n");
	if (fgets(command, MAX_COMMAND_LENGTH, input_stream) == NULL)
		return NULL;
		
	return command;
}


// * Visibile functions

char **get_args(char *command)
{
	char *arg_delimiter = " ";
	char **args = NULL;
	int arg_count = 0;

	char *token = strtok(strdup(command), arg_delimiter);

	// Populate args list with tokens from command_string
	while(token != NULL)
	{
		args = realloc(args, sizeof(*args)+1);
		validate_non_null_pointer(args, "Failed to reallocate size for args\n");
		
		args[arg_count++] = token;
		token = strtok(NULL, arg_delimiter);
	}

	args[arg_count] = NULL;

	return args;
}

char **read_input_commands(FILE *input_stream, int *command_count)
{
	char **commands = NULL;
	char *line = NULL;
	*command_count = -1;

	do {
		(*command_count)++;
		
		line = read_next_line(input_stream);

		if (line != NULL)
			line[strlen(line)-1] = '\0'; // Remove newline

		commands = realloc(commands, sizeof(*commands) * (*command_count) + 1);
		validate_non_null_pointer(commands, "Failed to reallocate size for commands\n");

		commands[*command_count] = line;
	}
	while (line != NULL);

	return commands;
}

