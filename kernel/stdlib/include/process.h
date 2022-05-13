#pragma once 
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "system.h"

/**
 * Handler to invoke the execution of program with name exec_name.
 * \param exe_name Name of the executable to be exec.
 * \returns true if the function is executed successfully, else return falses.
 */
bool exec(const char* exe_name);

/**
 * Hanlder to exit the current process and invoke shell exec.
 * \returns true if the function is executed successfully, else return falses.
 */
bool exit();