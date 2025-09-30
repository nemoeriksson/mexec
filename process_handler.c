
/**
 * process_handler.c
 *
 * @author c24nen
 * @version 2025.09.16
 */

#include "process_handler.h"
#include "parser.h"

// * Helper functions

/**
 * Sets up the file descriptors for
 * a child process. Returns 0 on
 * sucess, else 1.
 *
 * @param pip_fds			The file descriptors for the pipe
 * @param command_index		The index of the command to run
 * @param command_count		Total amount of commands
 * @param previous_read_fd	The read file descriptor for the previous pipe
 *
 * @return	0 on sucess, else 1.
 */
static int setup_child_pipes(int pipe_fds[2], 
		int command_index, 
		int command_count, 
		int previous_read_fd)
{
	// Setup STDIN pipe file descriptors
	if (command_index != 0)
	{
		if (dup2(previous_read_fd, STDIN_FILENO) == -1)
		{
			perror("Failed child process pipe STDIN setup");
			return 1;
		}
		
		close(previous_read_fd);
	}
	
	// Setup STDOUT pipe file descriptors
	if (command_index != command_count - 1)
	{
		close(pipe_fds[READ_END]);

		if (dup2(pipe_fds[WRITE_END], STDOUT_FILENO) == -1)
		{
			perror("Failed child process STDOUT setup on child");
			return 1;
		}

		close(pipe_fds[WRITE_END]);
	}

	return 0;
}

/**
 * Closes the pipe file descriptors on 
 * the parent process. Has no fail case.
 *
 * @param pipe_fds				The file descriptors for the pipe
 * @param command_index			The index of the command to run
 * @param command_count			Total amount of commands
 * @param previous_read_fd_ptr	A pointer to the previous read file descriptor.
 *								Will be updated when calling this function.
 */
static void close_parent_pipe(int pipe_fds[2],
		int command_index,
		int command_count,
		int *previous_read_fd_ptr)
{
	if (command_index != 0)
		close(*previous_read_fd_ptr);

	if (command_index != command_count - 1)
	{
		close(pipe_fds[WRITE_END]);
		*previous_read_fd_ptr = pipe_fds[READ_END];
	}
}

/**
 * Creates a new fork, includes error handling.
 * Will return -1 on error, otherwise returns the 
 * pid of the new forked process.
 *
 * @return	The pid of the new 
 */
static pid_t create_child_process(void)
{
	pid_t pid = fork();

	if (pid < 0)
	{
		perror("Error creating fork");
		return -1;
	}

	return pid;
}

/**
 * Opens a new sub process to execute a command. Will pipe
 * the output to the next command if it isn't the last. 
 *
 * @param commands_ptr: a pointer to the commands list.
 * @param command_count: the length of the commands list.
 * @param command_index: index of command to execute.
 * @param previous_read_fd_ptr: a pointer to the previously read end file
 *		descriptor. Will be updated after call to this function.
 *
 * @returns 0 on success, else 1.
 */
static int execute(char ***commands_ptr, 
		int command_count, 
		int command_index, 
		int *previous_read_fd_ptr)
{
	int pipe_fds[2];
	int previous_read_fd = *previous_read_fd_ptr;

	// Create pipe as long as it's not the last command since it should output to stdout.
	if (command_index != command_count - 1)
	{
		if (pipe(pipe_fds) == -1)
		{
			perror("Error creating pipe");
			return 1;
		}
	}

	pid_t pid = create_child_process();
	if (pid == -1) return 1;

	char **args = get_args((*commands_ptr)[command_index]);

	// Child process logic
	if (pid == 0)
	{
		// Setup pipes
		if (setup_child_pipes(pipe_fds, command_index, command_count, previous_read_fd) == 1)
		{
			free_commands(commands_ptr);
			free_args(&args);
			return 1;
		}

		// Execute the command
		if (execvp(args[0], args) == -1)
		{
			perror("Failed to execute command on child");
			free_commands(commands_ptr);
			free_args(&args);
			return 1;
		}
	}
	
	// Close the pipe & file descriptors on parent
	close_parent_pipe(pipe_fds, command_index, command_count, &previous_read_fd);
	
	free_args(&args);

	*previous_read_fd_ptr = previous_read_fd;

	return 0;
}

// * Visible functions

int execute_commands(char ***commands, int command_count)
{
	int previous_read_fd = -1;
	for (int command_index = 0; command_index < command_count; command_index++)
	{
		if (execute(commands, command_count, command_index, &previous_read_fd) == 1)
			return 1;
	}

	return 0;
}

int wait_for_children(int command_count)
{
	for (int i = 0; i < command_count; i++)
	{
		int child_status;
		if (wait(&child_status) == -1)
		{
			perror("No children to wait for");
			return 1;
		}
		
		// Check if child process failed
		if (child_status != 0) 			
			return 1;
	}

	return 0;
}

