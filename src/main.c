#include "int.h"
#include "idt.h"
#include "gdt.h"
#include "monitor.h"
#include "timer.h"
#include "page.h"

typedef struct multiboot multiboot_t;

int kernel_main(
    multiboot_t *mboot_info) {
    (void)mboot_info;

    monitor_init();
    monitor_clear();
    monitor_write("Hello World\n");
    monitor_putbase10(4231);
    monitor_putch('\n');

    gdt_init();
    idt_init();
    paging_init();


    uint32_t *ptr = (uint32_t *)0xA0000000;
    uint32_t do_page_fault = *ptr;

    // timer_init(50)  --> make sure to call "sti"

    return 0;
}
