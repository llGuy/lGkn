#include "int.h"
#include "idt.h"
#include "gdt.h"
#include "monitor.h"

typedef struct multiboot multiboot_t;

int main(
    multiboot_t *mboot_info) {
    (void)mboot_info;

    monitor_init();
    monitor_clear();
    monitor_write("Hello World\n");
    monitor_putbase10(4231);

    gdt_init();
    idt_init();

    asm volatile ("int $0x3");
    // asm volatile ("int $0x4");

    return 0;
}
