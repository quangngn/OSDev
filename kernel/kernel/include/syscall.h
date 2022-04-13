#pragma once

#include <stdint.h>
#include <system.h>

#include "keyboard.h"
#include "kprint.h"
#include "page.h"
#include "port.h"
#include "term.h"

/**
 * syscall_handler(...) is being called inside syscall_entry(). Notice that
 * syscall_entry() is invoked by the interrupt 80. Based on the value of arg nr,
 * the function would invoke the appropriate system call handlers.
 */
int64_t syscall_entry_handler(uint64_t nr, uint64_t arg0, uint64_t arg1,
                              uint64_t arg2, uint64_t arg3, uint64_t arg4,
                              uint64_t arg5);

/******************************************************************************/
// Syscall handlers: functions to process system calls
/**
 * Handlers for read system call. Return the number of read characters
 * (excluding the null-terminate AND backspace). The function is not responsible
 * for adding null terminate. If the function reads backspace, it would remove
 * the previously read character from the buffer and decrease the character
 * count by 1.
 *
 * The read stopped when:
 * - Read null terminate character '\0'.
 * - Reach the read_size.
 * - Read a newline character.
 *
 * Note: read_size also take into account null-terminated. Hence, the size of
 * buff should be read_size.
 *
 * \param f_descriptor Currently only support stdin (aka 0).
 * \param buff Buffer to be write the read character.
 * \param read_size The size of buffer.
 * \param incl_newln Boolean to decide whether to include newline character.
 * \param echo_char Boolean to decide whether to printout read character to
 * terminal.
 * \param read_char_counter The number of character has been read to str before
 * this function is called. The function would continue at reading to
 * buff[read_char_counter]. The use case is when we are reading to str, str run
 * out of space and get realloc. read_handler is called again and continue from
 * the end of str using read_char_counter value.
 * \returns the number of read bytes (excluding null-terminated). Return -1 if
 * the read failed.
 */
int64_t read_handler(uint64_t f_descriptor, char* buff, size_t read_size,
                     bool incl_newln, bool echo_char,
                     int64_t read_char_counter);

/**
 * Handler for write system call. Function prints characters in str up to the
 * write_size amount of characters. If the characters is null terminate, we
 * return immediately. The functions return -1 if error occurs, otherwise, we
 * return the number of written characters (exclude null terminate).
 *
 * \param f_descriptor: currently support stdout and stderr (aka 1 and 2).
 * \param str: buffer to be written out.
 * \param write_size: number of byte to be written out.
 * \returns -1 if error occurs, otherwise, we return the number of written
 * characters (exclude null terminate).
 */
int64_t write_handler(uint64_t f_descriptor, const char* str,
                      size_t write_size);
