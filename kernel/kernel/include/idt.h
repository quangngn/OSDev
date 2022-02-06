#pragma once

#include <stdint.h>

/*
 * By professor Charlie Curtsinger
 * src:
 * https://curtsinger.cs.grinnell.edu/teaching/2022S/CSC395/kernel/exceptions.html
 */
// Every interrupt handler must specify a code selector. We'll use entry 5
// (5*8=0x28), which is where our bootloader set up a usable code selector for
// 64-bit mode.
#define IDT_CODE_SELECTOR 0x28

// IDT entry types
#define IDT_TYPE_INTERRUPT 0xE
#define IDT_TYPE_TRAP 0xF

// Number of IDT entries
#define IDT_NUM_ENTRIES 256

// A struct the matches the layout of an IDT entry
typedef struct idt_entry {
  uint16_t offset_0;
  uint16_t selector;
  uint8_t ist : 3;
  uint8_t _unused_0 : 5;
  uint8_t type : 4;
  uint8_t _unused_1 : 1;
  uint8_t dpl : 2;
  uint8_t present : 1;
  uint16_t offset_1;
  uint32_t offset_2;
  uint32_t _unused_2;
} __attribute__((packed)) idt_entry_t;

// This struct is used to load the IDT
typedef struct idt_record {
  uint16_t size;
  void* base;
} __attribute__((packed)) idt_record_t;

/**
 * Set an interrupt handler for the given interrupt number.
 *
 * \param index The interrupt number to handle
 * \param fn    A pointer to the interrupt handler function
 * \param type  The type of interrupt handler being installed.
 *              Pass IDT_TYPE_INTERRUPT or IDT_TYPE_TRAP from above.
 */
void idt_set_handler(uint8_t index, void* fn, uint8_t type);
/**
 * Initialize an interrupt descriptor table, set handlers for standard
 * exceptions, and install the IDT.
 */
void idt_setup();

// HANDLERS
void idt_handler_div_error();
void idt_handler_db_exception();
void idt_handler_NMI_interrupt();
void idt_handler_breakpoint();
void idt_handler_overflow();
void idt_handler_BOUND_range_exceed();
void idt_handler_invalid_opcode();
void idt_handler_dev_unavailable();
void idt_handler_double_fault();
void idt_handler_coproc_seg_overrun();
void idt_handler_invalid_tss();
void idt_handler_seg_not_present();
void idt_handler_stack_seg_fault();
void idt_handler_general_proc();
void idt_handler_page_fault();
void idt_handler_x87_fpu_fp_error();
void idt_handler_alignment_check();
void idt_handler_machine_check();
void idt_handler_simd_fp_exception();
void idt_handler_vir_exception();
void idt_handler_ctrl_proc_exception();
