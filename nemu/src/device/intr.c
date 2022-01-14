#include <isa.h>

void dev_raise_intr() {
    cpu.INTR = cpu.gpr[0x300]._32 & 0x8;
}
