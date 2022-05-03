#pragma once

#include <graphic.h>
#include <stdint.h>
#include <system.h>

#include "executable.h"
#include "keyboard.h"
#include "kgraphic.h"
#include "kprint.h"
#include "page.h"
#include "port.h"
#include "stivale2.h"
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

/**
 * Handler to invoke the execution of program with name exec_name.
 * \param exe_name Name of the executable to be exec.
 * \returns true if the function is executed successfully, else return falses.
 */
bool exec_handler(const char* exe_name);

/**
 * Hanlder to exit the current process and invoke shell exec.
 * \returns true if the function is executed successfully, else return falses.
 */
bool exit_handler();

/**
 * Handler to handler query kernel's framebuffer information. The information is
 * written to a user's framebuffer information struct.
 * \param fb_info Pointer to the user's framebuffer information struct.
 * \returns true if query successfully and false otherwise.
 */
bool get_framebuffer_info_handler(framebuffer_info_t* fb_info);

/**
 * Copy src buffer to the kernel's framebuffer at coordinate (dst_x, dst_y).
 * This is similar to drawing a window onto the screen. We don't draw the
 * portion of the window outside of the screen.
 *
 * XY-coordinate has the origin on top-left corner.
 *
 * \param src Address of the source buffer.
 * \param dst_x The x coordinate on the destination buffer.
 * \param dst_y The y coordinate on the destination buffer.
 * \param src_w The width of the source buffer.
 * \param src_h The height of the source buffer.
 * \param flip Boolean whether we flip the source buffer.
 * \returns true if copy successfully and false otherwise.
 */
bool framebuffer_cpy_handler(pixel_t* src, int32_t dst_x, int32_t dst_y,
                             uint32_t src_w, uint32_t src_h, bool flip);