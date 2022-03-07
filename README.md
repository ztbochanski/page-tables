# page-tables

🍞 simulation of page tables: the purpose of a page table in the operating system is place to store to addresses of how virtual addresses map to physical addresses.

## build
run Makefile
```bash
make
```

## usage
### commands

new process
```bash
np <process_number_argument> <pages_to_allocate_argument>
```

print free page map
```bash
pfm
``` 

print page table map
```bash
ppt <page_table_number_to_print>
``` 
store byte
```bash
sb <process_number> <address> <value_to_store>
``` 

load byte
```bash
lb <process_number> <address>
```

### examples
`./ptsim np 1 1 pfm`
`./ptsim np 1 1 np 2 2 np 3 1`

`./ptsim np 1 2 np 2 3 ppt 1 ppt 2 pfm`

### out of memory (OOM):
`./ptsim np 12 66` allocates 66 pages which is more pages than the available 64