
#include <stdio.h>
#include <stdint.h>

extern uint8_t mem_read(uint16_t addr);
extern void mem_write(uint16_t addr, uint8_t val);

typedef struct {
  uint8_t a, x, y, s;
  uint16_t pc;

  union {
    uint8_t p_;
    struct {
      uint8_t c : 1;
      uint8_t z : 1;
      uint8_t i : 1;
      uint8_t d : 1;
      uint8_t : 2;
      uint8_t v : 1;
      uint8_t n : 1;
    } p;
  };
} CPU6502Regs;

void cpu6502_nmi(void);
void cpu6502_rst(void);
void cpu6502_irq(void);

void cpu6502_single_step(void);

void cpu6502_dump_regs(FILE * f);
CPU6502Regs * cpu6502_get_regs(void);
