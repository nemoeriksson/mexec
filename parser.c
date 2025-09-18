
/**
 * parser.c
 *
 * @author c24nen
 * @version 2025.09.17
 */

#include "parser.h"

FILE *get_input_stream(const int argc, const char **argv)
{
	FILE *input_stream = stdin;

	if (argc == 2)
	{
		input_stream = fopen(argv[1], "r");

		if (input_stream == NULL)
		{
			perror("Coudln't find input file");
			return NULL;
		}
	}

	return input_stream;
}

char **get_args(const char *command)
{
	char *arg_delimiter = " ";
	char **args = NULL;
	int arg_count = 0;

	char *dupcmd = strdup(command);
	if (dupcmd == NULL) 
	{
		perror("strdup failed");
		return NULL;
	}

	char *token = strtok(dupcmd, arg_delimiter);
	
	// Populate args list with tokens from command_string
	while(token != NULL)
	{
		// Resize list to fit all arguments
		args = realloc(args, sizeof(*args) * (arg_count+1));
		if (args == NULL)
		{
			perror("Failed to reallocate args list");
			return NULL;
		}

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
		
		line = malloc(sizeof(*line) * MAX_COMMAND_LENGTH);
		if (line == NULL)
		{
			perror("Failed to allocate memory for line");
			return NULL;
		}

		if (fgets(line, MAX_COMMAND_LENGTH, input_stream) == NULL)
		{
			// Free since memory space wont be used if no command is given
			free(line); 
			line = NULL;
		}
		else
			line[strlen(line)-1] = '\0'; // Remove newline

		// Resize list to fit all commands
		commands = realloc(commands, sizeof(*commands) * (*command_count + 1));
		if (commands == NULL)
		{
			perror("Failed to reallocate commands list");
			return NULL;
		}

		commands[*command_count] = line;
	} while (line != NULL);

	return commands;
}

void free_commands(char ***commands_ptr)
{
	if (*commands_ptr == NULL) return;

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
