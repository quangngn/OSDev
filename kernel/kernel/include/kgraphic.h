#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <system.h>

#include "stivale2.h"

/**
 * Read the framebuffer struct tag to gain information about the current
 * framebuffer. The function also initialize psf font. 
 * \returns true if init successfully else returns false.
 */
bool kgraphic_init();

/**
 * Set the framebuffer value to 0
 */ 
void kgraphic_clear_buffer();