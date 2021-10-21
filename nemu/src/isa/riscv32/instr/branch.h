def_EHelper(beq) {
    rtl_addi(s, s0, &s->pc, id_dest->simm);
    rtl_jrelop(s, RELOP_EQ, dsrc1, dsrc2, *s0);
}

def_EHelper(bne) {
    rtl_addi(s, s0, &s->pc, id_dest->simm);
    rtl_jrelop(s, RELOP_NE, dsrc1, dsrc2, *s0);
}