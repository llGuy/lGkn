;; 32-bit mode
bits 32

MBOOT_HEADER_MAGIC      equ 0xe85250d6
MBOOT_I386_ARCHITECTURE equ 0

extern kernel_main
extern stack_top
global start

section .multiboot
mboot_start:

    ;; Magic number
    dd MBOOT_HEADER_MAGIC

    ;; Architecture (protected mode i386)
    dd MBOOT_I386_ARCHITECTURE

    ;; Header length
    dd mboot_end - mboot_start

    ;; Checksum
    dd 0x100000000 - (MBOOT_HEADER_MAGIC + MBOOT_I386_ARCHITECTURE + (mboot_end - mboot_start))

    ;; End tag
    dw 0
    dw 0
    dd 8

mboot_end: 

section .text
start:
    push ebx
    mov esp, stack_top
    cli                         ; Clear interrupts
    call kernel_main

    jmp $

    
