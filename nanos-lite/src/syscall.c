#include <common.h>
#include "syscall.h"
void do_syscall(Context *c) {
  uintptr_t a[4];
  uintptr_t ret = 0;
  a[0] = c->GPR1;
  a[1] = c->GPR2;
  a[2] = c->GPR3;
  a[3] = c->GPR4;

  printf("syscall type:%d, args =: %d, %d, %d\n", a[0], a[1], a[2], a[3]);
  switch (a[0]) {
    case SYS_yield: yield(); ret = 0; break;
    case SYS_exit: halt(a[1]); break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
  printf("syscall return value: %d\n", ret);
  c->GPRx = ret;
}
