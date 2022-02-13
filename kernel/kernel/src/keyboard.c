#include "keyboard.h"

#include "kprint.h"

// True characters based on the scan code
char pressed_key1[] =
    "\e1234567890-=\b\tqwertyuiop[]\n%asdfghjkl;\'`%\\zxcvbnm,./%*% ";
// True characters based on the scan code with the Shift key being pressed
char pressed_key2[] =
    "\e!@#$%^&*()_+\b\tQWERTYUIOP{}\n%ASDFGHJKL:\"~%|ZXCVBNM<>\?    ";

// Function for circular queue
void cq_init(circular_queue_t** cq) {
  circular_queue_t* new_cq = *cq;
  new_cq->size = 0;
  new_cq->read = 0;
  new_cq->write = 0;
}

bool cq_read(circular_queue_t* cq, uint64_t* dest_val) {
  if (cq_is_empty(cq)) {
    // If the buffer is empty, we return false
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
  if (cq_is_full(cq)) {
    // If the buffer is full, we overwrite value to buffer
    (cq->buffer)[cq->write] = val;
    // Advance the write index (wrap around if possible)
    cq->write = (cq->write + 1) % KEYBOARD_BUFFER_SIZE;
    // Advance the read index (wrap around if possible)
    cq->read = (cq->read + 1) % KEYBOARD_BUFFER_SIZE;
  } else {
    // If the buffer is not full, we write value to buffer
    (cq->buffer)[cq->write] = val;
    // Advance the write index (wrap around if possible)
    cq->write = (cq->write + 1) % KEYBOARD_BUFFER_SIZE;
    // Increase the size of buffer
    cq->size++;
  }
}

// Functions for keyboard
/*
 * Init keyboard object with default value:
 * - shift = false
 * - buffer.read = 0
 * - buffer.write = 0
 * - buffer.size = 0
 */
void kb_init(keyboard_t** kb) {
  keyboard_t* new_kb = *kb;
  // Set key states = false
  new_kb->alt = 0;
  new_kb->ctrl = 0;
  new_kb->shift = 0;
  new_kb->capslock = 0;

  // Set buffer:
  new_kb->buffer.read = 0;
  new_kb->buffer.write = 0;
  new_kb->buffer.size = 0;
}

/*
 * This function is called by the keyboard interrupt handler. The scan code
 * being passed to the keyboard to be processed accordingly. For now, we record
 * input when the key is down.
 *
 * For now we do not support left and right Alt/Shift/CTRL key yet. We also does
 * not record to the buffer scancode >= CAPSLOCK_DOWN_SS. This is the limit
 * between printable characters and non-printable characters
 */
void kb_input_scan_code(keyboard_t* kb, uint64_t val) {
  // Special case for tab pressed scancode on Mac, which is 0x40000017 instead
  // of 0x0F

  switch (val) {
    // If the Alt key is pressed, we set alt variable to ALT_ON_MASK
    case ALT_DOWN_SS:
      kb->alt = ALT_ON_MASK;
      break;
    // If the Alt key is released, we set shift variable to 0
    case ALT_UP_SS:
      kb->alt = 0;
      break;

    // If the Shift key is pressed, we set shift variable to SHIFT_ON_MASK
    case LSHIFT_DOWN_SS:
    case RSHIFT_DOWN_SS:
      kb->shift = SHIFT_ON_MASK;
      break;
    // If the Shift key is released, we set shift variable to 0
    case LSHIFT_UP_SS:
    case RSHIFT_UP_SS:
      kb->shift = 0;
      break;

    // If the CAPSLOCK key is pressed, we toggle capslock with CAPSLOCK_ON_MASK
    case CAPSLOCK_DOWN_SS:
      kb->capslock ^= CAPSLOCK_ON_MASK;  // Toggle the caplock bit on/off
      break;

    // If the CTRL key is pressed, we set ctrl variable to CTRL_ON_MASK
    case CTRL_DOWN_SS:
      kb->ctrl = CTRL_ON_MASK;
      break;
    // If the CTRL key is released, we set ctrl variable to 0
    case CTRL_UP_SS:
      kb->ctrl = 0;
      break;

    default:
      if (val < CAPSLOCK_DOWN_SS) {
        cq_write(&(kb->buffer),
                 val | kb->alt | kb->ctrl | kb->shift | kb->capslock);
      }
  }
}

/*
 * Read one key off the buffer.
 * If the key is not within the range, we return 0
 */
bool kb_read_c(keyboard_t* kb, char* output_char) {
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