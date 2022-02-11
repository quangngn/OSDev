#include "keyboard.h"

// Functions
/*
 * Init keyboard object with default value:
 * - shift = false
 * - buffer.read = 0
 * - buffer.write = 0
 * - buffer.size = 0
 */
void kb_init(keyboard_t **kb) {
  keyboard_t *new_kb = *kb;
  // Set key states = false
  new_kb->alt = false;
  new_kb->ctrl = false;
  new_kb->shift = false;
  new_kb->capslock = false;

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
void kb_input_scan_code(keyboard_t *kb, uint64_t val) {
  switch (val) {
    // If the Alt key is pressed, we set alt variable to true
    case ALT_DOWN_SS:
      kb->alt = true;
      break;
    case ALT_UP_SS:
      kb->alt = false;
      break;

    // If the Shift key is pressed, we set shift variable to true
    case LSHIFT_DOWN_SS:
    case RSHIFT_DOWN_SS:
      kb->shift = true;
      break;
    // If the Shift key is released, we set shift variable to false
    case LSHIFT_UP_SS:
    case RSHIFT_UP_SS:
      kb->shift = false;
      break;

    // If the CAPSLOCK key is pressed, we set capslock variable to true
    case CAPSLOCK_DOWN_SS:
      kb->capslock = !kb->capslock;
      break;
    // // If the CAPSLOCK key is released, we set capslock variable to false
    // case CAPSLOCK_UP_SS:
    //   kb->capslock = false;
    //   break;

    // If the CTRL key is pressed, we set ctrl variable to true
    case CTRL_DOWN_SS:
      kb->ctrl = true;
      break;
    // If the CTRL key is released, we set ctrl variable to false
    case CTRL_UP_SS:
      kb->ctrl = false;
      break;

    default:
      if (val < CAPSLOCK_DOWN_SS) cq_write(&kb->buffer, val);
  }
}

/*
 * Read one key off the buffer.
 * If the key is not within the range, we return 0
 */
bool kb_read_c(keyboard_t *kb, char *output_char) {
  int scancode_key;
  if (!cq_read(&kb->buffer, &scancode_key)) {
    output_char = '\0';
    return false;
  }

  switch (scancode_key) {
    // Range of non alphabet characters
    case 0x01 ... 0x0F:  // ESC to tab
    case 0x1A ... 0x1C:  // [ to Enter
    case 0x27 ... 0x29:  // ; to ` (back stick)
    case 0x2B:           // forward slash
    case 0x33 ... 0x35:  // , to /
    case 0x37:           // *
    case 0x39:           // space
      output_char = kb->shift ? ss_code_2nd[scancode_key - 1]
                              : ss_code_1st[scancode_key - 1];
      return true;
    // Range of alphabet characters
    case 0x10 ... 0x19:  // q to p
    case 0x1E ... 0x26:  // a to l
    case 0x2C ... 0x32:  // z to m
      output_char = (kb->shift || kb->capslock) ? ss_code_2nd[scancode_key - 1]
                                                : ss_code_1st[scancode_key - 1];
      return true;
    default:
      output_char = '\0';
      return true;
  }
}