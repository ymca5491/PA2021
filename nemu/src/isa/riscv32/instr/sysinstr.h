/* mstatus 0x300, mtvec 0x305, mepc 0x341, mcause 0x342 */

def_EHelper(ecall) {
    rtl_j(s, isa_raise_intr(11, s->snpc)); // NO = 11: environment call from S-mode
}

def_EHelper(mret) {
    rtl_j(s, gpr(0x341));
}

def_EHelper(csrrw) {
    if (ddest != &(gpr(0)))
        rtl_mv(s, dsrc2, ddest);
    rtl_mv(s, dsrc1, dsrc2);
}

def_EHelper(csrrs) {
    rtl_mv(s, dsrc2, ddest);
    if (ddest != &(gpr(0)))
        rtl_and(s, dsrc1, dsrc2, dsrc2);
}
