def_EHelper(jal) {
  rtl_li(s, ddest, s->snpc);
  rtl_addi(s, s0, &s->pc, id_src1->simm);
  rtl_jr(s, s0);
}

def_EHelper(jalr) {
  rtl_addi(s, s0, dsrc1, id_src2->simm);
  rtl_andi(s, s0, s0, 0xfffffffe);
  rtl_li(s, ddest, s->snpc);
  rtl_jr(s, s0);
}