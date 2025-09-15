#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "error_handler.h"
#include "parser.h"

#define READ_END 0
#define WRITE_END 1

// TODO: Add const everywhere I can in function parameters
// TODO: Add free() statements
// TODO: Rewrite error_handler

FILE *open_input_file(char **argv);

int main(int argc, char **argv)
{
	FILE *input_stream = stdin;
	if (argc > 1)
		input_stream = open_input_file(argv);

	int command_count = 0;
	char **commands = read_input_commands(input_stream, &command_count);	

	int previous_read_fd = -1;
	for (int command_index = 0; command_index < command_count; command_index++)
	{
		int pipe_fds[2];

		if (command_index != command_count - 1)
			validate_pipe_creation(pipe(pipe_fds));

		pid_t pid = fork();
		validate_pid(pid);

		if (pid == 0)
		{
			if (command_index != 0)
			{
				dup2(previous_read_fd, STDIN_FILENO);
				close(previous_read_fd);
			}
			if (command_index != command_count - 1)
			{
				close(pipe_fds[READ_END]);
				dup2(pipe_fds[WRITE_END], STDOUT_FILENO);
				close(pipe_fds[WRITE_END]);
			}

			char **args = get_args(commands[command_index]);
			validate_exec(execvp(args[0], args));
		}

		if (command_index != 0)
			close(previous_read_fd);

		if (command_index != command_count - 1)
		{
			close(pipe_fds[WRITE_END]);
			previous_read_fd = pipe_fds[READ_END];
		}

		int child_status;
		int wait_return_code = waitpid(pid, &child_status, 0);
		validate_child_status(wait_return_code, child_status);
	}

	return 0;
}

FILE *open_input_file(char **argv)
{
	FILE *input_stream = fopen(argv[1], "r");

	validate_non_null_pointer(input_stream, "Couldn't open input file\n");

	return input_stream;
}


