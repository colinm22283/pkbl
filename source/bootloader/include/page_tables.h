#pragma once

#include <sys/paging/pml4t.h>
#include <sys/paging/pdpt.h>
#include <sys/paging/pdt.h>
#include <sys/paging/pt.h>

extern __SECTION(".page_tables") pml4t64_t pml4t;
extern __SECTION(".page_tables") pdpt64_t  kernel_identity_pdpt;
extern __SECTION(".page_tables") pdt64_t   identity_pdt;
extern __SECTION(".page_tables") pt64_t    identity_pt;
extern __SECTION(".page_tables") pdt64_t   kernel_pdt;
extern __SECTION(".page_tables") pt64_t    kernel_pt;

void page_tables_init(uint64_t kernel_load_point);