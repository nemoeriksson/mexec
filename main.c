#include <complex.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "error_handler.h"
#include "parser.h"

#define READ_END 0
#define WRITE_END 1

// TODO: Add const everywhere I can in function parameters
// TODO: Add free() statements

int main(int argc, char **argv)
{
	FILE *input_stream = stdin;
	if (argc > 2)
	{
		fprintf(stderr, "Too many arguments provided.\n Usage: %s [input_file]", argv[0]);
		exit(EXIT_FAILURE);
	}
	if (argc == 2)
	{
		input_stream = fopen(argv[1], "r");
		validate(NON_NULL(input_stream), NULL, NULL);
	}

	int command_count = 0;
	char **commands = read_input_commands(input_stream, &command_count);

	if (input_stream != stdin)
		fclose(input_stream);

	int previous_read_fd = -1;
	for (int command_index = 0; command_index < command_count; command_index++)
	{
		int pipe_fds[2];

		if (command_index != command_count - 1)
			validate(NON_NEGATIVE( pipe(pipe_fds) ), &commands, NULL);

		pid_t pid = fork();
		validate(NON_NEGATIVE( pid ), &commands, NULL);

		char **args = NULL;

		if (pid == 0)
		{
			if (command_index != 0)
			{
				validate(NON_NEGATIVE( dup2(previous_read_fd, STDIN_FILENO) ), &commands, NULL);
				validate(NON_NEGATIVE( close(previous_read_fd) ), &commands, NULL);
			}
			if (command_index != command_count - 1)
			{
				validate(NON_NEGATIVE( close(pipe_fds[READ_END]) ), &commands, NULL);
				validate(NON_NEGATIVE( dup2(pipe_fds[WRITE_END], STDOUT_FILENO) ), &commands, NULL);
				validate(NON_NEGATIVE( close(pipe_fds[WRITE_END]) ), &commands, NULL);
			}

			args = get_args(commands[command_index]);
			validate(NON_NEGATIVE( execvp(args[0], args) ), &commands, &args);
		}

		if (command_index != 0)
			validate(NON_NEGATIVE( close(previous_read_fd) ), &commands, NULL);

		if (command_index != command_count - 1)
		{
			validate(NON_NEGATIVE( close(pipe_fds[WRITE_END]) ), &commands, NULL);
			previous_read_fd = pipe_fds[READ_END];
		}

		free_args(&args);
	}

	free_commands(&commands);

	// Wait for all child processes
	for (int i = 0; i < command_count; i++)
	{
		int child_status;
		validate(NON_NEGATIVE( wait(&child_status) ), NULL, NULL);
		validate(CHECK_STATUS( child_status ), NULL, NULL);
	}

	return 0;
}

