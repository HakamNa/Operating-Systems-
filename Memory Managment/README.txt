Memory Management
Authored by :Hakam Nabulssi




===================Description===================

exercise that discribe a kind of simulation of the CPU, and how he contact and reach the memory,the virtual memory divided into a parts as paging (paging mechanism), as we bring the page we need according to the adress and converting the virual address into physical address to the main memory as the data we need.

the exercise based on the idea of the store,load functions that semulate the read, write operators of the CPU, and using the page table for converting the logical address into a physical address and mapping it in an orderd manner.

the page table contain :

1-valid values in the first column "V": that help us to know if the pages are valid "found in the main memory ".

2-Dirty values in the second column "D": that help us know if the page are dirty "if the page has used or get changed"

3-Permission values in the third column "P" : that help us to get the Permission to read or to write.

4- frame values in the fourth column "F" : that help us to find in which place the frame is placed in the main memory by knowing that the main memory divided into frames.

5- swap-index values in the fifth column "S" : that help us to know where the page is placed in the swap file. 

the exercise also contain a page number and the offset that help us to define the physical address that sent to the meomory unit.

i had used the linked list in this exercise for doing all the process.


===================Functions===================

1-sim_mem::sim_mem(char exe_file_name[], char swap_file_name[], int text_size, int data_size, int bss_size,int heap_stack_size, int num_of_pages, int page_size): for implementing the constructor.

2-sim_mem::~sim_mem(): for implementing the distractor

3- void sim_mem::deleteNode(Node** head_ref, int del): for deleting a node from the linked list.

4- void sim_mem::insertNode(Node** head_ref, int new_data) : for inserting a node to the linked list.

5- bool sim_mem::search(Node** head_ref, int x): for searching in the linked list if a specific index found in the linked list.

6- void sim_mem::swap_using_linked_list(int page): doing swap using linked list (sending the pager to the swap)

7- int sim_mem::write_in_swap(Node** head_ref): filling the swap (putting the file in the swap)

8- void sim_mem::print_memory():  printing the memory

9- void sim_mem::print_swap(): printing the swap

10- char sim_mem::load(int address):  bringing the page from the logical memory to physical memory

11- void sim_mem::store(int address, char value): bringing the page form logical memory to physical memory after the editing

12- void sim_mem::print_page_table() const : priting the table



==========Program Files==============

sim_mem.h : the header of the fucntion

sim_mem.cpp : contain the functions that been mentioned above

main.cpp : the main of the exercise.


=================compiling steps===================

compile: g++ sim_mem.cpp main.cpp -o main 
run: ./ex6


================= Output ===================


/home/student/CLionProjects/sim_mem/cmake-build-debug/sim_mem

 Physical memory
[a]
[P]
[P]
[P]
[P]
[c]
[c]
[c]
[c]
[c]

 Swap memory
0 - [0]	1 - [0]	2 - [0]	3 - [0]	4 - [0]	
0 - [0]	1 - [0]	2 - [0]	3 - [0]	4 - [0]	
0 - [0]	1 - [0]	2 - [0]	3 - [0]	4 - [0]	
0 - [0]	1 - [0]	2 - [0]	3 - [0]	4 - [0]	
0 - [0]	1 - [0]	2 - [0]	3 - [0]	4 - [0]	

 page table 
Valid	 Dirty	 Permission 	 Frame	 Swap index
[0]		[0]			[0]			[-1]			[-1]
[0]		[0]			[0]			[-1]			[-1]
[1]		[1]			[1]			[0]			[-1]
[1]		[0]			[1]			[1]			[-1]
[0]		[0]			[1]			[-1]			[-1]
[0]		[0]			[1]			[-1]			[-1]
[0]		[0]			[1]			[-1]			[-1]
 frame 0
 frame 1

 Physical memory
[b]
[0]
[0]
[0]
[0]
[c]
[0]
[0]
[0]
[0]

 Swap memory
0 - [a]	1 - [P]	2 - [P]	3 - [P]	4 - [P]	
0 - [0]	1 - [0]	2 - [0]	3 - [0]	4 - [0]	
0 - [0]	1 - [0]	2 - [0]	3 - [0]	4 - [0]	
0 - [0]	1 - [0]	2 - [0]	3 - [0]	4 - [0]	
0 - [0]	1 - [0]	2 - [0]	3 - [0]	4 - [0]	

 page table 
Valid	 Dirty	 Permission 	 Frame	 Swap index
[0]		[0]			[0]			[-1]			[-1]
[0]		[0]			[0]			[-1]			[-1]
[0]		[1]			[1]			[-1]			[0]
[0]		[0]			[1]			[-1]			[-1]
[0]		[0]			[1]			[-1]			[-1]
[1]		[1]			[1]			[1]			[-1]
[1]		[1]			[1]			[0]			[-1]

 Physical memory
[b]
[b]
[b]
[b]
[b]
[a]
[P]
[P]
[P]
[P]

 Swap memory
0 - [c]	1 - [0]	2 - [0]	3 - [0]	4 - [0]	
0 - [b]	1 - [0]	2 - [0]	3 - [0]	4 - [0]	
0 - [0]	1 - [0]	2 - [0]	3 - [0]	4 - [0]	
0 - [0]	1 - [0]	2 - [0]	3 - [0]	4 - [0]	
0 - [0]	1 - [0]	2 - [0]	3 - [0]	4 - [0]	

 page table 
Valid	 Dirty	 Permission 	 Frame	 Swap index
[0]		[0]			[0]			[-1]			[-1]
[1]		[0]			[0]			[0]			[-1]
[1]		[1]			[1]			[1]			[-1]
[0]		[0]			[1]			[-1]			[-1]
[0]		[0]			[1]			[-1]			[-1]
[0]		[1]			[1]			[-1]			[0]
[0]		[1]			[1]			[-1]			[1]

Process finished with exit code 0























  
