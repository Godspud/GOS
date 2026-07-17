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
DOCS_DIR = docs/GOS
DOC_BUILDER = $(shell pwd)/docs/doc_creator.py

# Output files
KERNEL = $(BUILD_DIR)/kernel.bin
ISO = $(BUILD_DIR)/os.iso
DISK_IMG = disk.img
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

# Run your custom OS mimicking modern Chromebook hardware
run: $(ISO) $(DISK_IMG)
	qemu-system-x86_64 \
		-cpu max \
		-smp 2 \
		-m 1024 \
		-vga std \
		-serial stdio \
		-cdrom $(ISO) \
		-boot d \
		-drive file=$(DISK_IMG),format=raw,if=ide,index=1,media=disk

# Debug target tracking interrupts, CPU resets, and outputting to log
run_debug: $(ISO) $(DISK_IMG)
	qemu-system-x86_64 \
		-cpu max \
		-smp 2 \
		-m 1024 \
		-vga std \
		-no-reboot \
		-d int,cpu_reset \
		-cdrom $(ISO) \
		-boot d \
		-drive file=$(DISK_IMG),format=raw,if=ide,index=1,media=disk 2>&1 | tee $(LOG_FILE)


$(DISK_IMG): | $(BUILD_DIR)
	qemu-img create -f raw $@ 100M

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

clean_disk:
	rm -f $(DISK_IMG)

clean_all:
	rm -rf $(BUILD_DIR) $(ISO_DIR)
	rm -f $(DISK_IMG)


.PHONY: docs
docs:
	cd $(DOCS_DIR) && npm start && cd ../../

.PHONY: docs_clean
docs_clean:
	cd $(DOCS_DIR) && npm run clear && npm start && cd ../../
