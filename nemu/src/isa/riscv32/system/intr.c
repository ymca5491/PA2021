#include <isa.h>
#include "../local-include/reg.h"
word_t isa_raise_intr(word_t NO, vaddr_t epc) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * Then return the address of the interrupt/exception vector.
   */

  /* mstatus 0x300, mtvec 0x305, mepc 0x341, mcause 0x342 */
  // mstatus MIE[3], MPIE[7]
  gpr(0x341) = epc;
  gpr(0x342) = NO;
  //gpr(0x300) = (gpr(0x300) & 0x8) << 4; // save MIE to MPIE, set MIE = 0
#ifdef CONFIG_ETRACE
  //printf("Exception occurs at pc=0x%08x, cause code: %u\n", epc, NO);
  log_write("Exception occurs at pc=0x%08x, cause code: %u\n", epc, NO);
#endif
  return gpr(0x305);
  //return 0;
}
#define IRQ_TIMER 0x80000007  // for riscv32

word_t isa_query_intr() {
  if (cpu.INTR && (gpr(0x300) & 0x8)) {
    cpu.INTR = false;
    //return IRQ_TIMER;
  }
  return INTR_EMPTY;
}
