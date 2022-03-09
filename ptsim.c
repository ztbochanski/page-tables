#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MEM_SIZE 16384 // MUST equal PAGE_SIZE * PAGE_COUNT
#define PAGE_SIZE 256  // MUST equal 2^PAGE_SHIFT
#define PAGE_COUNT 64
#define PAGE_SHIFT 8 // Shift page number this much

// Simulated RAM
unsigned char mem[MEM_SIZE];

//
// Convert a page,offset into an address
//
int get_address(int page, int offset)
{
    return (page << PAGE_SHIFT) | offset;
}

//
// Initialize RAM
//
void initialize_mem(void)
{
    for (size_t i = 0; i < MEM_SIZE; ++i)
    {
        mem[i] = 0;
    }

    mem[0] = 1;
}

//
// Allocate a physical page
//
// Returns the number of the page, or 0xff if no more pages available
//
unsigned char get_page(void)
{
    for (int page_number = 0; page_number < PAGE_COUNT; page_number++)
    {

        if (mem[page_number] == 0)
        {
            mem[page_number] = 1;
            return page_number;
        }
    }
    return 0xff;
}

//
// Allocate pages for a new process
//
// This includes the new process page table and page_count data pages.
//
void new_process(int proc_num, int page_count)
{
    unsigned char physical_page_number = get_page();
    if (physical_page_number == 0xff)
    {
        printf("OOM: proc %d: page table\n", proc_num);
        exit(1);
    }
    else
    {
        mem[64 + proc_num] = physical_page_number; // store physical page in page table map
    }

    for (int i = 0; i < page_count; ++i)
    {
        unsigned char new_physical_page_for_data = get_page();
        if (new_physical_page_for_data == 0xff)
        {
            printf("OOM: proc %d: page table\n", proc_num);
            exit(1);
        }
        else
        {
            int pt_addr = get_address(physical_page_number, i);
            mem[pt_addr] = new_physical_page_for_data;
        }
    }
}

void deallocate_page(int page_number)
{
    mem[page_number] = 0;
}

//
// Print the free page map
//
void print_page_free_map(void)
{
    printf("--- PAGE FREE MAP ---\n");

    for (int i = 0; i < 64; i++)
    {
        int addr = get_address(0, i);

        printf("%c", mem[addr] == 0 ? '.' : '#');

        if ((i + 1) % 16 == 0)
            putchar('\n');
    }
}
//
// Get the page table for a given process
//
unsigned char get_page_table(int proc_num)
{
    return mem[proc_num + 64];
}

//
// Print the address map from virtual pages to physical
//
void print_page_table(int proc_num)
{
    printf("--- PROCESS %d PAGE TABLE ---\n", proc_num);

    // Get the page table for this process
    int page_table = get_page_table(proc_num);

    // Loop through, printing out used pointers
    for (int i = 0; i < PAGE_COUNT; i++)
    {
        int addr = get_address(page_table, i);

        int page = mem[addr];

        if (page != 0)
        {
            printf("%02x -> %02x\n", i, page);
        }
    }
}

//
// Main -- process command line
//
int main(int argc, char *argv[])
{
    assert(PAGE_COUNT * PAGE_SIZE == MEM_SIZE);

    if (argc == 1)
    {
        fprintf(stderr, "usage: ptsim commands\n");
        return 1;
    }

    initialize_mem();

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "np") == 0)
        {
            int proc_num = atoi(argv[++i]);
            int pages = atoi(argv[++i]);
            new_process(proc_num, pages);
        }
        else if (strcmp(argv[i], "dp") == 0)
        {
            int proc_num = atoi(argv[++i]);
            deallocate_page(proc_num);
        }
        else if (strcmp(argv[i], "pfm") == 0)
        {
            print_page_free_map();
        }
        else if (strcmp(argv[i], "ppt") == 0)
        {
            int proc_num = atoi(argv[++i]);
            print_page_table(proc_num);
        }
    }
}
