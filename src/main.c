#include "int.h"
#include "monitor.h"

int main(
    struct multiboot *mboot_info) {
    (void)mboot_info;
#if 0
    unsigned int *screen = (unsigned int *)0xb8000;
    *screen = 0x2f4b2f4f;
#endif

    monitor_init();
    char hello_world[] = "Hello world\n";
    for (int i = 0; i < sizeof(hello_world) / sizeof(char); ++i) {
        monitor_put(hello_world[i]);
    }


    return 0;
}
