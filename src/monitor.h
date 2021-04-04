#ifndef MONITOR_H
#define MONITOR_H

#define MONITOR_START_ADDR 0xb8000
#define VGA_BLACK          0x0
#define VGA_BLUE           0x1
#define VGA_GREEN          0x2
#define VGA_CYAN           0x3
#define VGA_RED            0x4
#define VGA_MAGENTA        0x5
#define VGA_BROWN          0x6
#define VGA_LIGHT_GREY     0x7
#define VGA_DARK_GREY      0x8
#define VGA_LIGHT_BLUE     0x9
#define VGA_LIGHT_GREEN    0xa
#define VGA_LIGHT_CYAN     0xb
#define VGA_LIGHT_RED      0xc
#define VGA_LIGHT_MAGENTA  0xd
#define VGA_LIGHT_BROWN    0xe
#define VGA_WHITE          0xf

#include "int.h"

struct monitor {
    uint16_t *framebuffer;
    uint16_t x, y;
};

void monitor_init();
void monitor_putch(char c);
void monitor_putbase10(int i);
void monitor_putbase16(int i);
void monitor_clear();
void monitor_write(char *c);
void monitor_scroll();
void monitor_update_cursor();
void monitor_enable_cursor(uint8_t cursor_start, uint8_t cursor_end);

#endif
