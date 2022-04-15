#pragma once

/******************************************************************************/
// Syscall number
#define SYSCALL_READ 0
#define SYSCALL_WRITE 1
#define SYSCALL_MMAP 9
#define SYSCALL_MPROTECT 10
#define SYSCALL_MUNMAP 11
#define SYSCALL_EXEC 59
#define SYSCALL_EXIT 60

/******************************************************************************/
// Page related 
#define NUM_PT_ENTRIES 512
// This is also the size of pml4 entry, pdpt entry, pd entry
#define BYTE_SIZE_OF_PT_ENTRY 8
// PAGE_SIZE = NUM_PT_ENTRIES * BYTE_SIZE_OF_PT_ENTRY = 4KB
#define PAGE_SIZE 4096
#define PAGE_ALIGN_MASK 0xFFFFFFFFFFFFF000

/******************************************************************************/
// Mem location for stack and heap
#define USER_STACK 0x70000000000
#define USER_HEAP  0x90000000000

#define KERNEL_HEAP 0xffff900000000000
/******************************************************************************/
// I/O related
#define STD_IN 0
#define STD_OUT 1
#define STD_ERR 2

/******************************************************************************/
#define MAX_NB_PROCESS 256