export INCLUDE_DIRS+=$(SOURCE_DIR)/shared/include

include $(MAKE_SCRIPTS)

.DEFAULT: all
.PHONY: all
all: kernel modules

.PHONY: bootloader
kernel: $(BIN_DIR)/kernel.bin

include modules/module.mk

$(BIN_DIR)/kernel.bin: $(STATIC_MODULE_OBJS) $(STATIC_MODULE_HDRS) $(KN_OBJS) $(SOURCE_DIR)/kernel.ld
	mkdir -p $(@D)
	$(LD64) $(LDFLAGS) -T$(SOURCE_DIR)/kernel.ld $(KN_OBJS) $(STATIC_MODULE_OBJS) -o $@ -Map=$@.map

.PHONY: modules
modules: $(MODULE_TARGETS)
