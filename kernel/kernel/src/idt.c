#include "idt.h"

#include "kprint.h"
#include "util.h"

/*
 * By professor Charlie Curtsinger
 * src:
 * https://curtsinger.cs.grinnell.edu/teaching/2022S/CSC395/kernel/exceptions.html
 */

// Make an IDT
idt_entry_t idt[IDT_NUM_ENTRIES];

void idt_set_handler(uint8_t index, void* fn, uint8_t type) {
  // The entry is present
  idt[index].present = 1;
  // We don't use interrupt stack
  idt[index].ist = 0;
  // Run handler in kernel mode
  idt[index].dpl = 0;
  // Set type of the handler (IDT_TYPE_INTERRUPT or IDT_TYPE_TRAP)
  idt[index].type = type;

  // IDT entry offset_0 = bit 0 to 15;
  idt[index].offset_0 = (uint16_t)((uint64_t)fn & 0xFFFF);
  // IDT entry offset_1 = bit 15 to 31;
  idt[index].offset_1 = (uint16_t)((uint64_t)fn >> 16 & 0xFFFF);
  // IDT entry offset_2 = bit 32 to 63;
  idt[index].offset_2 = (uint32_t)((uint64_t)fn >> 32 & 0xFFFFFFFF);
}

void idt_setup() {
  // Zero out IDT
  kmemset(idt, 0, IDT_NUM_ENTRIES * sizeof(idt_entry_t));

  // Setup the reserved interrupt handler
  idt_set_handler(0, idt_handler_div_error, IDT_TYPE_TRAP);
  idt_set_handler(1, idt_handler_db_exception, IDT_TYPE_TRAP);
  idt_set_handler(2, idt_handler_NMI_interrupt, IDT_TYPE_INTERRUPT);
  idt_set_handler(3, idt_handler_breakpoint, IDT_TYPE_TRAP);

  idt_set_handler(4, idt_handler_overflow, IDT_TYPE_TRAP);
  idt_set_handler(5, idt_handler_BOUND_range_exceed, IDT_TYPE_TRAP);
  idt_set_handler(6, idt_handler_invalid_opcode, IDT_TYPE_TRAP);
  idt_set_handler(7, idt_handler_dev_unavailable, IDT_TYPE_TRAP);

  idt_set_handler(8, idt_handler_double_fault, IDT_TYPE_TRAP);
  idt_set_handler(10, idt_handler_invalid_tss, IDT_TYPE_TRAP);
  idt_set_handler(11, idt_handler_seg_not_present, IDT_TYPE_TRAP);
  idt_set_handler(12, idt_handler_stack_seg_fault, IDT_TYPE_TRAP);

  idt_set_handler(13, idt_handler_general_proc, IDT_TYPE_TRAP);
  idt_set_handler(14, idt_handler_page_fault, IDT_TYPE_TRAP);
  idt_set_handler(16, idt_handler_x87_fpu_fp_error, IDT_TYPE_TRAP);
  idt_set_handler(17, idt_handler_alignment_check, IDT_TYPE_TRAP);

  idt_set_handler(18, idt_handler_machine_check, IDT_TYPE_TRAP);
  idt_set_handler(19, idt_handler_simd_fp_exception, IDT_TYPE_TRAP);
  idt_set_handler(20, idt_handler_vir_exception, IDT_TYPE_TRAP);
  idt_set_handler(21, idt_handler_ctrl_proc_exception, IDT_TYPE_TRAP);

  // Step 3: Install the IDT
  idt_record_t record = {.size = sizeof(idt), .base = idt};
  __asm__("lidt %0" ::"m"(record));
}

// HANDLERS
void idt_handler_div_error() {
  kprint_s("[INT 0] Divide Error\n");
  halt();
}

void idt_handler_db_exception() {
  kprint_s("[INT 1] Debug Exception\n");
  halt();
}

void idt_handler_NMI_interrupt() {
  kprint_s("[INT 2] NMI Interrupt\n");
  halt();
}

void idt_handler_breakpoint() {
  kprint_s("[INT 3] Breakpoint\n");
  halt();
}

void idt_handler_overflow() {
  kprint_s("[INT 4] Overflow\n");
  halt();
}

void idt_handler_BOUND_range_exceed() {
  kprint_s("[INT 5] BOUND Range Exception\n");
  halt();
}
void idt_handler_invalid_opcode() {
  kprint_s("[INT 6] Invalid Opcode\n");
  halt();
}

void idt_handler_dev_unavailable() {
  kprint_s("[INT 7] Device Not Available (No Math Coprocessor)\n");
  halt();
}

void idt_handler_double_fault() {
  kprint_s("[INT 8] Double Fault\n");
  halt();
}

void idt_handler_coproc_seg_overrun() {
  kprint_s("[INT 9] Coprocessor Segment Overrun (reserved)\n");
  halt();
}
void idt_handler_invalid_tss() {
  kprint_s("[INT 10] Invalid TSS\n");
  halt();
}

void idt_handler_seg_not_present() {
  kprint_s("[INT 11] Segment Not Present\n");
  halt();
}

void idt_handler_stack_seg_fault() {
  kprint_s("[INT 12] Stack-Segment Fault\n");
}
void idt_handler_general_proc() {
  kprint_s("[INT 13] General Protection\n");
  halt();
}

void idt_handler_page_fault() {
  kprint_s("[INT 14] Page Fault\n");
  halt();
}

void idt_handler_x87_fpu_fp_error() {
  kprint_s("[INT 16] x87 FPU Floating-Point Error (Math Fault)\n");
  halt();
}

void idt_handler_alignment_check() {
  kprint_s("[INT 17] Alignment Check\n");
  halt();
}

void idt_handler_machine_check() {
  kprint_s("[INT 18] Machine Check\n");
  halt();
}

void idt_handler_simd_fp_exception() {
  kprint_s("[INT 19] SIMD Floating-Point Exception\n");
  halt();
}
void idt_handler_vir_exception() {
  kprint_s("[INT 20] Virtualization Exception\n");
  halt();
}
void idt_handler_ctrl_proc_exception() {
  kprint_s("[INT 21] Control Protection Exception\n");
  halt();
}
