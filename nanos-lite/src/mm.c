#include <memory.h>
#include <proc.h>

static void *pf = NULL;

void* new_page(size_t nr_page) {
  void *old = pf;
  pf += nr_page * PGSIZE;
  return old;
}

#ifdef HAS_VME
static void* pg_alloc(int n) {
  size_t nr_page = n / PGSIZE;
  void *ret = new_page(nr_page);
  memset(ret, 0, n);
  return ret;
}
#endif

void free_page(void *p) {
  panic("not implement yet");
}

/* The brk() system call handler. */
int mm_brk(uintptr_t brk) {
  uintptr_t last_brk = (current->max_brk - 1) & 0xfffff000;
  if (brk > current->max_brk) current->max_brk = brk;
  //uintptr_t old = brk;
  brk = (brk - 1) & 0xfffff000;
  if (brk > last_brk) {
    int nrpg = (brk - last_brk) >> 12;
    //printf("last_brk: 0x%x max_brk: 0x%x new_brk: 0x%x nrpg: %d \n", last_brk, old, brk, nrpg);
    last_brk += PGSIZE;
    uintptr_t ppg = (uintptr_t)new_page(nrpg);
    for (int i = 0; i < nrpg; i++) {
      //printf("Mapping 0x%p to 0x%p\n", last_brk, ppg);
      map(&current->as, (void *)last_brk, (void *)ppg, 0);
      last_brk += PGSIZE;
      ppg += PGSIZE;
    }
  }
  return 0;
}

void init_mm() {
  pf = (void *)ROUNDUP(heap.start, PGSIZE);
  Log("free physical pages starting from %p", pf);

#ifdef HAS_VME
  vme_init(pg_alloc, free_page);
#endif
}
