#include <am.h>
#include <nemu.h>
#include <klib.h>

static AddrSpace kas = {};
static void* (*pgalloc_usr)(int) = NULL;
static void (*pgfree_usr)(void*) = NULL;
static int vme_enable = 0;

static Area segments[] = {      // Kernel memory mappings
  NEMU_PADDR_SPACE
};

#define USER_SPACE RANGE(0x40000000, 0x80000000)

static inline void set_satp(void *pdir) {
  uintptr_t mode = 1ul << (__riscv_xlen - 1);
  asm volatile("csrw satp, %0" : : "r"(mode | ((uintptr_t)pdir >> 12)));
}

static inline uintptr_t get_satp() {
  uintptr_t satp;
  asm volatile("csrr %0, satp" : "=r"(satp));
  return satp << 12;
}

bool vme_init(void* (*pgalloc_f)(int), void (*pgfree_f)(void*)) {
  pgalloc_usr = pgalloc_f;
  pgfree_usr = pgfree_f;

  kas.ptr = pgalloc_f(PGSIZE);

  int i;
  for (i = 0; i < LENGTH(segments); i ++) {
    void *va = segments[i].start;
    for (; va < segments[i].end; va += PGSIZE) {
      map(&kas, va, va, 0);
    }
  }
  printf("before set satp\n");
  set_satp(kas.ptr);
  vme_enable = 1;

  return true;
}

void protect(AddrSpace *as) {
  PTE *updir = (PTE*)(pgalloc_usr(PGSIZE));
  as->ptr = updir;
  as->area = USER_SPACE;
  as->pgsize = PGSIZE;
  // map kernel space
  memcpy(updir, kas.ptr, PGSIZE);
}

void unprotect(AddrSpace *as) {
}

void __am_get_cur_as(Context *c) {
  c->pdir = (vme_enable ? (void *)get_satp() : NULL);
}

void __am_switch(Context *c) {
  if (vme_enable && c->pdir != NULL) {
    set_satp(c->pdir);
  }
}

void map(AddrSpace *as, void *va, void *pa, int prot) {
  // for Sv32, the higher 22 bit of PTE is PPN
  // PPN[1] = PTE[31:20], PPN[0] = PTE[19:10]
  // VPN[1] = va[31:22] , VPN[0] = PTE[21:12]
  // prot unused
  uintptr_t ppn, vpn1, vpn0;
  PTE *p1 = (PTE *)as->ptr, *p0;
  ppn = (uintptr_t)pa >> 12;
  vpn1 = (uintptr_t)va >> 22;
  vpn0 = ((uintptr_t)va & 0x3ff000) >> 12;
  if (p1[vpn1] == 0) {
    p1[vpn1] = (PTE)pgalloc_usr(PGSIZE) | 0x1;
  }
  p0 = (PTE *)(p1[vpn1] & 0xfffffffe);
  p0[vpn0] = ppn << 12 | 0x1;
}

Context *ucontext(AddrSpace *as, Area kstack, void *entry) {
  Context *cp = (Context *)(kstack.end - sizeof(Context));
  cp->mepc = (intptr_t)entry;
  return cp;
}
