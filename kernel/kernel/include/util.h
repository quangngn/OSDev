#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "stivale2.h"

// Halt the CPU in an infinite loop
void halt();

// Convert physical address to virtual address by adding hhdm base virtual
// address
uintptr_t ptov(uintptr_t paddr);
