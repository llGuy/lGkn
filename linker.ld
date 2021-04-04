ENTRY(start)

SECTIONS
{
    . = 1M;

    .data :
    {
        *(.data)
    }


    .text :
    {
        *(.text)
    }


}
