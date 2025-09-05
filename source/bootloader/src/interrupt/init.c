#include <interrupt/init.h>
#include <interrupt/idt.h>

#include <interrupt/null_handler.h>
#include <interrupt/fatal_handler_entry.h>
#include <interrupt/null_pic_handler_entry.h>
#include <interrupt/keyboard_handler_entry.h>
#include <interrupt/timer_handler_entry.h>

#include <pic/remap.h>
#include <pic/mask.h>

#include <sys/asm/lidt.h>
#include <sys/asm/sti.h>

#include <memory/memset.h>

void int_init(void) {
    pic_remap(0x20, 0x28);

    pic_mask_all();

    pic_clear_mask(0);
    pic_clear_mask(1);

    idt.div0                     = DEFINE_IDT32_ENTRY_INTERRUPT(int_fatal_handler_entry);
    idt.nmi                      = DEFINE_IDT32_ENTRY_INTERRUPT(int_fatal_handler_entry);
    idt.bp_int3                  = DEFINE_IDT32_ENTRY_INTERRUPT(int_fatal_handler_entry);
    idt.ovf                      = DEFINE_IDT32_ENTRY_INTERRUPT(int_fatal_handler_entry);
    idt.bound_range              = DEFINE_IDT32_ENTRY_INTERRUPT(int_fatal_handler_entry);
    idt.invalid_opcode           = DEFINE_IDT32_ENTRY_INTERRUPT(int_fatal_handler_entry);
    idt.device_not_avail         = DEFINE_IDT32_ENTRY_INTERRUPT(int_fatal_handler_entry);
    idt.double_fault             = DEFINE_IDT32_ENTRY_INTERRUPT(int_fatal_handler_entry);
    idt.coproc_segment_overrun   = DEFINE_IDT32_ENTRY_INTERRUPT(int_fatal_handler_entry);
    idt.invalid_tss              = DEFINE_IDT32_ENTRY_INTERRUPT(int_fatal_handler_entry);
    idt.segment_not_present      = DEFINE_IDT32_ENTRY_INTERRUPT(int_fatal_handler_entry);
    idt.stack_segment_fault      = DEFINE_IDT32_ENTRY_INTERRUPT(int_fatal_handler_entry);
    idt.general_protection_fault = DEFINE_IDT32_ENTRY_INTERRUPT(int_fatal_handler_entry);
    idt.page_fault               = DEFINE_IDT32_ENTRY_INTERRUPT(int_fatal_handler_entry);
    idt.x87_fpu_error            = DEFINE_IDT32_ENTRY_INTERRUPT(int_fatal_handler_entry);
    idt.alignment_check          = DEFINE_IDT32_ENTRY_INTERRUPT(int_fatal_handler_entry);
    idt.machine_check            = DEFINE_IDT32_ENTRY_INTERRUPT(int_fatal_handler_entry);
    idt.simd_fp_error            = DEFINE_IDT32_ENTRY_INTERRUPT(int_fatal_handler_entry);

    idt._res0 = DEFINE_IDT32_ENTRY_NULL;
    idt._res1 = DEFINE_IDT32_ENTRY_NULL;
    idt._res2 = DEFINE_IDT32_ENTRY_NULL;

    idt.mapped_irqs[0] = DEFINE_IDT32_ENTRY_INTERRUPT(int_timer_handler_entry);
    idt.mapped_irqs[1] = DEFINE_IDT32_ENTRY_INTERRUPT(int_keyboard_handler_entry);
    for (uint16_t i = 2; i < 8; i++) idt.mapped_irqs[i] = DEFINE_IDT32_ENTRY_INTERRUPT(int_null_pic1_handler_entry);
    for (uint16_t i = 8; i < 16; i++) idt.mapped_irqs[i] = DEFINE_IDT32_ENTRY_INTERRUPT(int_null_pic2_handler_entry);

    memset(idt._undefined_entries0, 0, sizeof(idt32_entry_t) * 11);
    memset(idt._undefined_entries1, 0, sizeof(idt32_entry_t) * 80);
    memset(idt._undefined_entries2, 0, sizeof(idt32_entry_t) * 127);



    lidt(&idt_ptr);
    sti();
}