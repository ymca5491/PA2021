/* mstatus 0x300, mtvec 0x305, mepc 0x341, mcause 0x342 */

def_EHelper(ecall) {
    rtl_j(s, isa_raise_intr(11, s->pc)); // NO = 11: environment call from S-mode
}

def_EHelper(mret) {
    rtl_j(s, gpr(0x341));
}

def_EHelper(csrrw) {
    if (ddest != &(gpr(0)))
        rtl_mv(s, ddest, dsrc2);
    rtl_mv(s, dsrc2, dsrc1);
}

def_EHelper(csrrs) {
    rtl_mv(s, ddest, dsrc2);
    if (ddest != &(gpr(0)))
        rtl_mv(s, dsrc2, dsrc1);
}
