#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

#define NON_NULL(a) (a == 0)
#define NON_NEGATIVE(a) (a < 0)
#define CHECK_STATUS(a) (!WIFEXITED(a))

void validate(int condition, char ***cmds_ptr, char ***args_ptr);

