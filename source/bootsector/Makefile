BS_SOURCE_DIR=$(SOURCE_DIR)/bootsector
BS_OD_16=$(OBJ_DIR)/16/bootsector/src
BS_OD_32=$(OBJ_DIR)/32/bootsector/src

BS_OBJS+=$(BS_OD_16)/bootsector.o
BS_OBJS+=$(BS_OD_32)/gdt32.o
BS_OBJS+=$(BS_OD_32)/memory_map.o

OPTIMIZATION=s

$(BIN_DIR)/bootsector.bin: $(BS_OBJS) $(BS_SOURCE_DIR)/bootsector.ld
	mkdir -p $(BIN_DIR)
	$(LD32) $(LDFLAGS) -T$(BS_SOURCE_DIR)/bootsector.ld $(BS_OBJS) -o $@ -Map=$@.map
	truncate $@ -s 512

include $(MAKE_SCRIPTS)