#include <proc.h>
#include <elf.h>
#include <fs.h>
#include <memory.h>

#ifdef __LP64__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif

static uintptr_t loader(PCB *pcb, const char *filename) {
  //TODO();
  Elf_Ehdr head;
  Elf_Phdr phdr;
  uintptr_t vpg = 0, offset;
  void *ppg = NULL, *paddr;
  int fd = fs_open(filename, 0, 0);
  fs_read(fd, &head, sizeof(Elf_Ehdr));
  assert(*(uint32_t *)head.e_ident == 0x464c457f);
  for (int i = 0; i < head.e_phnum; i++) {
    assert(fs_lseek(fd, head.e_phoff + i*head.e_phentsize, SEEK_SET) >= 0);
    fs_read(fd, &phdr, head.e_phentsize);
    if (phdr.p_type == PT_LOAD) {
      assert(fs_lseek(fd, phdr.p_offset, SEEK_SET) >= 0);
      printf("load paddr = 0x%x, vaddr = 0x%x", phdr.p_paddr, phdr.p_vaddr);
      if ((phdr.p_paddr & 0xfffff000) > vpg) {
        vpg = phdr.p_paddr & 0xfffff000;
        ppg = new_page(1);
        printf("Mapping 0x%p to 0x%p\n", (void *)vpg, ppg);
        map(&pcb->as, (void *)vpg, ppg, 0);
      }
      offset = phdr.p_paddr & 0xfff;
      paddr = (void *)((uintptr_t)ppg | offset);
      fs_read(fd, paddr, phdr.p_filesz);
      for (char *p = (char *)paddr + phdr.p_filesz; p != (char *)paddr + phdr.p_memsz; p++) {
        *p = 0;
      }
    }
  }
  fs_close(fd);
  return head.e_entry;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}

void context_kload(PCB *pcb, void (*entry)(), void *arg) {
  Area kstack = {.start = (void *)pcb, .end = (void *)pcb + sizeof(PCB)};
  pcb->cp = kcontext(kstack, entry, arg);
}

void context_uload(PCB *pcb, const char *filename, char *const argv[], char *const envp[]) {
  protect(&pcb->as); // apply for space
  void *st_top = new_page(8) + (PGSIZE << 3);  // 32kb for user stack
  void *pa = st_top, *va = pcb->as.area.end ;
  for (int i = 0; i < 8; i++) {
    pa -= PGSIZE;
    va -= PGSIZE;
    map(&pcb->as, va, pa, 0);
  }
  
  char *buf[64];
  uint32_t c = 0, size;
  int argc;

  for (int i = 0; argv[i] != NULL; i++) {
    //printf("argv[%d] = %s", i, argv[i]);
    size = strlen(argv[i]) + 1;
    st_top -= size;
    buf[c++] = memcpy(st_top, argv[i], size);
  }
  argc = c;
  buf[c++] = NULL;
  //printf("Pass argv\n");

  for (int i = 0; envp[i] != NULL; i++) {
    //printf("envp[%d] = %s\n", i, envp[i]);
    size = strlen(envp[i]) + 1;
    st_top -= size;
    buf[c++] = memcpy(st_top, envp[i], size);
  }
  buf[c++] = NULL;

  //printf("Pass envp\n");
  size = c * sizeof(char *);
  st_top -= size;
  memcpy(st_top, buf, size);

  st_top -= sizeof(int);
  *(int *)st_top = argc;

  uintptr_t entry = loader(pcb, filename);
  Area kstack = {.start = (void *)pcb, .end = (void *)pcb + sizeof(PCB)};
  pcb->cp = ucontext(&pcb->as, kstack, (void (*)())entry);

  pcb->cp->GPRx = (uintptr_t)st_top;

}

