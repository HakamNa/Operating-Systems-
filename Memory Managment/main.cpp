#include <iostream>
#include "sim_mem.h"


char main_memory[MEMORY_SIZE];
int main() {
    sim_mem mem_sm((char*) "exec_file.txt", (char*) "swap_file.txt" ,10, 10, 5,10, 7, 5);

    mem_sm.store(10, 'a');
    mem_sm.load(15);
    mem_sm.print_memory();
    mem_sm.print_swap();
    mem_sm.print_page_table();

    mem_sm.store(30, 'b');
    mem_sm.store(25, 'c');
    mem_sm.print_memory();
    mem_sm.print_swap();
    mem_sm.print_page_table();

    mem_sm.load(5);
    mem_sm.load(10);
    mem_sm.print_memory();
    mem_sm.print_swap();
    mem_sm.print_page_table();

    return 0;
}