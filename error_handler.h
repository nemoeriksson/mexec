#pragma once

#include <stdio.h>
#include <stdlib.h>

void validate_non_null_pointer(void *pointer, const char *error_msg);

void validate_pipe_creation(int return_code);

void validate_pid(int pid);

void validate_exec(int return_code);

void validate_child_status(int return_code, int status);
