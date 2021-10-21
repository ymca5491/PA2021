#include <cpu/decode.h>
#include "../local-include/rtl.h"

#define INSTR_LIST(f) f(add) f(sub) f(mul) f(and) f(or) f(xor) f(sll) f(sltu)\
                      f(addi) f(andi) f(xori) f(sltiu) f(srai)\
                      f(lui) f(auipc) \
                      f(jal) f(jalr) \
                      f(lw) f(lbu) f(sw) f(sh) f(sb)\
                      f(beq) f(bne) f(bge)\
                      f(inv) f(nemu_trap)

def_all_EXEC_ID();
