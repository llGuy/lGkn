#include "utility.h"
#include "monitor.h"

void memset(void *p, uint8_t value, uint32_t size) {
  for (uint32_t i = 0; i < size; ++i) {
    ((uint8_t *)p)[i] = value;
  }
}

void memcpy(void *dst, const void *src, uint32_t len) {
  const uint8_t *sp = (const uint8_t *)src;
  uint8_t *dp = (uint8_t *)dst;
  for (; len != 0; len--) {
    *dp++ = *sp++;
  }
}

void panic(char *msg, char *file, uint32_t line) {
  asm volatile ("cli");

  monitor_write("PANIC (");
  monitor_write(msg);
  monitor_write(") at");
  monitor_write(file);
  monitor_write(":");
  monitor_putbase10(line);
  monitor_write("\n");

  // Halt with infinite loop
  for(;;);
}

void panic_assert(char *file, uint32_t line, char *desc) {
  asm volatile ("cli");

  monitor_write("ASSERTION-FAILED (");
  monitor_write(desc);
  monitor_write(") at");
  monitor_write(file);
  monitor_write(":");
  monitor_putbase10(line);
  monitor_write("\n");

  for(;;);
}
