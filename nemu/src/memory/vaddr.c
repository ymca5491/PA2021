#include <isa.h>
#include <memory/paddr.h>

word_t vaddr_ifetch(vaddr_t addr, int len) {
  printf("Ifetch bedfore tr: 0x%x", addr);
  if (isa_mmu_check(addr, len, MEM_TYPE_IFETCH) == MMU_TRANSLATE) {
    addr = isa_mmu_translate(addr, len, MEM_TYPE_IFETCH);
    printf("Ifetch after tr: 0x%x", addr);
    return paddr_read(addr, len);
  }
  else if (isa_mmu_check(addr, len, MEM_TYPE_IFETCH) == MMU_DIRECT){
    return paddr_read(addr, len);
  }
  else {
    panic("Virtual address ifetch failed\n");
  }
}

word_t vaddr_read(vaddr_t addr, int len) {
  printf("Read bedfore tr: 0x%x", addr);
  if (isa_mmu_check(addr, len, MEM_TYPE_READ) == MMU_TRANSLATE) {
    addr = isa_mmu_translate(addr, len, MEM_TYPE_READ);
    printf("Read after tr: 0x%x", addr);
    return paddr_read(addr, len);
  }
  else if (isa_mmu_check(addr, len, MEM_TYPE_READ) == MMU_DIRECT){
    return paddr_read(addr, len);
  }
  else {
    panic("Virtual address read failed\n");
  }
}

void vaddr_write(vaddr_t addr, int len, word_t data) {
  printf("Write bedfore tr: 0x%x", addr);
  if (isa_mmu_check(addr, len, MEM_TYPE_WRITE) == MMU_TRANSLATE) {
    addr = isa_mmu_translate(addr, len, MEM_TYPE_READ);
    printf("Write after tr: 0x%x", addr);
    paddr_write(addr, len, data);
  }
  else if (isa_mmu_check(addr, len, MEM_TYPE_READ) == MMU_DIRECT){
    paddr_write(addr, len, data);
  }
  else {
    panic("Virtual address write failed\n");
  }
}
