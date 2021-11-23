/* mstatus 0x300, mtvec 0x305, mepc 0x341, mcause 0x342 */

def_EHelper(ecall) {
    isa_raise_intr(11, s->snpc); // NO = 11: environment call from S-mode
    s->dnpc = gpr(0x305);
}

