;; Tell NASM this is 16-bit mode
bits 16

;; Load at address 0x7c00
org 0x7c00
  
boot:  
  ;; Point si to hello label
  mov si, hello
  ;; 0x0e = Write Character in TTY mode
  mov ah, 0x0e
.loop:
  mov al, byte [si]
  or al, al
  jz halt
  int 0x10
  inc si
  jmp .loop
halt: 
  ;; Clear interrupts
  cli
  ;; Halt
  hlt

hello:  db "Hello World!", 0

;; Pad remaining 510 bytes with zeroes
times 510 - ($-$$) db 0
;; Magic bootloader - marks this 512 byte sector as bootable
dw 0xaa55
