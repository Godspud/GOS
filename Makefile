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
ISO = $(BUILD_DIR)/os.iso
LOG_FILE = build.log

# Auto-detect all C files
C_SOURCES = $(shell find $(SRC_DIR) -name '*.c')
C_OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(C_SOURCES))

# Auto-detect all ASM files
ASM_SOURCES = $(shell find . -maxdepth 1 -name '*.asm')
ASM_OBJ = $(patsubst %.asm,$(BUILD_DIR)/%.o,$(ASM_SOURCES))

# All object files
OBJECTS = $(ASM_OBJ) $(C_OBJECTS)

.DEFAULT_GOAL := all

# -------------------------------
# Build Rules
# -------------------------------

all: $(ISO)

# Build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Assemble ASM files
$(BUILD_DIR)/%.o: %.asm | $(BUILD_DIR)
	$(AS) $(ASFLAGS) $< -o $@

# Compile C files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Link kernel
$(KERNEL): $(OBJECTS) linker.ld
	$(LD) $(LDFLAGS) -T linker.ld -o $@ $(OBJECTS)

# Create bootable ISO
$(ISO): $(KERNEL)
	mkdir -p $(ISO_DIR)/boot/grub
	cp $(KERNEL) $(ISO_DIR)/boot/kernel.bin
	cp grub.cfg $(ISO_DIR)/boot/grub/grub.cfg
	grub-mkrescue -o $@ $(ISO_DIR)

# Run Bochs
run: $(ISO)
	qemu-system-i386 -cdrom build/os.iso -boot d -m 32 -vga std -serial stdio 

run_debug: $(ISO)
	qemu-system-i386 -cdrom build/os.iso -boot d -m 32 -vga std -no-reboot -d int,cpu_reset 2>&1 | tee $(LOG_FILE)

# -------------------------------
# Git integration
# -------------------------------

git: all
	@echo "=== Git push: $$(date) ==="
	@git add .
	@-git diff --cached --quiet || git commit -m "Update: $$(date +'%Y-%m-%d %H:%M')"
	@git push origin testing

# -------------------------------
# Clean
# -------------------------------

clean:
	rm -rf $(BUILD_DIR) $(ISO_DIR)