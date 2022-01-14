#include <isa.h>
#include <memory/paddr.h>
#include <memory/vaddr.h>
#define satp cpu.gpr[0x180]._32
paddr_t isa_mmu_translate(vaddr_t vaddr, int len, int type) {
  uintptr_t vpn1, vpn0, offset;
  paddr_t p1 = satp << 10, p0, paddr;
  offset = vaddr & 0xfff;
  vpn1 = (uintptr_t)vaddr >> 22;
  vpn0 = ((uintptr_t)vaddr & 0x3ff000) >> 12;
  p0 = paddr_read(p1 + vpn1, sizeof(paddr_t));
  Assert((p0 & 0x1) == 1, "Level 1 page table isn't valid\n");
  p0 &= 0xfffffffe;
  paddr = paddr_read(p0 + vpn0, sizeof(paddr_t));
  Assert((paddr & 0x1) == 1, "Level 0 page table isn't valid\n");
  paddr = (paddr & 0xfffff000) | offset;
  Assert(paddr == vaddr, "va != pa/n");
  printf("after translation!\n");
  return paddr;
}

int isa_mmu_check(vaddr_t vaddr, int len, int type) {
  return satp >> 31 ? MMU_TRANSLATE: MMU_DIRECT;
}
