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
#define LSHIFT_ON_MASK 0x20000000
#define RSHIFT_ON_MASK 0x10000000
#define SHIFT_ON_MASK 0x30000000

#define CTRL_ON_MASK 0x01000000

#define ALT_ON_MASK 0x0010000

#define CAPSLOCK_ON_MASK 0x00010000

#define KEYBOARD_BUFFER_SIZE 1024

/******************************************************************************/
// CIRCULAR_QUEUE for KEYBOARD
typedef struct circular_queue {
  int read;
  int write;
  int size;
  uint64_t buffer[KEYBOARD_BUFFER_SIZE];
} circular_queue_t;

/**
 * Init the circular queue with default value:
 * - read = 0
 * - write = 0
 * - size = 0
 * Notice: buffer is NOT 0-init
 * \param cq Pointer of a circular queue.
 */
void cq_init(circular_queue_t *cq);

/**
 * Read the data from the buffer. If the queue is empty, we return false, else:
 * - Read data pointed to by cq->read index
 * - Advance cq->read index.
 * - Reduce buffer size (number of elements not bytes).
 *
 * \param cq Pointer to the circular queue.
 * \param read_val Pointer to the variable where cq_read stores the read
 * character.
 * \returns true if the read was successful, else returns false.
 */
bool cq_read(circular_queue_t *cq, uint64_t *read_val);

/**
 * Write the data to the buffer. If the queue is full, we overwrite the oldest
 * value, else:
 * - Write val to the buffer at index cq->write.
 * - Advance cq->write index.
 * - Increase the buffer size (number of elements not bytes).
 *
 * \param cq Pointer to the circular queue.
 * \param write_val Value to be written.
 */
void cq_write(circular_queue_t *cq, uint64_t write_val);

// Functions to check for queue whether it is empty or full
static inline bool cq_is_empty(circular_queue_t *cq) { return cq->size <= 0; }
static inline bool cq_is_full(circular_queue_t *cq) {
  return cq->size >= KEYBOARD_BUFFER_SIZE;
}

/******************************************************************************/
// KEYBOARD
// Struct the hold the state of the keyboard, including the input buffer
typedef struct keyboard {
  circular_queue_t buffer;
  uint32_t alt;
  uint32_t ctrl;
  uint32_t shift;
  uint32_t capslock;
} keyboard_t;

/**
 * Init keyboard object with default value:
 * - shift = false
 * - buffer.read = 0
 * - buffer.write = 0
 * - buffer.size = 0
 * \param kb Pointer to pointer of a keyboard object.
 */
void kb_init(keyboard_t *kb);

/**
 * This function is called by the keyboard interrupt handler in idt.h. The scan
 * code being passed to the keyboard to be processed accordingly. We record
 * input when the key is down.
 *
 * We do not differentiate left and right Alt/Shift/CTRL key yet.
 * We also do not record to the buffer scancode >= CAPSLOCK_DOWN_SS, which is
 * the limit between printable characters and non-printable characters.
 *
 * \param kb Pointer to keyboard object.
 * \param val Value of the scan code.
 */
void kb_input_scan_code(keyboard_t *kb, uint64_t val);

/**
 * Read one scan code off the buffer and convert to character. If the key is not
 * within the range, we return false.
 * \param kb Pointer to the keyboard object.
 * \param output_char Pointer to output character.
 * \returns: true if the read is successful, else returns false.
 */
bool kb_read_c(keyboard_t *kb, char *output_char);
