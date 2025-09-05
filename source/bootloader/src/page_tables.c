#include <stdint.h>

#include <page_tables.h>

#include <memory/memset.h>

#include <console/print_hex.h>
#include <console/newline.h>

#include <sys/paging/load_page_table.h>

__SECTION(".page_tables") pml4t64_t pml4t;
__SECTION(".page_tables") pdpt64_t  kernel_identity_pdpt;
__SECTION(".page_tables") pdt64_t   identity_pdt;
__SECTION(".page_tables") pt64_t    identity_pt;
__SECTION(".page_tables") pdt64_t   kernel_pdt;
__SECTION(".page_tables") pt64_t    kernel_pt;

void page_tables_init(uint64_t kernel_load_point) {
    memset(&pml4t,         0, sizeof(pml4t64_t));
    memset(&kernel_identity_pdpt, 0, sizeof(pdpt64_t));
    memset(&identity_pdt,  0, sizeof(pdt64_t));
    memset(&identity_pt,   0, sizeof(pt64_t));
    memset(&kernel_pdt,    0, sizeof(pdt64_t));
    memset(&kernel_pt,     0, sizeof(pt64_t));

    PML4T64_SET_ADDRESS(pml4t[0], kernel_identity_pdpt);
    pml4t[0].present = 1;
    pml4t[0].read_write = 1;

    // identity map the first 2MB
    PDPT64_SET_ADDRESS(kernel_identity_pdpt[0], identity_pdt);
    kernel_identity_pdpt[0].present = 1;
    kernel_identity_pdpt[0].read_write = 1;
    PDT64_SET_ADDRESS(identity_pdt[0], identity_pt);
    identity_pdt[0].present = 1;
    identity_pdt[0].read_write = 1;
    pt64_map_2mb(&identity_pt, 0x00000000);

    PDPT64_SET_ADDRESS(kernel_identity_pdpt[3], kernel_pdt);
    kernel_identity_pdpt[3].present = 1;
    kernel_identity_pdpt[3].read_write = 1;
    PDT64_SET_ADDRESS(kernel_pdt[0], kernel_pt);
    kernel_pdt[0].present = 1;
    kernel_pdt[0].read_write = 1;
    pt64_map_2mb(&kernel_pt, kernel_load_point);
}

