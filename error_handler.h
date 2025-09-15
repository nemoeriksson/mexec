#pragma once

#include <stdio.h>
#include <stdlib.h>

void validate_non_null_pointer(void *pointer);
void validate_non_negative(int return_code);
void validate_process_status(int status);

