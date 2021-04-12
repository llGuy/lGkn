ENTRY(start)

SECTIONS
{
    . = 1M;

    .multiboot :
    {
        *(.multiboot)
    }

    .text :
    {
        *(.text)
        . = ALIGN(4096);
    }

    .data :
    {
        *(.data)
        *(.rodata)
        . = ALIGN(4096);
    }

    .bss :
    {
        *(.bss)
        . = ALIGN(4096);

        /* Initialise the stack in the .bss section */
        stack_bottom = .;
        stack_top = stack_bottom + 16384;
        . = stack_top + 16;
    }


    end = .; _end = .; __end = .;

}
