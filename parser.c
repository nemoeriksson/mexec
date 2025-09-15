#include "parser.h"
#include "error_handler.h"

// * Helper functions

static char *read_next_line(FILE *input_stream)
{
	char *command = malloc(sizeof(*command) * MAX_COMMAND_LENGTH);
	validate(NON_NULL( command ), NULL, NULL);

	if (fgets(command, MAX_COMMAND_LENGTH, input_stream) == NULL)
	{
		free(command);
		return NULL;
	}
		
	return command;
}

// * Visibile functions

char **get_args(char *command)
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
		args = realloc(args, sizeof(*args)*(arg_count+1));
		validate(NON_NULL( args ), NULL, NULL);

		args[arg_count++] = token;
		token = strtok(NULL, arg_delimiter);
	}	

	args = realloc(args, sizeof(*args) * (arg_count+1));
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

	int i = 0;
	while((*commands_ptr)[i])
	{
		free((*commands_ptr)[i]);
		i++;
	}

	free(*commands_ptr);
	*commands_ptr = NULL;
}

void free_args(char ***args_ptr)
{
	if (args_ptr == NULL) return;

	free(*args_ptr);
	*args_ptr = NULL;
}
