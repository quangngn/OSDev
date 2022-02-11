#include "circular_queue.h"

#include "util.h"

// Function
void cq_init(circular_queue_t** cq) {
  circular_queue_t* new_cq = *cq;
  new_cq->size = 0;
  new_cq->read = 0;
  new_cq->write = 0;
}

bool cq_read(circular_queue_t* cq, uint64_t* dest_val) {
  // Check if the buffer is empty
  if (is_empty_cq(cq)) {
    return false;
  } else {
    // If the buffer is not empty ...
    // Read the value from pointed to by read pointer
    *dest_val = (cq->buffer)[(cq->read)];
    // Advance the read pointer (wrap around if possible)
    cq->read = (cq->read + 1) % KEYBOARD_BUFFER_SIZE;
    // Decrease the size of buffer
    cq->size--;
    return true;
  }
}

void cq_write(circular_queue_t* cq, uint64_t val) {
  if (!is_full(cq)) {
    // If the buffer is not full, we write value to buffer
    (cq->buffer)[cq->write] = val;
    // Advance the write index (wrap around if possible)
    cq->write = (cq->write + 1) % KEYBOARD_BUFFER_SIZE;
    // Increase the size of buffer
    cq->size++;
  } else {
    // If the buffer is full, we overwrite value to buffer
    (cq->buffer)[cq->write] = val;
    // Advance the write index (wrap around if possible)
    cq->write = (cq->write + 1) % KEYBOARD_BUFFER_SIZE;
    // Advance the read index (wrap around if possible)
    cq->read = (cq->read + 1) % KEYBOARD_BUFFER_SIZE;
  }
}
