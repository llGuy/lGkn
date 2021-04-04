global start
global luc
    
section .multiboot_header
header_start:

    ;; Magic number
    dd 0xe85250d6

    ;; Architecture (protected mode i386)
    dd 0

    ;; Header length
    dd header_end - header_start

    ;; Checksum
    dd 0x100000000 - (0xe85250d6 + 0 + (header_end - header_start))

    ;; End tag
    dw 0
    dw 0
    dd 8

header_end: 

bits 32

section .text
start:
    ;; Print 'OK'
    mov dword [0xb8000], 0x2f4b2f4f
    hlt


