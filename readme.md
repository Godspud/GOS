## OS

## DISCLAMIER:

i used ai in hte Makefile, readme.md, linker.ld, boot.asm, grb.cfg

anways

A minimal 32-bit x86 operating system written in C and Assembly. Built from scratch for educational purposes to understand how operating systems work at the lowest level.

## Overview

MyOS is a bare-metal operating system that boots via GRUB and runs on x86 hardware or emulators like QEMU. It features a command-line interface with keyboard input, VGA text output, and a simple shell with extensible commands.

## Features

32-bit protected mode kernel\
GRUB Multiboot compliance\
VGA text mode display (80x25)\
PS/2 keyboard driver with key repeat\
Interactive shell with built-in commands\
Adjustable key repeat delay and rate\
Modular driver architecture\
Automatic Git integration for development\
Building\
Requirements\
GCC (with multilib support for 32-bit)\
NASM assembler\
GNU Make\
GRUB utilities\
QEMU (for testing)\
On Debian/Ubuntu:

sudo apt install nasm gcc-multilib make grub-pc-bin xorriso qemu-system-x86
Compile

make
This creates a bootable ISO at build/os.iso.

Run in QEMU

make run
Clean Build Files

make clean
Commit and Push

make git
This automatically commits changes and pushes to the testing branch.

## VGA Colors

The system supports 16 VGA colors:

Value Color\
0 Black\
1 Blue\
2 Green\
3 Cyan\
4 Red\
5 Magenta\
6 Brown\
7 Light Grey\
8 Dark Grey\
9 Light Blue\
10 Light green\
11 Light Cyan\
12 Light Red\
13 Light Magenta\
14 Light Brown\
15 White

## Project Structure

OS/
boot.asm Bootloader entry point\
linker.ld Kernel linker script\
grub.cfg GRUB configuration\
Makefile Build system\
readme.md The description\
src/ Source code\
src/kernel.c Main kernel entry\
src/vga.c/h VGA text driver\
src/keyboard.c/h PS/2 keyboard driver\
src/string.c/h String utilities\
src/io.h I/O port helpers\

## How It Works

Boot: GRUB loads the kernel at 1MB memory address\
Entry: Assembly bootloader sets up stack and calls C kernel\
Initialize: VGA driver, keyboard driver, and shell are initialized\
Loop: Main loop waits for keyboard input and processes commands\

## Key Repeat

Hold a key to enable auto-repeat. Adjust timing with:

delay <ms> - Time before repeat starts (default: 300ms)\
rate <ms> - Time between repeats (default: 100ms)\
Lower values = faster repeat. Higher values = slower repeat.

## TODO

# Priority 1

Add comments to all funcs Done vga.c\
Add command history with up\down arrows\
Add color command to change text color\
Add echo command to print text\
Add reboot command to restart system\
Fix clear command for proper screen reset\

# Priority 2

Add version command\
Add boot banner with version info\
Add LED control for CapsLock and NumLock\
Add arrow key support\
Add memory info command\

# Priority 3

Add timer interrupts using PIT\
Add proper GDT setup\
Add IDT and interrupt handling\
Add basic filesystem with RAM disk\
Add custom shell prompt\
Add command aliases\
Add detailed help for each command\
Add screen saver after idle period\
Add simple games like Snake or Pong\
Add PS\2 mouse support

## Development

The testing branch is the active development branch. All changes are automatically pushed there when running make.

## License

This project is for educational purposes. Feel free to use, modify, and learn from the code.

## Acknowledgments

OSDev Wiki for documentation\
James Molloy's kernel tutorials\
The broken-thorn OS development series
