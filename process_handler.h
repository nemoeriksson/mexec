#pragma once

/**
 * process_handler.h
 *
 * Responsible for handling subprocess logic and making sure
 * the pipes, processes, and eventuall error handling is taken
 * care of.
 *
 * @author c24nen
 * @version 2025.09.25
 */

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "error_handler.h"

#define READ_END 0
#define WRITE_END 1

/**
 * Creates subprocesses for all commands in the provided list and
 * sets up pipes for communication between them. Will exit with
 * EXIT_FAILURE if any function that sets errno fails.
 *
 * @param commands: a list of all commands to execute as subprocesses
 * @param command_count: the length of the commands list
 */
void execute_commands(char **commands, int command_count);

/**
 * Waits for command_count amount of subprocesses. Will exit with EXIT_FAILURE
 * if any function that sets errno fails.
 *
 * @param command_count: the amount of subprocesses to wait for. Is assumed to 
 *		correspond to the length of the command list given in execute_commands().
 *		Will exit with EXIT_FAILURE if any error occurs.
 */
void wait_for_children(int command_count);


