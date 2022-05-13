#include "process.h"

// External functions for system call handler. syscall(uint64_t nr, ...) is
// defined in asm/syscall.s
extern int64_t syscall(uint64_t nr, ...);

/**
 * Handler to invoke the execution of program with name exec_name.
 * \param exe_name Name of the executable to be exec.
 * \returns true if the function is executed successfully, else return falses.
 */
bool exec(const char* exe_name) { return syscall(SYSCALL_EXEC, exe_name); }

/**
 * Hanlder to exit the current process and invoke shell exec.
 * \returns true if the function is executed successfully, else return falses.
 */
bool exit() { return syscall(SYSCALL_EXIT); }