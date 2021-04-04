#include "int.h"
#include "monitor.h"

typedef struct multiboot multiboot_t;

int main(
    multiboot_t *mboot_info) {
    (void)mboot_info;

    monitor_init();
    monitor_clear();
    monitor_write("Hello World\n");
    monitor_putbase10(4231);

    return 0;
}
