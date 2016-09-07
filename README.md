# cpu6502
A 6502 simulator written in C

This is an simulator for the 6502 processor tested with [Peter Monta's 6502 test suite](https://github.com/pmonta/FPGA-netlist-tools/tree/master/6502-test-code).
To use this, supply your own external `mem_read` and `mem_write` functions.
Start the processor with `cpu6502_reset`, then call `cpu6502_single_step` repeatedly.

Only `cpu6502.c` needs to be compiled--it directly includes `addrmodes.c` and `opcodes.c`.

## Features:

- All documented NMOS 6502 opcodes

## To do:

- Implement 65C02 opcodes
- Add cycle count
- Further testing; decimal mode has yet to be tested.

### Special thanks to:

- [fake6502.c](http://rubbermallet.org/fake6502.c), which was very helpful in implementing the overflow check
- [Peter Monta](https://github.com/pmonta) for his test suite
