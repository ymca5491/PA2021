#include <proc.h>
#include <elf.h>
#include <fs.h>

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
  int fd = fs_open(filename, 0, 0);
  fs_read(fd, &head, sizeof(Elf_Ehdr));
  assert(*(uint32_t *)head.e_ident == 0x464c457f);
  for (int i = 0; i < head.e_phnum; i++) {
    assert(fs_lseek(fd, head.e_phoff + i*head.e_phentsize, SEEK_SET) == 0);
    fs_read(fd, &phdr, head.e_phentsize);
    if (phdr.p_type == PT_LOAD) {
      assert(fs_lseek(fd, phdr.p_offset, SEEK_SET) == 0);
      fs_read(fd, (void *)phdr.p_vaddr, phdr.p_filesz);
      for (char *p = (char *)phdr.p_vaddr + phdr.p_filesz; p != (char *)phdr.p_vaddr + phdr.p_memsz; p++) {
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

