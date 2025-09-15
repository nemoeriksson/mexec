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
		validate_non_null_pointer(input_stream);
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
			validate_non_negative(pipe(pipe_fds));

		pid_t pid = fork();
		validate_non_negative(pid);

		if (pid == 0)
		{
			if (command_index != 0)
			{
				validate_non_negative( dup2(previous_read_fd, STDIN_FILENO) );
				validate_non_negative( close(previous_read_fd) );
			}
			if (command_index != command_count - 1)
			{
				validate_non_negative( close(pipe_fds[READ_END]) );
				validate_non_negative( dup2(pipe_fds[WRITE_END], STDOUT_FILENO) );
				validate_non_negative( close(pipe_fds[WRITE_END]) );
			}

			char **args = get_args(commands[command_index]);
			validate_non_negative( execvp(args[0], args) );
		}

		if (command_index != 0)
			validate_non_negative( close(previous_read_fd) );

		if (command_index != command_count - 1)
		{
			validate_non_negative( close(pipe_fds[WRITE_END]) );
			previous_read_fd = pipe_fds[READ_END];
		}

		int child_status;
		validate_non_negative( waitpid(pid, &child_status, 0) );
		validate_process_status(child_status);
	}

	return 0;
}

