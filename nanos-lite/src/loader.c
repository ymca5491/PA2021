#include <proc.h>
#include <elf.h>
size_t ramdisk_read(void *buf, size_t offset, size_t len);

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
  ramdisk_read(&head, 0, sizeof(Elf_Ehdr));
  for (int i = 0; i < head.e_phnum; i++) {
    ramdisk_read(&phdr, head.e_phoff + i*head.e_phentsize, head.e_phentsize);
    if (phdr.p_type == PT_LOAD) {
      ramdisk_read((void *)phdr.p_vaddr, phdr.p_offset, phdr.p_filesz);
      for (char *p = (char *)phdr.p_vaddr + phdr.p_filesz; p != (char *)phdr.p_vaddr + phdr.p_memsz; p++) {
        *p = 0;
      }
    }
  }
  return head.e_entry;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}

