#include <isa.h>
#include <memory/paddr.h>

word_t vaddr_ifetch(vaddr_t addr, int len) {
  if (isa_mmu_check(addr, len, MEM_TYPE_IFETCH) == MMU_TRANSLATE) {
    addr = isa_mmu_translate(addr, len, MEM_TYPE_IFETCH);
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
    addr = isa_mmu_translate(addr, len, MEM_TYPE_READ);
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
    addr = isa_mmu_translate(addr, len, MEM_TYPE_READ);
    paddr_write(addr, len, data);
  }
  else if (isa_mmu_check(addr, len, MEM_TYPE_READ) == MMU_DIRECT){
    paddr_write(addr, len, data);
  }
  else {
    panic("Virtual address write failed\n");
  }
}
