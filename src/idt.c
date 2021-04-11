#include "idt.h"
#include "port.h"
#include "monitor.h"
#include "utility.h"

static idt_entry_t idt_entries[256];
static idt_ptr_t idt_ptr;

extern void idt_flush(uint32_t);

void idt_set_entry_fields(
  uint8_t num,
  uint32_t base,
  uint16_t selector,
  uint8_t flags) {
  /* Set bits for base */
  idt_entries[num].base_low = base & 0xFFFF;
  idt_entries[num].base_high = (base >> 16) & 0xFFFF;
  /* The rest */
  idt_entries[num].segment_selector = selector;
  idt_entries[num].zero = 0;
  idt_entries[num].flags = flags /* | 0x60 when we do user-mode*/;
}

// Master
#define MPIC_COMMAND_PORT 0x20
#define MPIC_DATA_PORT (MPIC_COMMAND_PORT + 1)
// Slave
#define SPIC_COMMAND_PORT 0xA0
#define SPIC_DATA_PORT (SPIC_COMMAND_PORT + 1)


static void irq_remap() {
  /*
    Master PIC command port: 0x20
    Master PIC data port   : 0x21
    Slave PIC command port : 0xA0
    Slave PIC data port    : 0xA1
      
    By default:
    Master PIC interrupt request (IRQ) numbers 0 to 7 <=> CPU interrupts 0x08 to 0x0F
    Slave PIC interrupt request (IRQ) numbers 8 to 15 <=> CPU interrupts 0x70 to 0x77 
      
    We are going to move the IRQs of the master PIC card to the first available
    CPU interrupts numbers (32-47)
  */

  // Start initialisation sequance
  out_u8(MPIC_COMMAND_PORT, 0x11);
  out_u8(SPIC_COMMAND_PORT, 0x11);

  // Set the master PIC vector offset (starting CPU interrupt number)
  // The master will start at 32 (0x20) all the way to 39 (0x27)
  out_u8(MPIC_DATA_PORT, 0x20);
  // Set the slave PIC vector offset (starting CPU interrupt number)
  // The slave will start at 40 (0x28) and go to 47 (0x2F)
  out_u8(SPIC_DATA_PORT, 0x28);
    
  // Tell the master PIC that there is a slave PIC at IRQ2 (0, 1, >2<)
  out_u8(MPIC_DATA_PORT, 0b0100);
  out_u8(SPIC_DATA_PORT, 2); // Tell the slave its cascade identity

  // We are in 8086 mode
  out_u8(MPIC_DATA_PORT, 1);
  out_u8(SPIC_DATA_PORT, 1);

  // TODO: Why 0?
  out_u8(MPIC_DATA_PORT, 0);
  out_u8(SPIC_DATA_PORT, 0);
}

void idt_init() {
  idt_ptr.limit = sizeof(idt_entry_t) * 256 - 1;
  idt_ptr.base = (uint32_t)&idt_entries;

  memset(idt_entries, 0, sizeof(idt_entry_t) * 256);

  irq_remap();

  /* 
     Choose the kernel code segment (0x08) when fired 
     0x8E = 0b10001110
  */
  idt_set_entry_fields(0,  (uint32_t)isr0, 0x08, 0x8E);
  idt_set_entry_fields(1,  (uint32_t)isr1, 0x08, 0x8E);
  idt_set_entry_fields(2,  (uint32_t)isr2, 0x08, 0x8E);
  idt_set_entry_fields(3,  (uint32_t)isr3, 0x08, 0x8E);
  idt_set_entry_fields(4,  (uint32_t)isr4, 0x08, 0x8E);
  idt_set_entry_fields(5,  (uint32_t)isr5, 0x08, 0x8E);
  idt_set_entry_fields(6,  (uint32_t)isr6, 0x08, 0x8E);
  idt_set_entry_fields(7,  (uint32_t)isr7, 0x08, 0x8E);
  idt_set_entry_fields(8,  (uint32_t)isr8, 0x08, 0x8E);
  idt_set_entry_fields(9,  (uint32_t)isr9, 0x08, 0x8E);
  idt_set_entry_fields(10, (uint32_t)isr10, 0x08, 0x8E);
  idt_set_entry_fields(11, (uint32_t)isr11, 0x08, 0x8E);
  idt_set_entry_fields(12, (uint32_t)isr12, 0x08, 0x8E);
  idt_set_entry_fields(13, (uint32_t)isr13, 0x08, 0x8E);
  idt_set_entry_fields(14, (uint32_t)isr14, 0x08, 0x8E);
  idt_set_entry_fields(15, (uint32_t)isr15, 0x08, 0x8E);
  idt_set_entry_fields(16, (uint32_t)isr16, 0x08, 0x8E);
  idt_set_entry_fields(17, (uint32_t)isr17, 0x08, 0x8E);
  idt_set_entry_fields(18, (uint32_t)isr18, 0x08, 0x8E);
  idt_set_entry_fields(19, (uint32_t)isr19, 0x08, 0x8E);
  idt_set_entry_fields(20, (uint32_t)isr20, 0x08, 0x8E);
  idt_set_entry_fields(21, (uint32_t)isr21, 0x08, 0x8E);
  idt_set_entry_fields(22, (uint32_t)isr22, 0x08, 0x8E);
  idt_set_entry_fields(23, (uint32_t)isr23, 0x08, 0x8E);
  idt_set_entry_fields(24, (uint32_t)isr24, 0x08, 0x8E);
  idt_set_entry_fields(25, (uint32_t)isr25, 0x08, 0x8E);
  idt_set_entry_fields(26, (uint32_t)isr26, 0x08, 0x8E);
  idt_set_entry_fields(27, (uint32_t)isr27, 0x08, 0x8E);
  idt_set_entry_fields(28, (uint32_t)isr28, 0x08, 0x8E);
  idt_set_entry_fields(29, (uint32_t)isr29, 0x08, 0x8E);
  idt_set_entry_fields(30, (uint32_t)isr30, 0x08, 0x8E);
  idt_set_entry_fields(31, (uint32_t)isr31, 0x08, 0x8E);

  idt_set_entry_fields(32, (uint32_t)irq0,  0x08, 0x8E);
  idt_set_entry_fields(33, (uint32_t)irq1,  0x08, 0x8E);
  idt_set_entry_fields(34, (uint32_t)irq2,  0x08, 0x8E);
  idt_set_entry_fields(35, (uint32_t)irq3,  0x08, 0x8E);
  idt_set_entry_fields(36, (uint32_t)irq4,  0x08, 0x8E);
  idt_set_entry_fields(37, (uint32_t)irq5,  0x08, 0x8E);
  idt_set_entry_fields(38, (uint32_t)irq6,  0x08, 0x8E);
  idt_set_entry_fields(39, (uint32_t)irq7,  0x08, 0x8E);
  idt_set_entry_fields(40, (uint32_t)irq8,  0x08, 0x8E);
  idt_set_entry_fields(41, (uint32_t)irq9,  0x08, 0x8E);
  idt_set_entry_fields(42, (uint32_t)irq10,  0x08, 0x8E);
  idt_set_entry_fields(43, (uint32_t)irq11,  0x08, 0x8E);
  idt_set_entry_fields(44, (uint32_t)irq12,  0x08, 0x8E);
  idt_set_entry_fields(45, (uint32_t)irq13,  0x08, 0x8E);
  idt_set_entry_fields(46, (uint32_t)irq14,  0x08, 0x8E);
  idt_set_entry_fields(47, (uint32_t)irq15,  0x08, 0x8E);


  idt_flush((uint32_t)&idt_ptr);
}

static isr_t interrupt_handlers[256];

void isr_handler(registers_t regs) {
  if (interrupt_handlers[regs.int_no]) {
    (interrupt_handlers[regs.int_no])(regs);
  }
  else {
    monitor_write("Received interrupt: ");
    monitor_putbase10(regs.int_no);
    monitor_putch('\n');
  }
}


void register_interrupt_handler(uint8_t n, isr_t handler) {
  interrupt_handlers[n] = handler;
}

void irq_handler(registers_t regs) {
  if (regs.int_no >= 40) {
    // Send reset signal to slave
    out_u8(SPIC_COMMAND_PORT, 0x20);
  }

  // Send reset signal to master
  out_u8(MPIC_COMMAND_PORT, 0x20);

  if (interrupt_handlers[regs.int_no]) {
    (interrupt_handlers[regs.int_no])(regs);
  }
}
