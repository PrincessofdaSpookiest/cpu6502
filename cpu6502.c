
#include "cpu6502.h"

#include <assert.h>
#include <stdio.h>

#define PUSH(value) mem_write(0x100 + regs.s--, (value))
#define POP() mem_read(0x100 + ++regs.s)

#define READ16(addr) ( mem_read(addr) + (mem_read((addr) + 1) << 8) )
#define READ16_WRAP(addr) ( mem_read(addr) + (mem_read((addr) + 1 & 255) << 8) )

CPU6502Regs regs;

#include "addrmodes.c"
#include "opcodes.c"

void cpu6502_nmi(void) {
  PUSH(regs.pc >> 8);
  PUSH(regs.pc & 255);
  PUSH(regs.p_ | 0x30);
  regs.p.i = 1;
  regs.pc = READ16(0xFFFA);
}
void cpu6502_rst(void) {
  regs.pc = READ16(0xFFFC);
}

void cpu6502_irq(void) {
  if (regs.p.i) return;

  PUSH(regs.pc >> 8);
  PUSH(regs.pc & 255);
  PUSH(regs.p_ | 0x30);
  regs.p.i = 1;
  regs.pc = READ16(0xFFFE);
}

void cpu6502_single_step() {
  uint8_t opcode = mem_read(regs.pc++);

  switch (opcode) {
    case 0x61: adc(zpxi()); break;
    case 0x65: adc(zp()); break;
    case 0x69: adc(imm()); break;
    case 0x6D: adc(abs_()); break;
    case 0x71: adc(zpiy()); break;
    case 0x75: adc(zpx()); break;
    case 0x79: adc(absy()); break;
    case 0x7D: adc(absx()); break;

    case 0x21: and(zpxi()); break;
    case 0x25: and(zp()); break;
    case 0x29: and(imm()); break;
    case 0x2D: and(abs_()); break;
    case 0x31: and(zpiy()); break;
    case 0x35: and(zpx()); break;
    case 0x39: and(absy()); break;
    case 0x3D: and(absx()); break;

    case 0x06: asl(zp()); break;
    case 0x0A: asl_a(); break;
    case 0x0E: asl(abs_()); break;
    case 0x16: asl(zpx()); break;
    case 0x1E: asl(absx()); break;

    case 0x24: bit(zp()); break;
    case 0x2C: bit(abs_()); break;

    case 0x00: brk(); break;

    case 0x10: bpl(imm()); break;
    case 0x30: bmi(imm()); break;
    case 0x50: bvc(imm()); break;
    case 0x70: bvs(imm()); break;
    case 0x90: bcc(imm()); break;
    case 0xB0: bcs(imm()); break;
    case 0xD0: bne(imm()); break;
    case 0xF0: beq(imm()); break;

    case 0x18: clc(); break;
    case 0x58: cli(); break;
    case 0xB8: clv(); break;
    case 0xD8: cld(); break;

    case 0xC1: cmp(zpxi()); break;
    case 0xC5: cmp(zp()); break;
    case 0xC9: cmp(imm()); break;
    case 0xCD: cmp(abs_()); break;
    case 0xD1: cmp(zpiy()); break;
    case 0xD5: cmp(zpx()); break;
    case 0xD9: cmp(absy()); break;
    case 0xDD: cmp(absx()); break;

    case 0xE0: cpx(imm()); break;
    case 0xE4: cpx(zp()); break;
    case 0xEC: cpx(abs_()); break;

    case 0xC0: cpy(imm()); break;
    case 0xC4: cpy(zp()); break;
    case 0xCC: cpy(abs_()); break;

    case 0xC6: dec(zp()); break;
    case 0xCE: dec(abs_()); break;
    case 0xD6: dec(zpx()); break;
    case 0xDE: dec(absx()); break;

    case 0xCA: dex(); break;

    case 0x88: dey(); break;

    case 0x41: eor(zpxi()); break;
    case 0x45: eor(zp()); break;
    case 0x49: eor(imm()); break;
    case 0x4D: eor(abs_()); break;
    case 0x51: eor(zpiy()); break;
    case 0x55: eor(zpx()); break;
    case 0x59: eor(absy()); break;
    case 0x5D: eor(absx()); break;

    case 0xE6: inc(zp()); break;
    case 0xEE: inc(abs_()); break;
    case 0xF6: inc(zpx()); break;
    case 0xFE: inc(absx()); break;

    case 0xE8: inx(); break;

    case 0xC8: iny(); break;

    case 0x4C: jmp(abs_()); break;

    case 0x6C: jmp_i(abs_()); break;

    case 0x20: jsr(abs_()); break;

    case 0xA1: lda(zpxi()); break;
    case 0xA5: lda(zp()); break;
    case 0xA9: lda(imm()); break;
    case 0xAD: lda(abs_()); break;
    case 0xB1: lda(zpiy()); break;
    case 0xB5: lda(zpx()); break;
    case 0xB9: lda(absy()); break;
    case 0xBD: lda(absx()); break;

    case 0xA2: ldx(imm()); break;
    case 0xA6: ldx(zp()); break;
    case 0xAE: ldx(abs_()); break;
    case 0xB6: ldx(zpy()); break;
    case 0xBE: ldx(absy()); break;

    case 0xA0: ldy(imm()); break;
    case 0xA4: ldy(zp()); break;
    case 0xAC: ldy(abs_()); break;
    case 0xB4: ldy(zpx()); break;
    case 0xBC: ldy(absx()); break;

    case 0x46: lsr(zp()); break;
    case 0x4A: lsr_a(); break;
    case 0x4E: lsr(abs_()); break;
    case 0x56: lsr(zpx()); break;
    case 0x5E: lsr(absx()); break;

    case 0xEA: /* NOP */ break;

    case 0x01: ora(zpxi()); break;
    case 0x05: ora(zp()); break;
    case 0x09: ora(imm()); break;
    case 0x0D: ora(abs_()); break;
    case 0x11: ora(zpiy()); break;
    case 0x15: ora(zpx()); break;
    case 0x19: ora(absy()); break;
    case 0x1D: ora(absx()); break;

    case 0x08: php(); break;
    case 0x48: pha(); break;

    case 0x28: plp(); break;
    case 0x68: pla(); break;

    case 0x26: rol(zp()); break;
    case 0x2A: rol_a(); break;
    case 0x2E: rol(abs_()); break;
    case 0x36: rol(zpx()); break;
    case 0x3E: rol(absx()); break;

    case 0x66: ror(zp()); break;
    case 0x6A: ror_a(); break;
    case 0x6E: ror(abs_()); break;
    case 0x76: ror(zpx()); break;
    case 0x7E: ror(absx()); break;

    case 0x40: rti(); break;

    case 0x60: rts(); break;

    case 0xE1: sbc(zpxi()); break;
    case 0xE5: sbc(zp()); break;
    case 0xE9: sbc(imm()); break;
    case 0xED: sbc(abs_()); break;
    case 0xF1: sbc(zpiy()); break;
    case 0xF5: sbc(zpx()); break;
    case 0xF9: sbc(absy()); break;
    case 0xFD: sbc(absx()); break;

    case 0x38: sec(); break;
    case 0x78: sei(); break;
    case 0xF8: sed(); break;

    case 0x81: sta(zpxi()); break;
    case 0x85: sta(zp()); break;
    case 0x8D: sta(abs_()); break;
    case 0x91: sta(zpiy()); break;
    case 0x95: sta(zpx()); break;
    case 0x99: sta(absy()); break;
    case 0x9D: sta(absx()); break;

    case 0x86: stx(zp()); break;
    case 0x8E: stx(abs_()); break;
    case 0x96: stx(zpy()); break;

    case 0x84: sty(zp()); break;
    case 0x8C: sty(abs_()); break;
    case 0x94: sty(zpx()); break;

    case 0x8A: txa(); break;
    case 0x98: tya(); break;
    case 0x9A: txs(); break;
    case 0xA8: tay(); break;
    case 0xAA: tax(); break;
    case 0xBA: tsx(); break;

    default: assert(0 && "unimplemented opcode");
  }
}

void cpu6502_dump_regs(FILE * f) {
  fprintf(f,
    "PC=%04X, A=%02X, X=%02X, Y=%02X, S = %02X, P = %c%c__%c%c%c%c\n",
    regs.pc,
    regs.a,
    regs.x,
    regs.y,
    regs.s,
    regs.p.n ? 'N' : 'N',
    regs.p.v ? 'V' : 'v',
    regs.p.d ? 'D' : 'd',
    regs.p.i ? 'I' : 'i',
    regs.p.z ? 'Z' : 'z',
    regs.p.c ? 'C' : 'c');
}

CPU6502Regs * cpu6502_get_regs(void) {
  return &regs;
}
