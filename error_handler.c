#include "error_handler.h"

#include <signal.h>
#include <sys/wait.h>

void validate_non_null_pointer(void *pointer, const char *error_msg)
{
	if (pointer == NULL)
	{
		fputs(error_msg, stderr);
		exit(EXIT_FAILURE);
	}
}

void validate_pipe_creation(int return_code)
{
	if (return_code != 0)
	{
		fputs("Error while trying to create pipe\n", stderr);
		exit(EXIT_FAILURE);
	}
}

void validate_pid(int pid)
{
	if (pid < 0)
	{
		fputs("Error while trying to create child process\n", stderr);
		exit(EXIT_FAILURE);
	}
}

void validate_exec(int return_code)
{
	if (return_code == -1)
	{
		fputs("Error while trying to execute command\n", stderr);
		exit(EXIT_FAILURE);
	}
}

void validate_child_status(int return_code, int status)
{
	if (return_code == -1)
	{
		perror("Error while waiting; no child processes");
		exit(EXIT_FAILURE);
	}
	if (!WIFEXITED(status))
	{
		psignal(status, "Child process aborted abnormally");
		exit(EXIT_FAILURE);
	}
}

