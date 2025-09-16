#include "process_handler.h"
#include "parser.h"

// * Helper functions

static void execute(char ***commands_ptr, int command_count, int command_index, int *previous_read_fd_ptr)
{
	int pipe_fds[2];
	int previous_read_fd = *previous_read_fd_ptr;

	// Create pipe as long as it's not the last command since it should output to stdout.
	if (command_index != command_count - 1)
		validate(NON_NEGATIVE( pipe(pipe_fds) ), commands_ptr, NULL);

	pid_t pid = fork();
	validate(NON_NEGATIVE( pid ), commands_ptr, NULL);

	char **args = get_args((*commands_ptr)[command_index]);

	// Child process logic
	if (pid == 0)
	{
		//
		if (command_index != 0)
		{
			validate(NON_NEGATIVE( dup2(previous_read_fd, STDIN_FILENO) ), commands_ptr, NULL);
			validate(NON_NEGATIVE( close(previous_read_fd) ), commands_ptr, NULL);
		}
		
		//
		if (command_index != command_count - 1)
		{
			validate(NON_NEGATIVE( close(pipe_fds[READ_END]) ), commands_ptr, NULL);
			validate(NON_NEGATIVE( dup2(pipe_fds[WRITE_END], STDOUT_FILENO) ), commands_ptr, NULL);
			validate(NON_NEGATIVE( close(pipe_fds[WRITE_END]) ), commands_ptr, NULL);
		}

		validate(NON_NEGATIVE( execvp(args[0], args) ), commands_ptr, &args);
	}
	
	// Parent process logic
	if (command_index != 0)
		validate(NON_NEGATIVE( close(previous_read_fd) ), commands_ptr, NULL);

	if (command_index != command_count - 1)
	{
		validate(NON_NEGATIVE( close(pipe_fds[WRITE_END]) ), commands_ptr, NULL);
		previous_read_fd = pipe_fds[READ_END];
	}

	free_args(&args);

	*previous_read_fd_ptr = previous_read_fd;
}

// * Visible functions

void execute_commands(char ***commands, int command_count)
{
	int previous_read_fd = -1;
	for (int command_index = 0; command_index < command_count; command_index++)
	{
		execute(commands, command_count, command_index, &previous_read_fd);
	}

}

void wait_for_children(int command_count)
{
	for (int i = 0; i < command_count; i++)
	{
		int child_status;
		validate(NON_NEGATIVE( wait(&child_status) ), NULL,NULL);
		
		// Exit if child process failed
		if (WEXITSTATUS(child_status) == EXIT_FAILURE) 			
			exit(EXIT_FAILURE);
	}
}

