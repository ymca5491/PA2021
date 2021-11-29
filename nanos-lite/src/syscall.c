#include <common.h>
#include "syscall.h"
#include <fs.h>
//#include <nemu.h>
//#include <riscv.h>

//uintptr_t sys_gettimeofday(struct timeval *tv, struct timezone *tz);

void do_syscall(Context *c) {
  uintptr_t a[4];
  uintptr_t ret = 0;
  a[0] = c->GPR1;
  a[1] = c->GPR2;
  a[2] = c->GPR3;
  a[3] = c->GPR4;
#ifdef STRACE
  printf("Syscall type:%d, args: %d, %d, %d\n", a[0], a[1], a[2], a[3]);
#endif
  switch (a[0]) {
    case SYS_yield: yield(); ret = 0; break;
    case SYS_exit:  halt(a[1]); break;
    case SYS_open:  ret = fs_open((char*)a[1], a[2], a[3]); break;
    case SYS_read:  ret = fs_read(a[1], (void *)a[2], a[3]); break;
    case SYS_lseek: ret = fs_lseek(a[1], a[2], a[3]); break;
    case SYS_write: ret = fs_write(a[1], (void *)a[2], a[3]); break;
    case SYS_close: ret = fs_close(a[1]); break;
    case SYS_brk:   ret = 0; break;
    //case SYS_gettimeofday: ret = sys_gettimeofday((struct timeval *)a[1], (struct timezone *)a[2]); break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
#ifdef STRACE
  printf("Syscall return value: %d\n", ret);
#endif
  c->GPRx = ret;
}
/*
uintptr_t sys_gettimeofday(struct timeval *tv, struct timezone *tz) {
  long useconds;
  useconds = inl(RTC_ADDR + 4);
  useconds <<= 32;
  useconds += inl(RTC_ADDR);
  tv->tv_sec = useconds / 1000000;
  tv->tv_usec = useconds - 1000000 * tv->tv_sec;
  return 0;
}
*/