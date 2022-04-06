#pragma once

#include <stdint.h>
#include <system.h>

#include "keyboard.h"
#include "kprint.h"
#include "page.h"
#include "port.h"
#include "term.h"

// syscall_handler(...) is being called inside syscall_entry(). syscall_entry()
// is invoked by the interrupt 80.
int64_t syscall_entry_handler(uint64_t nr, uint64_t arg0, uint64_t arg1,
                              uint64_t arg2, uint64_t arg3, uint64_t arg4,
                              uint64_t arg5);

// Syscall handlers: functions to process system calls
/**
 *  Handlers for read system call. Return the number of read characters
 * (excluding the backspace). Reading stops when the user hit enter (this
 * newline character is not added to buffer). The function is not responsible
 * for adding null terminate. If the function failed to read any characters,
 * return -1.
 */
int64_t read_handler(uint64_t f_descriptor, char* buff, size_t read_size);
/**
 * Handler for write system call. Function prints characters in str up to the
 * write_size amount of characters. If the characters is null terminate, we
 * return immediately. The functions return -1 if error occurs, otherwise, we
 * return the number of written characters (exclude null terminate).
 */
int64_t write_handler(uint64_t f_descriptor, const char* str,
                      size_t write_size);
