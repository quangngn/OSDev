#pragma once
#include <stdbool.h>

#define KEYBOARD_BUFFER_SIZE 1024

// Structure
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
