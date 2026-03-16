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

# Create bootable ISO
$(ISO): $(KERNEL)
	mkdir -p $(ISO_DIR)/boot/grub
	cp $(KERNEL) $(ISO_DIR)/boot/kernel.bin
	cp grub.cfg $(ISO_DIR)/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO) $(ISO_DIR)

# Run in QEMU
run: $(ISO)
	qemu-system-i386 -cdrom $(ISO) -m 128M

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
