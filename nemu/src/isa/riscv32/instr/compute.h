def_EHelper(lui) {
  rtl_li(s, ddest, id_src1->imm);
}

def_EHelper(auipc) {
  rtl_addi(s, ddest, &s->pc, id_src1->imm);
}


def_EHelper(addi) {
  rtl_addi(s, ddest, dsrc1, id_src2->simm);
}


def_EHelper(add) {
  rtl_add(s, ddest, dsrc1, dsrc2);
}

def_EHelper(or) {
  rtl_or(s, ddest, dsrc1, dsrc2);
}

def_EHelper(xor) {
  rtl_xor(s, ddest, dsrc1, dsrc2);
}

def_EHelper(sltu) {
  if (id_src1->imm < id_src2->imm) {
    rtl_li(s, ddest, 1);
  }
  else {
    rtl_li(s, ddest, 0);
  }
}

