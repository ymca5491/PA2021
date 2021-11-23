#include <isa.h>
#include <../local-include/reg.h>
word_t isa_raise_intr(word_t NO, vaddr_t epc) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * Then return the address of the interrupt/exception vector.
   */

  /* mstatus 0x300, mtvec 0x305, mepc 0x341, mcause 0x342 */
  gpr(0x341) = epc << 2;
  gpr(0x342) = NO;
  return gpr(0x305);
}

word_t isa_query_intr() {
  return INTR_EMPTY;
}
