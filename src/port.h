#ifndef PORT_H
#define PORT_H

#include "int.h"

void out_u8(uint16_t port, uint8_t value);

uint8_t in_u8(uint16_t port);
uint16_t in_u16(uint16_t port);

#endif
