#include <isa.h>
#include <memory/paddr.h>

word_t vaddr_ifetch(vaddr_t addr, int len) {
  if (isa_mmu_check(addr, len, MEM_TYPE_IFETCH) == MMU_TRANSLATE) {
    printf("Ifetch before tr: 0x%x\n", addr);
    addr = isa_mmu_translate(addr, len, MEM_TYPE_IFETCH);
    printf("Ifetch after tr: 0x%x\n", addr);
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
  if (isa_mmu_check(addr, len, MEM_TYPE_READ) == MMU_TRANSLATE) {
    printf("Read before tr: 0x%x\n", addr);
    addr = isa_mmu_translate(addr, len, MEM_TYPE_READ);
    printf("Read after tr: 0x%x\n", addr);
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
  if (isa_mmu_check(addr, len, MEM_TYPE_WRITE) == MMU_TRANSLATE) {
    printf("Write before tr: 0x%x\n", addr);
    addr = isa_mmu_translate(addr, len, MEM_TYPE_READ);
    printf("Write after tr: 0x%x\n", addr);
    paddr_write(addr, len, data);
  }
  else if (isa_mmu_check(addr, len, MEM_TYPE_READ) == MMU_DIRECT){
    paddr_write(addr, len, data);
  }
  else {
    panic("Virtual address write failed\n");
  }
}
