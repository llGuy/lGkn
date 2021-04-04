bits 32

global gdt_flush

section .text

gdt_flush:

    ;; The pointer to the GDT, passed as a parameter
    mov eax, [esp + 4]

    ;; Actually loads the new GDT pointer
    lgdt [eax]


    ;; Set all the data segments to the offset to the data segment (0x10)
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ;; We need to do a far jump (jmp [segment offset]:[offset])
    jmp 0x08:.flush

.flush:

    ret
