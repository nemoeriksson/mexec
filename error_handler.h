#pragma once

/**
 * error_handler.h
 *
 * The error handler is responsible for catching any eventual
 * errors from functions that may set errno. 
 *
 * @author c24nen
 * @version 2025.09.25
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

/**
 * Evaluates if a value is equal to NULL.
 *
 * @param ptr: the pointer to check.
 * @returns 1 if parameter is 0 or NULL, else 0.
 */
#define NON_NULL(ptr) (ptr != 0)

/**
 * Checks if a value is greater than or equal to zero.
 *
 * @param val: the value to check.
 * @returns 1 if value is great than or equal to zero, else 0.
 */
#define NON_NEGATIVE(val) (val >= 0)

/**
 * Validates a specified condition in order to see that functions
 * that may set errno completed as expected. Will free the given 
 * lists and exit the program with error code EXIT_FAILURE if the 
 * condition evaluates to false.
 *
 * @param condition: the condition to check.
 * @param cmds_ptr: a pointer to a list of commands.
 * @param args_ptr: a pointer to a list of arguments.
 *
 * @note The command and argument list will be freed if the
 *		condition evaluates to false.
 */
void validate(int condition, char ***cmds_ptr, char ***args_ptr);

/**
 * Checks that the program has recieved the correct amount of
 * command line arguments.
 *
 * @param argc: Amount of program arguments.
 * @param argv: List of program arguments.
 */
void validate_program_arguments(const int argc, const char **argv);
