// test implemetation

#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "defs.h"

void test_kalloc() {
    char *mem = kalloc();
    memset(mem, 0, PGSIZE);
    strncpy(mem, "Hello, xv6-k210", 16);
    printf("[test]mem: %s\n", mem);
}

void ptesprint(pagetable_t pagetable, int level){
  // there are 2^9 = 512 PTEs in a page table.
  for(int i = 0; i < 512; i++){
    pte_t pte = pagetable[i];
    if(pte & PTE_V){
      for(int j = 0; j < level-1; j++)
        printf(".. ");
      printf("..%d: pte %p pa %p\n", i, pte, PTE2PA(pte));
    }
    if((pte & PTE_V) && (pte & (PTE_R|PTE_W|PTE_X)) == 0){
      // this PTE points to a lower-level page table.
      uint64 child = PTE2PA(pte);
      ptesprint((pagetable_t)child, level+1);
    }
  }
}

int vmprint(pagetable_t pagetable){

  printf("page table %p\n", pagetable);
  ptesprint(pagetable, 1);

  return 0;
}

void test_vm(pagetable_t kernel_pt) {
  printf("va: %p, pa: %p\n", KERNBASE + 0x1000, walkaddr(kernel_pt, KERNBASE + 0x1000));
  printf("va: %p, pa: %p\n", KERNBASE + 0x2000, walkaddr(kernel_pt, KERNBASE + 0x2000));
  printf("va: %p, pa: %p\n", KERNBASE + 0x3000, walkaddr(kernel_pt, KERNBASE + 0x3000));
}
