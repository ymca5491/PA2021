#define concat_temp(x, y) x ## y
#define concat(x, y) concat_temp(x, y)
#define MAP(c, f) c(f)

#if __riscv_xlen == 32
#define LOAD  lw
#define STORE sw
#define XLEN  4
#else
#define LOAD  ld
#define STORE sd
#define XLEN  8
#endif

#define REGS(f) \
      f( 1)       f( 3) f( 4) f( 5) f( 6) f( 7) f( 8) f( 9) \
f(10) f(11) f(12) f(13) f(14) f(15) f(16) f(17) f(18) f(19) \
f(20) f(21) f(22) f(23) f(24) f(25) f(26) f(27) f(28) f(29) \
f(30) f(31)

#define PUSH(n) STORE concat(x, n), (n * XLEN)(sp);
#define POP(n)  LOAD  concat(x, n), (n * XLEN)(sp);

#define CONTEXT_SIZE  ((32 + 3 + 1) * XLEN)
#define OFFSET_SP     ( 2 * XLEN)
#define OFFSET_CAUSE  (32 * XLEN)
#define OFFSET_STATUS (33 * XLEN)
#define OFFSET_EPC    (34 * XLEN)
#define OFFSET_NP     (35 * XLEN)

.align 3
.globl __am_asm_trap
__am_asm_trap:
  csrrw sp, mscratch, sp     // (1) atomically exchange sp and mscratch
  bnez sp, save_context_reg_user // (2) take the branch if we trapped from user
  csrr sp, mscratch          // (3) if we trapped from kernel, restore the original sp
  
save_context_reg_kernel:
  # save regs except sp
  addi sp, sp, -CONTEXT_SIZE
  MAP(REGS, PUSH)

  # c->np = KERNEL = 0
  STORE x0, OFFSET_NP(sp)

  jal save_context_other

save_context_reg_user:
  # save regs except sp
  addi sp, sp, -CONTEXT_SIZE
  MAP(REGS, PUSH)

  # c->np = USER = 1
  li t0, 1
  STORE t0, OFFSET_NP(sp)

save_context_other:
  # c->sp = old($sp) = mscratch
  csrr t0, mscratch
  STORE t0, OFFSET_SP(sp)

  # set mscratch = 0 to support re-entry
  csrw mscratch, x0

  csrr t0, mcause
  csrr t1, mstatus
  csrr t2, mepc

  STORE t0, OFFSET_CAUSE(sp)
  STORE t1, OFFSET_STATUS(sp)
  STORE t2, OFFSET_EPC(sp)

  # set mstatus.MPRV to pass difftest
  li a0, (1 << 17)
  or t1, t1, a0
  csrw mstatus, t1

  mv a0, sp
  jal __am_irq_handle

  # load new context
  mv sp, a0

  LOAD t1, OFFSET_STATUS(sp)
  LOAD t2, OFFSET_EPC(sp)
  csrw mstatus, t1
  csrw mepc, t2

  LOAD t0, OFFSET_NP(sp)
  bnez t0, restore_user

restore_kernel:
  # restore regs except sp
  MAP(REGS, POP)

  addi sp, sp, CONTEXT_SIZE
  mret

restore_user:
  # set mscratch = c->sp
  LOAD t0, OFFSET_SP(sp)
  csrw mscratch, t0

  # restore regs except sp
  MAP(REGS, POP)

  addi sp, sp, CONTEXT_SIZE

  # mscratch = $sp, $sp = c->sp = mscratch
  csrrw sp, mscratch, sp

  mret
