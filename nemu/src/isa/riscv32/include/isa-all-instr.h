#include <cpu/decode.h>
#include "../local-include/rtl.h"

#define INSTR_LIST(f) f(add) f(sub) f(or) f(xor) f(sltu)\
                      f(addi) f(sltiu) f(srai)\
                      f(lui) f(auipc) \
                      f(jal) f(jalr) \
                      f(lw) f(sw) f(sh)\
                      f(beq) f(bne)\
                      f(inv) f(nemu_trap)

def_all_EXEC_ID();
