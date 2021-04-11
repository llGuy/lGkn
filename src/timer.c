#include "timer.h"
#include "idt.h"
#include "monitor.h"
#include "port.h"

uint32_t tick = 0;

static void timer_callback(registers_t regs) {
  tick++;
  monitor_write("Tick: ");
  monitor_putbase10(tick);
  monitor_write("\n");
}

void timer_init(uint32_t frequency) {
  register_interrupt_handler(0x20, timer_callback);

  uint32_t divisor = 1193180 / frequency;

  out_u8(0x43, 0x36);

  uint8_t l = (uint8_t)(divisor & 0xFF);
  uint8_t h = (uint8_t)((divisor >> 8) & 0xFF);

  out_u8(0x40, l);
  out_u8(0x40, h);
}
