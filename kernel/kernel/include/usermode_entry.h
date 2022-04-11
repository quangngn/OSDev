/**
 * Source code provided by professor Curtsinger.
 * Link:
 * https://curtsinger.cs.grinnell.edu/teaching/2022S/CSC395/kernel/usermode.html
 */
#pragma once

#include <stddef.h>
#include <stdint.h>

void usermode_entry(uint64_t data_sel, uintptr_t stack_ptr, uint64_t code_sel,
                    uintptr_t instruction_ptr, ...);
