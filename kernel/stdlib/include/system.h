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
#define SYSCALL_GET_FRAMEBUFFER_INFO 1000
#define SYSCALL_FRAMEBUFFER_CPY 1001
#define SYSCALL_PEEK_CHAR 2000

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
#define USER_FRAMEBUFFER 0x100000000000

#define KERNEL_HEAP 0xffff900000000000
/******************************************************************************/
// I/O related
#define STD_IN 0
#define STD_OUT 1
#define STD_ERR 2

/******************************************************************************/
#define MAX_NB_PROCESS 256

/******************************************************************************/
#define FLT_MAX 3.40282e+38
#define FLT_MIN 1.17549e-38

/******************************************************************************/
#define ARGB32_WHITE 0x00FFFFFF
#define ARGB32_BLACK 0x00000000
#define ARGB32_RED 0x00FF0000
#define ARGB32_GREEN 0x0000FF00
#define ARGB32_BLUE 0x000000FF
#define ARGB32_LIGHT_RED 0x00FF6666
#define ARGB32_LIGHT_GREEN 0x0033FF99
#define ARGB32_LIGHT_BLUE 0x003399FF
#define ARGB32_MAGENTA 0x00FF00FF
#define ARGB32_CYAN 0x0000FFFF
#define ARGB32_YELLOW 0x00FFFF00
#define ARGB32_BROW 0x00CC6600
#define ARGB32_GRAY 0x00808080

typedef uint32_t color_t;
typedef uint32_t pixel_t;