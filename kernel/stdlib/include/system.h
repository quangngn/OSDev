#pragma once

/**
 * Must match with those defined in page.h and syscall.h. The reason why we
 * have to redefine here is that stdlib should stand alone as a separate module.
 */
#define SYSCALL_READ 0
#define SYSCALL_WRITE 1

#define SYSCALL_MMAP 9
#define SYSCALL_MPROTECT 10
#define SYSCALL_MUNMAP 11

#define PAGE_SIZE 4096
#define PAGE_ALIGN_MASK 0xFFFFFFFFFFFFF000

#define USER_STACK 0x70000000000
#define USER_HEAP 0x70000001000

#define STD_IN 0
#define STD_OUT 1
#define STD_ERR 2