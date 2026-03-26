# Compiler & Tools
CC = gcc
AS = nasm
LD = ld
SHELL = /bin/bash

# Flags
CFLAGS = -m32 -ffreestanding -fno-stack-protector -fno-pic -nostdlib -I src/
ASFLAGS = -f elf32
LDFLAGS = -m elf_i386

# Directories
BUILD_DIR = build
ISO_DIR = iso
SRC_DIR = src

# Output files
KERNEL = $(BUILD_DIR)/kernel.bin
FLOPPY_IMG = $(BUILD_DIR)/floppy.img
LOG_FILE = build.log

# Auto-detect all C files in src/
C_SOURCES = $(shell find $(SRC_DIR) -name '*.c')
C_OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(C_SOURCES))

# Assembly object
ASM_OBJ = $(BUILD_DIR)/boot.o

# All object files
OBJECTS = $(ASM_OBJ) $(C_OBJECTS)

.DEFAULT_GOAL := log

# Default target
all: $(ISO)

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Assemble boot.asm
$(ASM_OBJ): boot.asm | $(BUILD_DIR)
	$(AS) $(ASFLAGS) $< -o $@

# Compile all C files (pattern rule)
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Link kernel.bin
$(KERNEL): $(OBJECTS) linker.ld
	$(LD) $(LDFLAGS) -T linker.ld -o $(KERNEL) $(OBJECTS)

# Create bootable floppy
$(FLOPPY_IMG): $(KERNEL)
	mkdir -p $(BUILD_DIR)
	dd if=/dev/zero of=$(FLOPPY_IMG) bs=512 count=2880
	mkfs.fat -F 12 $(FLOPPY_IMG)
	mmd -i $(FLOPPY_IMG) ::boot
	mmd -i $(FLOPPY_IMG) ::boot/grub
	mcopy -i $(FLOPPY_IMG) $(KERNEL) ::boot/kernel.bin
	mcopy -i $(FLOPPY_IMG) grub.cfg ::boot/grub/grub.cfg

# Run in Bochs
run: $(FLOPPY_IMG)
	bochs -f .bochsrc -q

# Build with output logged to build.log, then push
log:
	@echo "=== Build: $$(date) ===" >> $(LOG_FILE)
	@$(MAKE) all 2>&1 | tee -a $(LOG_FILE); exit $${PIPESTATUS[0]}
	@$(MAKE) git

# Push to git
git:
	git add .
	git commit -m "Update: $$(date +'%Y-%m-%d %H:%M')"
	git push origin testing

clean:
	rm -rf $(BUILD_DIR) $(ISO_DIR)
