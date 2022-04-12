/**
 * By professor Charlie Curtsinger
 * src:
 * https://curtsinger.cs.grinnell.edu/teaching/2022S/CSC395/kernel/usermode.html
 */
#pragma once
#include "util.h"

// Define the offsets into the GDT where we'll place important descriptors
#define KERNEL_CODE_SELECTOR 0x08
#define KERNEL_DATA_SELECTOR 0x10
#define USER_CODE_SELECTOR 0x18
#define USER_DATA_SELECTOR 0x20
#define TSS_SELECTOR 0x28

// Set up and load the GDT
void gdt_setup();
