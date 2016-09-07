
static uint16_t imm(void) {
  return regs.pc++;
}

static uint16_t zp(void) {
  return mem_read(regs.pc++);
}

static uint16_t zpx(void) {
  return (mem_read(regs.pc++) + regs.x) & 0xFF;
}

static uint16_t zpy(void) {
  return (mem_read(regs.pc++) + regs.y) & 0xFF;
}

static uint16_t abs_(void) {
  uint16_t addr = READ16(regs.pc);
  regs.pc += 2;
  return addr;
}

static uint16_t absx(void) {
  uint16_t addr = READ16(regs.pc) + regs.x;
  regs.pc += 2;
  return addr;
}

static uint16_t absy(void) {
  uint16_t addr = READ16(regs.pc) + regs.y;
  regs.pc += 2;
  return addr;
}

static uint16_t zpi(void) {
  uint8_t temp = mem_read(regs.pc++);
  return READ16_WRAP(temp);
}

static uint16_t zpxi(void) {
  uint8_t temp = mem_read(regs.pc++) + regs.x & 255;
  return READ16_WRAP(temp);
}

static uint16_t zpiy(void) {
  uint8_t temp = mem_read(regs.pc++);
  return READ16_WRAP(temp) + regs.y;
}
