#include <isa.h>
#include <cpu/difftest.h>
#include "../local-include/reg.h"

bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
  if (ref_r->pc != pc) {
    printf("pc is different at DUT pc = 0x%08x, REF pc = 0x%08x\n", cpu.pc, ref_r->pc);
    return false;
  }

  for (int i = 0; i < 32; i++) {
    if (!difftest_check_reg(reg_name(i, 0), cpu.pc, ref_r->gpr[i]._32, gpr(i)))
      return false;
  }
  return true;
}

void isa_difftest_attach() {
}
