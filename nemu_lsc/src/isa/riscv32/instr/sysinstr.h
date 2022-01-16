/* mstatus 0x300, mtvec 0x305, mepc 0x341, mcause 0x342 */

def_EHelper(ecall) {
    rtl_j(s, isa_raise_intr(11, s->pc)); // NO = 11: environment call from S-mode
}

def_EHelper(mret) {
    rtl_andi(s, s0, &gpr(0x300), 0x80);
    rtl_srli(s, s0, s0, 4);
    rtl_ori(s, s0, s0, 0x80);
    rtl_andi(s, s1, &gpr(0x300), 0xffffff77);
    rtl_or(s, &gpr(0x300), s0, s1);
    rtl_j(s, gpr(0x341));
}

def_EHelper(csrrw) {
    rtl_mv(s, s0, dsrc1);
    if (ddest != &(gpr(0)))
        rtl_mv(s, ddest, dsrc2);
    rtl_mv(s, dsrc2, s0);
}

def_EHelper(csrrs) {
    rtl_mv(s, s0, dsrc1);
    rtl_mv(s, ddest, dsrc2);
    if (ddest != &(gpr(0)))
        rtl_or(s, dsrc2, dsrc2, s0);
}
