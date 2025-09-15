#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error_handler.h"

#define MAX_COMMAND_LENGTH 1024

char **get_args(char *command);
char **read_input_commands(FILE *input_stream, int *command_count_out);

void free_commands(char ***commands_ptr);
void free_args(char ***args_ptr);
