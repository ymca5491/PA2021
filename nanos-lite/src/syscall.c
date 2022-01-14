#include <common.h>
#include "syscall.h"
#include <fs.h>
#include <proc.h>

intptr_t sys_gettimeofday(void *tv, void *tz);
intptr_t sys_execve(const char *filename, char *const argv[], char *const envp[]);

//void naive_uload(PCB *pcb, const char *filename);
char *empty[] = {NULL};
int mm_brk(uintptr_t brk);
void switch_boot_pcb();
void context_uload(PCB *pcb, const char *filename, char *const argv[], char *const envp[]);

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
    case SYS_exit: halt(0);
      //sys_execve("/bin/nterm", empty, empty); ret = a[1]; break;// halt(a[1]); break;
    case SYS_open:  ret = fs_open((char*)a[1], a[2], a[3]); break;
    case SYS_read:  ret = fs_read(a[1], (void *)a[2], a[3]); break;
    case SYS_lseek: ret = fs_lseek(a[1], a[2], a[3]); break;
    case SYS_write: ret = fs_write(a[1], (void *)a[2], a[3]); break;
    case SYS_close: ret = fs_close(a[1]); break;
    case SYS_brk:   ret = mm_brk(a[1]); break;
    case SYS_gettimeofday: ret = sys_gettimeofday((void *)a[1], (void *)a[2]); break;
    case SYS_execve:ret = sys_execve((char*)a[1], (char**)a[2], (char**)a[3]); break; // return -2 when file not found
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
#ifdef STRACE
  printf("Syscall return value: %d\n", ret);
#endif
  c->GPRx = ret;
}

intptr_t sys_execve(const char *filename, char *const argv[], char *const envp[]){
  if (fs_open(filename, 0, 0) >= 0) {
    context_uload(current, filename, argv, envp);
    //printf("Pass uload\n");
    switch_boot_pcb();
    yield();
    return 0;
  }
  else {
    return -2;
  }
}

intptr_t sys_gettimeofday(void *tv, void *tz) {
  struct timeval {
    long sec;
    long usec;
  } *time = (struct timeval *)tv;
  long useconds;
  useconds = io_read(AM_TIMER_UPTIME).us;
  time->sec = useconds / 1000000;
  time->usec = useconds - 1000000 * time->sec;
  return 0;
}