
static void calc_nz(uint8_t result) {
  regs.p.n = (result & 0x80) != 0;
  regs.p.z = result == 0;
}

static void adc(uint16_t addr) {
  uint8_t opr = mem_read(addr);
  uint16_t result = regs.a + opr + regs.p.c;

  regs.p.c = result > 255;
  regs.p.v = ((regs.a ^ result) & (opr ^ result) & 0x80) != 0;
  regs.a = result;

  // Decimal adjust (TODO: TEST ME)
  if (regs.p.d) {
    regs.p.c = 0;
    if ((regs.a & 0x0F) > 0x09) {
      regs.a += 0x06;
    }
    if ((regs.a & 0x0F) > 0x90) {
      regs.a += 0x60;
      regs.p.c = 1;
    }
  }
  calc_nz(result);
}

static void and(uint16_t addr) {
  regs.a &= mem_read(addr);
  calc_nz(regs.a);
}

static void asl(uint16_t addr) {
  uint16_t result = mem_read(addr) << 1;
  regs.p.c = result > 255;
  calc_nz(result);
  mem_write(addr, result);
}

static void asl_a(void) {
  uint16_t result = regs.a << 1;
  regs.p.c = result > 255;
  calc_nz(regs.a);
  regs.a = result;
}

static void bit(uint16_t addr) {
  calc_nz(regs.a & mem_read(addr));
}

static void brk(void) {
  PUSH(regs.pc >> 8);
  PUSH(regs.pc & 255);
  PUSH(regs.p_ | 0x30);
  regs.p.i = 1;
  regs.pc = mem_read(0xFFFE);
  regs.pc += mem_read(0xFFFF) << 8;
}

static void bpl(uint16_t addr) {
  if (!regs.p.n) regs.pc += (int8_t)mem_read(addr);
}

static void bmi(uint16_t addr) {
  if (regs.p.n) regs.pc += (int8_t)mem_read(addr);
}

static void bvc(uint16_t addr) {
  if (!regs.p.v) regs.pc += (int8_t)mem_read(addr);
}

static void bvs(uint16_t addr) {
  if (regs.p.v) regs.pc += (int8_t)mem_read(addr);
}

static void bra(uint16_t addr) {
  regs.pc += (int8_t)mem_read(addr);
}

static void bcc(uint16_t addr) {
  if (!regs.p.c) regs.pc += (int8_t)mem_read(addr);
}

static void bcs(uint16_t addr) {
  if (regs.p.c) regs.pc += (int8_t)mem_read(addr);
}

static void bne(uint16_t addr) {
  if (!regs.p.z) regs.pc += (int8_t)mem_read(addr);
}

static void beq(uint16_t addr) {
  if (regs.p.z) regs.pc += (int8_t)mem_read(addr);
}

static void clc(void) {
  regs.p.c = 0;
}

static void cli(void) {
  regs.p.i = 0;
}

static void clv(void) {
  regs.p.v = 0;
}

static void cld(void) {
  regs.p.d = 0;
}

static void cmp(uint16_t addr) {
  uint16_t result = regs.a - mem_read(addr);

  regs.p.c = result > 255;
  calc_nz(result);
}

static void cpx(uint16_t addr) {
  uint16_t result = regs.x - mem_read(addr);

  regs.p.c = result > 255;
  calc_nz(result);
}

static void cpy(uint16_t addr) {
  uint16_t result = regs.y - mem_read(addr);

  regs.p.c = result > 255;
  calc_nz(result);
}

static void dec(uint16_t addr) {
  uint8_t result = mem_read(addr) - 1;

  calc_nz(result);
  mem_write(addr, result);
}

static void dec_a(void) {
  calc_nz(--regs.a);
}

static void dex(void) {
  calc_nz(--regs.x);
}

static void dey(void) {
  calc_nz(--regs.y);
}

static void eor(uint16_t addr) {
  regs.a ^= mem_read(addr);
  calc_nz(regs.a);
}

static void inc(uint16_t addr) {
  uint8_t result = mem_read(addr) + 1;

  calc_nz(result);
  mem_write(addr, result);
}

static void inc_a(void) {
  calc_nz(++regs.a);
}

static void inx(void) {
  calc_nz(++regs.x);
}

static void iny(void) {
  calc_nz(++regs.y);
}

static void jmp(uint16_t addr) {
  regs.pc = addr;
}

static void jmp_i(uint16_t addr) {
  regs.pc = mem_read(addr);
  regs.pc += mem_read((addr) + 1) << 8;
}

static void jsr(uint16_t addr) {
  --regs.pc;
  PUSH(regs.pc >> 8);
  PUSH(regs.pc & 255);
  regs.pc = addr;
}

static void lda(uint16_t addr) {
  regs.a = mem_read(addr);
  calc_nz(regs.a);
}

static void ldx(uint16_t addr) {
  regs.x = mem_read(addr);
  calc_nz(regs.x);
}

static void ldy(uint16_t addr) {
  regs.y = mem_read(addr);
  calc_nz(regs.y);
}

static void lsr(uint16_t addr) {
  uint8_t result = mem_read(addr);

  regs.p.c = result & 1;
  result >>= 1;
  calc_nz(result);
  mem_write(addr, result);
}

static void lsr_a() {
  regs.p.c = regs.a & 1;
  regs.a >>= 1;
  calc_nz(regs.a);
}

static void ora(uint16_t addr) {
  regs.a |= mem_read(addr);
  calc_nz(regs.a);
}

static void php(void) {
  PUSH(regs.p_ | 0x30);
}

static void pha(void) {
  PUSH(regs.a);
}

static void phx(void) {
  PUSH(regs.y);
}

static void phy(void) {
  PUSH(regs.y);
}

static void plp(void) {
  regs.p_ = POP();
}

static void pla(void) {
  regs.a = POP();
  calc_nz(regs.a);
}

static void plx(void) {
  regs.x = POP();
  calc_nz(regs.x);
}

static void ply(void) {
  regs.y = POP();
  calc_nz(regs.y);
}

static void rol(uint16_t addr) {
  uint16_t result = (mem_read(addr) << 1) | regs.p.c;

  regs.p.c = result > 255;
  calc_nz(result);
  mem_write(addr, result);
}

static void rol_a(void) {
  uint16_t result = (regs.a << 1) | regs.p.c;

  regs.p.c = result > 255;
  calc_nz(result);
  regs.a = result;
}

static void ror(uint16_t addr) {
  uint16_t result = mem_read(addr) | (regs.p.c << 8);

  regs.p.c = result & 1;
  result >>= 1;
  calc_nz(result);
  mem_write(addr, result);
}

static void ror_a(void) {
  uint16_t result = regs.a | (regs.p.c << 8);

  regs.p.c = result & 1;
  result >>= 1;
  calc_nz(result);
  regs.a = result;
}

static void rti(void) {
  regs.p_ = POP();
  regs.pc = POP();
  regs.pc += POP() << 8;
}

static void rts(void) {
  regs.pc = POP();
  regs.pc += POP() << 8;
  ++regs.pc;
}

void sbc(uint16_t addr) {
  uint8_t opr = ~mem_read(addr);
  uint16_t result = regs.a + opr + regs.p.c;

  regs.p.c = result > 255;
  regs.p.v = ((regs.a ^ result) & (opr ^ result) & 0x80) != 0;
  regs.a = result;

  // Decimal adjust (TODO: TEST ME)
  if (regs.p.d) {
    regs.p.c = 0;
    if ((regs.a & 0x0F) > 0x09) {
      regs.a += 0x06;
    }
    if ((regs.a & 0x0F) > 0x90) {
      regs.a += 0x60;
      regs.p.c = 1;
    }
  }
  calc_nz(result);
}

static void sec() {
  regs.p.c = 1;
}

static void sed() {
  regs.p.d = 1;
}

static void sei() {
  regs.p.i = 1;
}

static void sta(uint16_t addr) {
  mem_write(addr, regs.a);
}

static void stx(uint16_t addr) {
  mem_write(addr, regs.x);
}

static void sty(uint16_t addr) {
  mem_write(addr, regs.y);
}

static void stz(uint16_t addr) {
  mem_write(addr, 0);
}

static void trb(uint16_t addr) {
  uint8_t temp = mem_read(addr);
  mem_write(addr, temp & ~regs.a);
  calc_nz(temp & regs.a);
}

static void tsb(uint16_t addr) {
  uint8_t temp = mem_read(addr);
  mem_write(addr, temp | regs.a);
  calc_nz(temp & regs.a);
}

static void txa(void) {
  regs.a = regs.x;
  calc_nz(regs.a);
}

static void tya(void) {
  regs.a = regs.y;
  calc_nz(regs.a);
}

static void txs(void) {
  regs.s = regs.x;
  calc_nz(regs.s);
}
static void tay(void) {
  regs.y = regs.a;
  calc_nz(regs.y);
}
static void tax(void) {
  regs.x = regs.a;
  calc_nz(regs.x);
}
static void tsx(void) {
  regs.x = regs.s;
  calc_nz(regs.x);
}
