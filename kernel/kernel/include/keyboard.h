#pragma once

#include <stdbool.h>
#include <stdint.h>

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

// SET KEYBOARD STAGE BITS
#define SHIFT_ON_MASK 0x80000000
#define CTRL_ON_MASK 0x40000000
#define CAPSLOCK_ON_MASK 0x20000000
#define ALT_ON_MASK 0x10000000

#define KEYBOARD_BUFFER_SIZE 1024

// CIRCULAR_QUEUE for KEYBOARD
typedef struct circular_queue {
  int read;
  int write;
  int size;
  uint64_t buffer[KEYBOARD_BUFFER_SIZE];
} circular_queue_t;

// Functions
/*
 * Init the circular queue with default value:
 * - read = 0
 * - write = 0
 * - size = 0
 * - buffer is NOT 0-init
 */
void cq_init(circular_queue_t **cq);
/*
 * Read the data from the buffer.
 * - If the queue is full, we return (unsigned)-1.
 * - Else, we reduced the size, advance the read pointer, and return the value
 * currently pointed at cq->read pointer.
 */
bool cq_read(circular_queue_t *cq, uint64_t *dest_val);
/*
 * Write the data to the buffer.
 * - If the buffer is full, we overwrite the old value. Size value is unchanged.
 * read and write value are both advanced.
 * - Else we write the value at where write pointer points. Size is increased
 * by 1. write is advanced by 1.
 */
void cq_write(circular_queue_t *cq, uint64_t val);
// Functions to check for queue whether it is empty or full
static inline bool cq_is_empty(circular_queue_t *cq) { return cq->size <= 0; }
static inline bool cq_is_full(circular_queue_t *cq) {
  return cq->size >= KEYBOARD_BUFFER_SIZE;
}

// KEYBOARD
// Struct the hold the state of the keyboard, including the input buffer
typedef struct keyboard {
  circular_queue_t buffer;
  uint32_t alt;
  uint32_t ctrl;
  uint32_t shift;
  uint32_t capslock;
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
