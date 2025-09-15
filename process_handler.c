#include "process_handler.h"
#include "parser.h"

// * Helper functions

/**
 * Handles the logic for the parent process. Responsible for updating
 * previous_read_fd. Closes all pipe file descriptors.
 * 
 * @param command_index: current command index.
 * @param command_count: total length of the commands list.
 * @param previous_read_fd: pointer to previous read file descriptor, will be updated.
 * @param pipe_fds: pipe file descriptors.
 * @param commands: pointer to the command list.
 */
static void run_parent_process(
		int command_index,
		int command_count,
		int *previous_read_fd,
		int pipe_fds[2],
		char ***commands)
{
	if (command_index != 0)
		validate(NON_NEGATIVE( close(*previous_read_fd) ), commands, NULL);

	if (command_index != command_count - 1)
	{
		validate(NON_NEGATIVE( close(pipe_fds[WRITE_END]) ), commands, NULL);
		*previous_read_fd = pipe_fds[READ_END];
	}
}

/**
 * Handles the logic on the child process and is responsible for
 * running the provided command. Will exit with EXIT_FAILURE if
 * any function fails and sets errno.
 *
 * @param command_index: current command index.
 * @param command_count: total length of the commands list.
 * @param previous_read_fd: previous read file descriptor.
 * @param pipe_fds: pipe file descriptors.
 * @param commands: pointer to the commands list.
 * @param args: pointe to the command arguments.
 */
static void run_child_process(
		int command_index,
		int command_count,
		int previous_read_fd,
		int pipe_fds[2],
		char ***commands,
		char ***args)
{
	if (command_index != 0)
	{
		validate(NON_NEGATIVE( dup2(previous_read_fd, STDIN_FILENO) ), commands, NULL);
		validate(NON_NEGATIVE( close(previous_read_fd) ), commands, NULL);
	}

	if (command_index != command_count - 1)
	{
		validate(NON_NEGATIVE( close(pipe_fds[READ_END]) ), commands, NULL);
		validate(NON_NEGATIVE( dup2(pipe_fds[WRITE_END], STDOUT_FILENO) ), commands, NULL);
		validate(NON_NEGATIVE( close(pipe_fds[WRITE_END]) ), commands, NULL);
	}

	validate(NON_NEGATIVE( execvp(*args[0], *args) ), commands, args);
}


// * Visible functions

void execute_commands(char **commands, int command_count)
{
	int previous_read_fd = -1;
	for (int command_index = 0; command_index < command_count; command_index++)
	{
		int pipe_fds[2];

		// Create pipe as long as it's not the last command since it should
		//	output to stdout.
		if (command_index != command_count - 1)
			validate(NON_NEGATIVE( pipe(pipe_fds) ), &commands, NULL);

		pid_t pid = fork();
		validate(NON_NEGATIVE( pid ), &commands, NULL);

		char **args = get_args(commands[command_index]);

		if (pid == 0)
		{
			run_child_process(command_index, 
					command_count, previous_read_fd, 
					pipe_fds, &commands, &args);
		}

		run_parent_process(command_index, command_count, 
				&previous_read_fd, pipe_fds, &commands);

		free_args(&args);
	}

}

void wait_for_children(int command_count)
{
	for (int i = 0; i < command_count; i++)
	{
		int child_status;
		validate(NON_NEGATIVE( wait(&child_status) ), NULL, NULL);
		validate(CHECK_STATUS( child_status ), NULL, NULL);
	}
}

