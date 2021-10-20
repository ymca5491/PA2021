def_EHelper(jal) {
  rtl_li(s, ddest, s->snpc);
  rtl_j(s, s->pc + id_src1->simm);
}