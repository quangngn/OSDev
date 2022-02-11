#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "circular_queue.h"

// ESC key
#define ESC_DOWN_SS 0x01
#define ESC_UP_SS 0x81
// Shift keys
#define LSHIFT_DOWN_SS 0x2A
#define RSHIFT_DOWN_SS 0x36
#define LSHIFT_UP_SS 0xAA
#define RSHIFT_UP_SS 0xB6
// Control keys
#define CTRL_DOWN_SS 0x1D
#define CTRL_UP_SS 0x9D
// Alt keys
#define ALT_DOWN_SS 0x38
#define ALT_UP_SS 0xB8
// Enter key
#define ENTER_DOWN_SS 0x1C
#define ENTER_UP_SS 0x9C
// Capslock key
#define CAPSLOCK_DOWN_SS 0x3A
#define CAPSLOCK_UP_SS 0xBA

// ASCII code
#define ASCII_ESC 27
#define ASCII_BACKSPACE 8

// True characters based on the scan code
char ss_code_1st[] =
    "\e1234567890-=\b"
    "\tqwertyuiop[]"
    "\n%asdfghjkl;\'`%\\"
    "zxcvbnm,./%*% ";
// True characters based on the scan code with the Shift key being pressed
char ss_code_2nd[] =
    "\e!@#$%^&*()_+\b"
    "\tQWERTYUIOP{}"
    "\n%ASDFGHJKL:\"~%|"
    "ZXCVBNM<>\?";

typedef struct keyboard {
  circular_queue_t buffer;
  bool alt;
  bool ctrl;
  bool shift;
  bool capslock;
} keyboard_t;

// Functions
/*
 * Init keyboard object with default value:
 * - shift = false
 * - buffer.read = 0
 * - buffer.write = 0
 * - buffer.size = 0
 */
void kb_init(keyboard_t **kb);

/*
 * This function is called by the keyboard interrupt handler. The scan code
 * being passed to the keyboard to be processed accordingly. For now, we record
 * input when the key is down.
 *
 * For now we do not support left and right Alt/Shift/CTRL key yet. We also does
 * not record to the buffer scancode >= CAPSLOCK_DOWN_SS. This is the limit
 * between printable characters and non-printable characters
 */
void kb_input_scan_code(keyboard_t *kb, uint64_t val);

/*
 * Read one key off the buffer. The result is written to output_char variable
 * then return true. If the key is not within the printable range, we write 0
 * then return true. If the buffer is empty, return false.
 */
bool kb_read_c(keyboard_t *kb, char *output_char);
