#include "keyboard.h"

#include "kprint.h"

// True characters based on the scan code
char pressed_key1[] =
    "\e1234567890-=\b\tqwertyuiop[]\n%asdfghjkl;\'`%\\zxcvbnm,./%*% ";
// True characters based on the scan code with the Shift key being pressed
char pressed_key2[] =
    "\e!@#$%^&*()_+\b\tQWERTYUIOP{}\n%ASDFGHJKL:\"~%|ZXCVBNM<>\?    ";

/******************************************************************************/
/**
 * Init the circular queue with default value:
 * - read = 0
 * - write = 0
 * - size = 0
 * Notice: buffer is NOT 0-init
 * \param cq Pointer of a circular queue.
 */
void cq_init(circular_queue_t* cq) {
  if (cq == NULL) return;

  cq->size = 0;
  cq->read = 0;
  cq->write = 0;
}

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
bool cq_read(circular_queue_t* cq, uint64_t* read_val) {
  if (cq == NULL || read_val == NULL) return false;

  if (cq_is_empty(cq)) {
    // If the buffer is empty, we return false
    return false;
  } else {
    // If the buffer is not empty ...
    // Read the value from pointed to by read pointer
    *read_val = (cq->buffer)[(cq->read)];
    // Advance the read pointer (wrap around if possible)
    cq->read = (cq->read + 1) % KEYBOARD_BUFFER_SIZE;
    // Decrease the size of buffer
    cq->size--;
    return true;
  }
}

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
void cq_write(circular_queue_t* cq, uint64_t write_val) {
  if (cq == NULL) return;

  if (cq_is_full(cq)) {
    // If the buffer is full, we overwrite value to buffer
    (cq->buffer)[cq->write] = write_val;
    // Advance the write index (wrap around if possible)
    cq->write = (cq->write + 1) % KEYBOARD_BUFFER_SIZE;
    // Advance the read index (wrap around if possible)
    cq->read = (cq->read + 1) % KEYBOARD_BUFFER_SIZE;
  } else {
    // If the buffer is not full, we write value to buffer
    (cq->buffer)[cq->write] = write_val;
    // Advance the write index (wrap around if possible)
    cq->write = (cq->write + 1) % KEYBOARD_BUFFER_SIZE;
    // Increase the size of buffer
    cq->size++;
  }
}

/******************************************************************************/
/**
 * Init keyboard object with default value:
 * - shift = false
 * - buffer.read = 0
 * - buffer.write = 0
 * - buffer.size = 0
 * \param kb Pointer to pointer of a keyboard object.
 */
void kb_init(keyboard_t* kb) {
  if (kb == NULL) return;

  // Set key states = false
  kb->alt = 0;
  kb->ctrl = 0;
  kb->shift = 0;
  kb->capslock = 0;

  // Set buffer:
  kb->buffer.read = 0;
  kb->buffer.write = 0;
  kb->buffer.size = 0;
}

/**
 * This function is called by the keyboard interrupt handler in idt.h. The scan
 * code being passed to the keyboard to be processed accordingly. We record
 * input when the key is down.
 *
 * We do not differentiate left and right Alt/CTRL key yet.
 * We also do not record to the buffer scancode >= CAPSLOCK_DOWN_SS, which is
 * the limit between printable characters and non-printable characters.
 * 
 * The value being stored to the circular queue has 32 bits where:
 * - bit 0 to 15: store scancode.
 * - bit 16 to 19: Caps Lock state.
 * - bit 20 to 23: ALT state.
 * - bit 24 to 27: CTRL state.  
 * - bit 28 to 31: SHIFT state.
 *
 * \param kb Pointer to keyboard object.
 * \param val Value of the scan code.
 */
void kb_input_scan_code(keyboard_t* kb, uint64_t val) {
  if (kb == NULL) return;

  switch (val) {
    // If the Alt key is pressed, we toggle alt variable with ALT_ON_MASK
    case ALT_DOWN_SS:
    case ALT_UP_SS:
      kb->alt ^= ALT_ON_MASK;
      break;

    // If the Shift key is pressed, we toggle shift variable with SHIFT_ON_MASK
    case LSHIFT_DOWN_SS:
    case LSHIFT_UP_SS:
      kb->shift ^= LSHIFT_ON_MASK;
      break;
    case RSHIFT_DOWN_SS:
    case RSHIFT_UP_SS:
      kb->shift ^= RSHIFT_ON_MASK;
      break;

    // If the CAPSLOCK key is pressed, we toggle capslock with CAPSLOCK_ON_MASK
    case CAPSLOCK_DOWN_SS:
      kb->capslock ^= CAPSLOCK_ON_MASK; 
      break;

    // If the CTRL key is pressed, we toggle ctrl variable with CTRL_ON_MASK
    case CTRL_DOWN_SS:
    case CTRL_UP_SS:
      kb->ctrl ^= CTRL_ON_MASK;
      break;

    default:
      if (val < CAPSLOCK_DOWN_SS) {
        cq_write(&(kb->buffer),
                 val | kb->alt | kb->ctrl | kb->shift | kb->capslock);
      }
  }
}

/**
 * Read one scan code off the buffer and convert to character. If the key is not
 * within the range, we return false.
 * \param kb Pointer to the keyboard object.
 * \param output_char Pointer to output character.
 * \returns: true if the read is successful, else returns false.
 */
bool kb_read_c(keyboard_t* kb, char* output_char) {
  if (kb == NULL || output_char == NULL) return false;

  uint64_t scancode_key;
  if (!cq_read(&kb->buffer, &scancode_key)) {
    *output_char = 0;
    return false;
  }

  switch (scancode_key & 0xFFFF) {
    // Range of non alphabet characters
    case 0x01 ... 0x0F:  // ESC to tab
    case 0x1A ... 0x1C:  // [ to Enter
    case 0x27 ... 0x29:  // ; to ` (back stick)
    case 0x2B:           // forward slash
    case 0x33 ... 0x35:  // , to /
    case 0x37:           // *
    case 0x39:           // space
      // If the shift bit is 1, it means that shift is being pressed down, we
      // return characters from the 2nd key_pressed string
      *output_char = ((scancode_key & SHIFT_ON_MASK) != 0)
                         ? pressed_key2[(scancode_key & 0xFFFF) - 1]
                         : pressed_key1[(scancode_key & 0xFFFF) - 1];
      return true;
    // Range of alphabet characters
    case 0x10 ... 0x19:  // q to p
    case 0x1E ... 0x26:  // a to l
    case 0x2C ... 0x32:  // z to m
      // If the shift bit / capslock bit is 1, it means that shift is being
      // pressed down, we return characters from the 2nd key_pressed string
      *output_char = ((scancode_key & SHIFT_ON_MASK) != 0 ||
                      (scancode_key & CAPSLOCK_ON_MASK) != 0)
                         ? pressed_key2[(scancode_key & 0xFFFF) - 1]
                         : pressed_key1[(scancode_key & 0xFFFF) - 1];
      return true;
    default:
      *output_char = 0;
      return true;
  }
}