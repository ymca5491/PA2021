#include <common.h>
#include "syscall.h"
void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  a[1] = c->GPR2;
  a[2] = c->GPR3;
  a[3] = c->GPR4;

  printf("type:%d\n", a[0]);
  switch (a[0]) {
    case SYS_yield: yield(); c->mepc += 4; c->GPRx = 0; break;

    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}
