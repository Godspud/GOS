# OS

A minimal 32-bit x86 operating system written in C and Assembly. Built from scratch for educational purposes to understand how operating systems work at the lowest level.

Overview
MyOS is a bare-metal operating system that boots via GRUB and runs on x86 hardware or emulators like QEMU. It features a command-line interface with keyboard input, VGA text output, and a simple shell with extensible commands.

Features
32-bit protected mode kernel
GRUB Multiboot compliance
VGA text mode display (80x25)
PS/2 keyboard driver with key repeat
Interactive shell with built-in commands
Adjustable key repeat delay and rate
Modular driver architecture
Automatic Git integration for development
Building
Requirements
GCC (with multilib support for 32-bit)
NASM assembler
GNU Make
GRUB utilities
QEMU (for testing)
On Debian/Ubuntu:

Copy
sudo apt install nasm gcc-multilib make grub-pc-bin xorriso qemu-system-x86
Compile

Copy
make
This creates a bootable ISO at build/os.iso.

Run in QEMU

Copy
make run
Clean Build Files

Copy
make clean
Commit and Push

Copy
make git
This automatically commits changes and pushes to the testing branch.
