#include <cpu/decode.h>
#include "../local-include/rtl.h"

#define INSTR_LIST(f) f(add) f(sub) \
                      f(mul) f(div) f(rem) \
                      f(and) f(or) f(xor) \
                      f(sll) f(sltu) \
                      f(addi) f(andi) f(xori) \
                      f(slt) f(sltiu) \
                      f(srai) \
                      f(lui) f(auipc) \
                      f(jal) f(jalr) \
                      f(lw) f(lh) f(lbu) \
                      f(sw) f(sh) f(sb) \
                      f(beq) f(bne) f(bge) f(blt)\
                      f(inv) f(nemu_trap)

def_all_EXEC_ID();
