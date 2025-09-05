#include <gdt64.h>

#include <sys/idt/idt64.h>
#include <sys/idt/idt_ptr.h>
#include <sys/asm/ltr.h>

__NORETURN void gpf_handler64(void);
__NORETURN void pf_handler64(void);
__NORETURN void unhandled_handler64(void);

extern char stack_top;

idt64_t idt64;

tss64_t tss;

idt_ptr_t idt64_ptr = DEFINE_IDT_POINTER(idt64);

void idt64_init(void) {
    uint64_t tss_base = (uint64_t) &tss;
    uint32_t tss_limit = sizeof(tss64_t);

    gdt64.tss.base_upper = tss_base >> 24;
    gdt64.tss.base_lower = tss_base;

    gdt64.tss.limit_upper = tss_limit >> 16;
    gdt64.tss.limit_lower = tss_limit;

    gdt64.tss.access =
        GDT_TSS_ACCESS_TSS64 |
        GDT_TSS_ACCESS_PRESENT |
        GDT_TSS_ACCESS_PRIVILEGE_LEVEL(0);

    gdt64.tss.flags = GDT_FLAGS_SIZE;

    tss.rsp0 = (uint64_t) &stack_top;
    tss.ist1 = (uint64_t) &stack_top;

    idt64.div0                     = DEFINE_IDT64_ENTRY_INTERRUPT(0x10, unhandled_handler64);
    idt64.nmi                      = DEFINE_IDT64_ENTRY_INTERRUPT(0x10, unhandled_handler64);
    idt64.bp_int3                  = DEFINE_IDT64_ENTRY_INTERRUPT(0x10, unhandled_handler64);
    idt64.ovf                      = DEFINE_IDT64_ENTRY_INTERRUPT(0x10, unhandled_handler64);
    idt64.bound_range              = DEFINE_IDT64_ENTRY_INTERRUPT(0x10, unhandled_handler64);
    idt64.invalid_opcode           = DEFINE_IDT64_ENTRY_INTERRUPT(0x10, unhandled_handler64);
    idt64.device_not_avail         = DEFINE_IDT64_ENTRY_INTERRUPT(0x10, unhandled_handler64);
    idt64.double_fault             = DEFINE_IDT64_ENTRY_INTERRUPT(0x10, unhandled_handler64);
    idt64.coproc_segment_overrun   = DEFINE_IDT64_ENTRY_INTERRUPT(0x10, unhandled_handler64);
    idt64.invalid_tss              = DEFINE_IDT64_ENTRY_INTERRUPT(0x10, unhandled_handler64);
    idt64.segment_not_present      = DEFINE_IDT64_ENTRY_INTERRUPT(0x10, unhandled_handler64);
    idt64.stack_segment_fault      = DEFINE_IDT64_ENTRY_INTERRUPT(0x10, unhandled_handler64);
    idt64.general_protection_fault = DEFINE_IDT64_ENTRY_INTERRUPT(0x10, unhandled_handler64);
    idt64.page_fault               = DEFINE_IDT64_ENTRY_INTERRUPT(0x10, unhandled_handler64);
    idt64.x87_fpu_error            = DEFINE_IDT64_ENTRY_INTERRUPT(0x10, unhandled_handler64);
    idt64.alignment_check          = DEFINE_IDT64_ENTRY_INTERRUPT(0x10, unhandled_handler64);
    idt64.machine_check            = DEFINE_IDT64_ENTRY_INTERRUPT(0x10, unhandled_handler64);
    idt64.simd_fp_error            = DEFINE_IDT64_ENTRY_INTERRUPT(0x10, unhandled_handler64);

    idt64.general_protection_fault = DEFINE_IDT64_ENTRY_INTERRUPT(0x10, gpf_handler64);
    idt64.page_fault = DEFINE_IDT64_ENTRY_INTERRUPT(0x10, pf_handler64);
}