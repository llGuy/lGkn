section .boot

bits 16
  
global boot
  
boot: 

  ;; Enable A20 bit
  mov ax, 0x2401
  int 0x15

  ;; Set VGA text mode 3
  mov ax, 0x3
  int 0x10
  
  mov [disk], dl
  
  ;; Read sectors
  mov ah, 0x2
  ;; Sectors to read
  mov al, 6
  mov ch, 0
  mov dh, 0
  mov cl, 2
  mov dl, [disk]
  mov bx, copy_target
  int 0x13

  cli
  
  ;; Load the GDT table
  lgdt [gdt_pointer]
  mov eax, cr0
  ;; Set the protected mode bit on special CPU register cr0
  or eax, 0x1
  mov cr0, eax
  
  mov ax, DATA_SEG
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov ss, ax

  ;; Long jump to code segment
  jmp CODE_SEG:boot2
  
gdt_start:  
  ;; Null segment
  dq 0x0
gdt_code: 
  dw 0xFFFF
  dw 0x0
  db 0x0 
  db 10011010b
  db 11001111b
  db 0x0 
gdt_data: 
  dw 0xFFFF
  dw 0x0 
  db 0x0 
  db 10010010b 
  db 11001111b
  db 0x0
gdt_end:  

gdt_pointer:  
  dw gdt_end - gdt_start 
  dd gdt_start 
disk: 
  db 0x0
CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

;; Pad remaining 510 bytes with zeroes
times 510 - ($-$$) db 0
;; Magic bootloader - marks this 512 byte sector as bootable
dw 0xaa55

bits 32
  
extern kmain

copy_target:  

boot2:  
  call kmain
  cli
  hlt

section .bss
align 4
kernel_stack_bottom:  equ $
  resb 16384                    ; 16 KB
kernel_stack_top: 
