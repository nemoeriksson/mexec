#include "parser.h"
#include "error_handler.h"

/**
 * Parser.c
 *
 * @author c24nen
 * @version 2025.09.25
 */

// * Helper functions

static char *read_next_line(FILE *input_stream)
{
	char *command = malloc(sizeof(*command) * MAX_COMMAND_LENGTH);
	validate(NON_NULL( command ), NULL, NULL);

	if (fgets(command, MAX_COMMAND_LENGTH, input_stream) == NULL)
	{
		// Free since memory space wont be used if no command is given
		free(command); 
		return NULL;
	}
		
	return command;
}

// * Visibile functions

FILE *get_input_stream(const int argc, const char **argv)
{
	FILE *input_stream = stdin;

	if (argc == 2)
	{
		input_stream = fopen(argv[1], "r");
		validate(NON_NULL(input_stream), NULL, NULL);
	}

	return input_stream;
}

//TODO: Rewrite loop
char **get_args(const char *command)
{
	char *arg_delimiter = " ";
	char **args = NULL;
	int arg_count = 0;

	char *dupcmd = strdup(command);
	validate(NON_NULL( dupcmd ), NULL, NULL);

	char *token = strtok(dupcmd, arg_delimiter);
	
	// Populate args list with tokens from command_string
	while(token != NULL)
	{
		// Resize list to fit all arguments
		args = realloc(args, sizeof(*args) * (arg_count+1));
		validate(NON_NULL( args ), NULL, NULL);

		args[arg_count++] = token;
		token = strtok(NULL, arg_delimiter);
	}	

	// Make list NULL-terminated
	args = realloc(args, sizeof(*args) * (arg_count+1));
	args[arg_count] = NULL;

	return args;
}

char **read_input_commands(FILE *input_stream, int *command_count)
{
	char **commands = NULL;
	char *line = NULL;
	*command_count = -1;

	// Populate the command list
	do {
		(*command_count)++;
		
		line = read_next_line(input_stream);

		if (line != NULL)
			line[strlen(line)-1] = '\0'; // Remove newline

		// Reize list to fit all commands
		commands = realloc(commands, sizeof(*commands) * (*command_count + 1));
		validate(NON_NULL( commands ), NULL, NULL);

		commands[*command_count] = line;
	}
	while (line != NULL);

	return commands;
}

void free_commands(char ***commands_ptr)
{
	if (commands_ptr == NULL) return;

	// Iterate through and free all commands in the list
	char **curr_cmd = *commands_ptr;
	while(*curr_cmd)
	{
		free(*curr_cmd);
		curr_cmd++;
	}

	// Free the list and update the pointer to NULL
	free(*commands_ptr);
	*commands_ptr = NULL;
}

void free_args(char ***args_ptr)
{
	if (args_ptr == NULL) return;

	// Frees the first token returned from stdtok()
	free(**args_ptr);
	
	// Free the list and update the pointer to NULL
	free(*args_ptr);
	*args_ptr = NULL;
}
