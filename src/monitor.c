#include "monitor.h"
#include "port.h"

static struct monitor mtr;

static inline uint8_t vga_attrib(uint8_t bg, uint8_t fg) {
    return (bg << 4) | (fg & 0x0f);
}

static inline uint16_t vga_char(uint8_t c, uint8_t attrib) {
    return c | (attrib << 8);
}

void monitor_enable_cursor(uint8_t cursor_start, uint8_t cursor_end) {
    out_u8(0x3D4, 0x0A);
    out_u8(0x3D5, (in_u8(0x3D5) & 0xC0) | cursor_start);
 
    out_u8(0x3D4, 0x0B);
    out_u8(0x3D5, (in_u8(0x3D5) & 0xE0) | cursor_end);
}

void monitor_update_cursor() {
    uint16_t cursor_loc = mtr.y * 80 + mtr.x;

    /* We are going to set the high cursor byte */
    out_u8(0x3D4, 14);

    /* Send the high cursor byte */
    out_u8(0x3D5, cursor_loc >> 8);

    /* Tell the VGA board we are going to set the low byte */
    out_u8(0x3D4, 15);

    /* Send the low cursor byte */
    out_u8(0x3D5, cursor_loc);
}

void monitor_scroll() {
    uint8_t attrib = vga_attrib(VGA_BLACK, VGA_WHITE);
    uint16_t space = vga_char(0x20, attrib);

    if (mtr.y >= 25) {
        for (int i = 0; i < 24 * 80; ++i) {
            mtr.framebuffer[i] = mtr.framebuffer[i + 80];
        }

        for (int i = 24 * 80; i < 25 * 80; ++i) {
            mtr.framebuffer[i] = space;
        }

        mtr.y = 24;
    }
}

void monitor_init() {
    mtr.framebuffer = (uint16_t *)MONITOR_START_ADDR;
    mtr.x = 0;
    mtr.y = 0;

    // Make it a fat box
    monitor_enable_cursor(0, 15);
}

void monitor_putch(char c) {
    uint8_t bg = VGA_BLACK;
    uint8_t fg = VGA_WHITE;

    uint8_t attrib = vga_attrib(bg, fg);

    /* Backspace */
    if (c == 0x08 && mtr.x) {
        --mtr.x;
    }
    else if (c == 0x09) {
        mtr.x = (mtr.x + 8) & ~(8 - 1);
    }
    else if (c == '\r') {
        mtr.x = 0;
    }
    else if (c == '\n') {
        mtr.x = 0;
        ++mtr.y;
    }
    else if (c >= ' ') {
        uint16_t *loc = mtr.framebuffer + (mtr.y * 80 + mtr.x);
        *loc = vga_char(c, attrib);
        ++mtr.x;
    }

    if (mtr.x >= 80) {
        mtr.x = 0;
        ++mtr.y;
    }

    monitor_scroll();
    monitor_update_cursor();
}


void monitor_putbase10(int i) {
    char buffer[10] = { 0 };
    int c = 0;
    for (int num = i; num; num /= 10, ++c) {
        buffer[c] = num % 10;
    }

    for (int i = 0; i < c; ++i) {
        monitor_putch('0' + buffer[c - i - 1]);
    }
}

void monitor_putbase16(int i) {
    
}

void monitor_clear() {
    uint8_t attrib = vga_attrib(VGA_BLACK, VGA_WHITE);
    uint16_t space = vga_char(0x20, attrib);

    for (int i = 0; i < 80 * 25; ++i) {
        mtr.framebuffer[i] = space;
    }

    mtr.x = 0;
    mtr.y = 0;

    monitor_update_cursor();
}

void monitor_write(char *c) {
    for (int i = 0; c[i]; ++i) {
        monitor_putch(c[i]);
    }
}
