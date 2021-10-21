def_EHelper(beq) {
    rtl_xor(s, s0, dsrc1, dsrc2);
    if (*s0 == 0) {
        rtl_addi(s, s1, &s->pc, id_dest->simm);
        rtl_jr(s, s1);
    }
}

def_EHelper(bne) {
    rtl_xor(s, s0, dsrc1, dsrc2);
    if (*s0 != 0) {
        rtl_addi(s, s1, &s->pc, id_dest->simm);
        rtl_jr(s, s1);
    }
}