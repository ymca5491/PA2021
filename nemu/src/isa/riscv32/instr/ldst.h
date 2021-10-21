def_EHelper(lw) {
  rtl_lm(s, ddest, dsrc1, id_src2->imm, 4);
}

def_EHelper(lbu) {
  rtl_lm(s, s0, dsrc1, id_src2->imm, 1);
  rtl_zext(s, ddest, s0, 1);
}

def_EHelper(sw) {
  rtl_sm(s, ddest, dsrc1, id_src2->imm, 4);
}

def_EHelper(sh) {
  rtl_andi(s, s0, ddest, 0xffff);
  rtl_sm(s, s0, dsrc1, id_src2->imm, 2);
}

def_EHelper(sb) {
  rtl_andi(s, s0, ddest, 0xff);
  rtl_sm(s, s0, dsrc1, id_src2->imm, 1);
}