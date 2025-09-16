#pragma once

/**
 * parser.h
 *
 * The parser is responsible for reading user input
 * and split a commands into their respective 
 * arguments that can be used in execvp.
 *
 * @author c24nen
 * @version 2025.09.16
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_COMMAND_LENGTH 1024

/**
 * Gets the input stream to read user input (commands) from.
 * Will default to stdin if no file is provided through command
 * line arguments.
 *
 * @param argc: amount of program arguments.
 * @param argv: list of program arguments.
 *
 * @returns a file pointer to the specified input file. Returns
 *		NULL on failure.
 */
FILE *get_input_stream(const int argc, const char **argv);

/**
 * Splits a given command into it's arguments/tokens to be used in 
 * functions such as execvp. List will be null-terminated.
 *
 * @returns a list of arguments, NULL on failure.
 */
char **get_args(const char *command);

/**
 * Reads user input and returns a list of all entered commands. Will 
 * also give the amount of commands entered through command_count_out.
 *
 * @param input_stream: the stream to read user input from.
 * @param command_count_out: will be set to the the total amount of
 *		command_count_out entered
 *
 * @returns a list of commands or NULL on failure
 */
char **read_input_commands(FILE *input_stream, int *command_count_out);

/**
 * Frees a list of commands and sets the pointer to NULL. Assumes the
 * command list was generated with the read_input_commands function.
 *
 * @param commands_ptr: a pointer to a list of commands.
 */
void free_commands(char ***commands_ptr);

/**
 * Frees a list of arguments and sets the pointer to NULL. Assumes
 * the argument list was generated with the get_args function.
 *
 * @param args_ptr: a pointer to a list of arguments, NULL on failure.
 */
void free_args(char ***args_ptr);


