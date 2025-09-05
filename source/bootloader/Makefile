BL_SOURCE_DIR=$(SOURCE_DIR)/bootloader
BL_OD_32=$(OBJ_DIR)/32/bootloader/src
BL_OD_64=$(OBJ_DIR)/64/bootloader/src

INCLUDE_DIRS+=$(CURDIR)/include

OBJS+=$(BL_OD_32)/bootloader_entry.o

OBJS+=$(BL_OD_32)/interrupt/idt.o
OBJS+=$(BL_OD_32)/interrupt/init.o

OBJS+=$(BL_OD_32)/interrupt/null_handler.o
OBJS+=$(BL_OD_32)/interrupt/fatal_handler_entry.o
OBJS+=$(BL_OD_32)/interrupt/null_pic_handler_entry.o
OBJS+=$(BL_OD_32)/interrupt/null_pic_handler.o
OBJS+=$(BL_OD_32)/interrupt/keyboard_handler_entry.o
OBJS+=$(BL_OD_32)/interrupt/keyboard_handler.o
OBJS+=$(BL_OD_32)/interrupt/timer_handler.o
OBJS+=$(BL_OD_32)/interrupt/timer_handler_entry.o
OBJS+=$(BL_OD_32)/interrupt/keyboard_lut.o

OBJS+=$(BL_OD_32)/pic/remap.o
OBJS+=$(BL_OD_32)/pic/mask.o

OBJS+=$(BL_OD_32)/boot_error.o

OBJS+=$(BL_OD_32)/console/console.o
OBJS+=$(BL_OD_32)/console/update.o
OBJS+=$(BL_OD_32)/console/newline.o
OBJS+=$(BL_OD_32)/console/print.o
OBJS+=$(BL_OD_32)/console/print_color.o
OBJS+=$(BL_OD_32)/console/printn.o
OBJS+=$(BL_OD_32)/console/print_hex.o
OBJS+=$(BL_OD_32)/console/print_dec.o
OBJS+=$(BL_OD_32)/console/print_sdec.o
OBJS+=$(BL_OD_32)/console/print_bool.o
OBJS+=$(BL_OD_32)/console/put.o
OBJS+=$(BL_OD_32)/console/backspace.o
OBJS+=$(BL_OD_32)/console/clear.o
OBJS+=$(BL_OD_32)/console/clear_color.o
OBJS+=$(BL_OD_32)/console/color_lut.o
OBJS+=$(BL_OD_32)/console/set_color.o
OBJS+=$(BL_OD_32)/console/init.o

OBJS+=$(BL_OD_32)/shell/shell_entry.o
OBJS+=$(BL_OD_32)/shell/keyboard_handler.o
OBJS+=$(BL_OD_32)/shell/input.o
OBJS+=$(BL_OD_32)/shell/command_tree.o
OBJS+=$(BL_OD_32)/shell/execute.o
OBJS+=$(BL_OD_32)/shell/timer_handler.o
OBJS+=$(BL_OD_32)/shell/blinker.o
OBJS+=$(BL_OD_32)/shell/history.o

OBJS+=$(BL_OD_32)/commands/echo.o
OBJS+=$(BL_OD_32)/commands/exit.o
OBJS+=$(BL_OD_32)/commands/boot.o
OBJS+=$(BL_OD_32)/commands/clear.o
OBJS+=$(BL_OD_32)/commands/color.o
OBJS+=$(BL_OD_32)/commands/fibonacci.o
OBJS+=$(BL_OD_32)/commands/calc.o
OBJS+=$(BL_OD_32)/commands/lsmem.o
OBJS+=$(BL_OD_32)/commands/gtn.o
OBJS+=$(BL_OD_32)/commands/help.o
OBJS+=$(BL_OD_32)/commands/version.o
OBJS+=$(BL_OD_32)/commands/tree.o
OBJS+=$(BL_OD_32)/commands/cat.o
OBJS+=$(BL_OD_32)/commands/reset.o
OBJS+=$(BL_OD_32)/commands/ls.o
OBJS+=$(BL_OD_32)/commands/wmem.o
OBJS+=$(BL_OD_32)/commands/stat.o
OBJS+=$(BL_OD_32)/commands/touch.o
OBJS+=$(BL_OD_32)/commands/af.o
OBJS+=$(BL_OD_32)/commands/rm.o
OBJS+=$(BL_OD_32)/commands/ed.o
OBJS+=$(BL_OD_32)/commands/disc.o
OBJS+=$(BL_OD_32)/commands/cp.o
OBJS+=$(BL_OD_32)/commands/less.o
OBJS+=$(BL_OD_32)/commands/lsdisc.o

OBJS+=$(BL_OD_32)/string/strcmp.o
OBJS+=$(BL_OD_32)/string/strcmpn.o
OBJS+=$(BL_OD_32)/string/strlen.o
OBJS+=$(BL_OD_32)/string/stoi.o
OBJS+=$(BL_OD_32)/string/strcpy.o
OBJS+=$(BL_OD_32)/string/itoa.o

OBJS+=$(BL_OD_32)/memory/memcpy.o
OBJS+=$(BL_OD_32)/memory/memset.o

OBJS+=$(BL_OD_32)/math/pow.o

OBJS+=$(BL_OD_32)/keyboard/keyboard.o
OBJS+=$(BL_OD_32)/keyboard/getch.o
OBJS+=$(BL_OD_32)/keyboard/readline.o

OBJS+=$(BL_OD_32)/gdt16.o
OBJS+=$(BL_OD_32)/gdt64.o
OBJS+=$(BL_OD_32)/bios_idt_ptr.o
OBJS+=$(BL_OD_32)/page_tables.o
OBJS+=$(BL_OD_32)/memory_map.o

OBJS+=$(BL_OD_32)/disc/reset.o
OBJS+=$(BL_OD_32)/disc/detect.o
OBJS+=$(BL_OD_32)/disc/select.o
OBJS+=$(BL_OD_32)/disc/read.o
OBJS+=$(BL_OD_32)/disc/write.o
OBJS+=$(BL_OD_32)/disc/filesystem.o
OBJS+=$(BL_OD_32)/disc/ports.o

OBJS+=$(BL_OD_32)/boot/sequence.o
OBJS+=$(BL_OD_32)/boot/disc.o
OBJS+=$(BL_OD_32)/boot/graphics_mode.o
OBJS+=$(BL_OD_32)/boot/default_kernel_path.o
OBJS+=$(BL_OD_32)/boot/64/enter_kernel.o
OBJS+=$(BL_OD_32)/boot/64/long_mode_available.o
OBJS+=$(BL_OD_32)/boot/64/error_handlers.o
OBJS+=$(BL_OD_32)/boot/64/idt64.o

OBJS+=$(BL_OD_32)/heap/heap.o
OBJS+=$(BL_OD_32)/heap/init.o
OBJS+=$(BL_OD_32)/heap/alloc.o
OBJS+=$(BL_OD_32)/heap/free.o

OPTIMIZATION=s

$(BIN_DIR)/bootloader.bin: $(OBJS) $(BL_SOURCE_DIR)/bootloader.ld
	mkdir -p $(BIN_DIR)
	$(LD32) $(LDFLAGS) -T$(BL_SOURCE_DIR)/bootloader.ld $(OBJS) -o $@ -Map=$@.map
	truncate $@ -s 32256

include $(MAKE_SCRIPTS)