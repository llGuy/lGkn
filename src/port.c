#include "port.h"

void out_u8(uint16_t port, uint8_t value) {
    /* 
       The "a" constraint says that the parameter will be in the a register 
       The "d" constraint says that the parameter will be in the d register
       The "N" constraint says that the parameter is an unsigned 8-bit constant
       (for in and out instructions)
    */
    asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

uint8_t in_u8(uint16_t port) {
    uint8_t ret;
    asm volatile ("inb %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

uint16_t in_u16(uint16_t port) {
    uint16_t ret;
    asm volatile ("inw %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}
