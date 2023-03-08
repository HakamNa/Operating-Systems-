//Hakam Nabulssi


// libraries that been used in this exercise
#include <iostream>
#include <fcntl.h>
#include "sim_mem.h"
#include <unistd.h>
#include <stack>

#define MEMORY_SIZE 10
// NULL
#define Null_For_pointer nullptr
using namespace std;
// constructor
sim_mem::sim_mem(char exe_file_name[], char swap_file_name[], int text_size, int data_size, int bss_size,int heap_stack_size, int num_of_pages, int page_size) {
    this->text_size=text_size;
    this->data_size=data_size;
    this->bss_size=bss_size;
    this->heap_stack_size=heap_stack_size;
    this->num_of_pages=num_of_pages;
    this->page_size=page_size;
    int the_page_text = text_size / page_size; // number of pages of the text
    int the_swap_size = page_size * (num_of_pages - the_page_text); // size of the swap file

    // read only, the file must  exist
    if((this->program_fd = open(exe_file_name,O_RDONLY)) <= -1){
        perror( "ERROR, cannot open the executable file ");
        //
        exit(1);// failed so exit
    }
    // creat the file if  doesnt exist
    if((this->swapfile_fd= open(swap_file_name, O_CREAT| O_RDWR, 0666)) <= -1){
        perror( "ERROR, cannot open swap file ");
        exit(1);
    }
    // filling the main memory with zeros
    int i =0 ;
        while (i<MEMORY_SIZE) {
            main_memory[i] = '0';
            i++;
        }
        i=0;
    // filling the swap file  with zeros
        while(i < the_swap_size ){
            if (write(this->swapfile_fd, "0", 1) <= -1) {
                perror("ERROR, cannot write ");
                exit(EXIT_FAILURE);
            }
            i++;
        }
        // creating page table
    page_table = new page_descriptor [num_of_pages];
        //array the help us to swap
    array_swap = new int [the_swap_size/page_size];
    // definition og the page
    i=0;
    while(i<num_of_pages){
        page_table[i].D = 0;
        page_table[i].swap_index = -1;
        page_table[i].V=0;
        page_table[i].frame=-1;
        if(i < the_page_text)
            page_table[i].P = 0;
        else
            page_table[i].P = 1;
        i++;
    }
    //filling the array with -1
    i=0;
    while(i<the_swap_size/page_size){
        array_swap[i]=-1;
        i++;
    }
    head= Null_For_pointer;
}
//the distractor
sim_mem::~sim_mem() {
    close(swapfile_fd);
    close(program_fd);
    delete [] page_table;
    delete [] array_swap;
    if(head == Null_For_pointer) return;
        Node *temp = head;
        while(temp != Null_For_pointer){
            Node *prev = temp;
            temp = temp->next;
            deleteNode(&head, prev->data);
        }
    }
// node deletion method
void sim_mem::deleteNode(Node** head_ref, int del) {
    if(*head_ref == Null_For_pointer) return;
    Node* temp ;
    for(temp = *head_ref;temp!= Null_For_pointer;temp = temp->next){
        if(temp->data==del)
            break;
    }
    if (temp == Null_For_pointer) {
        return;
    }
    if (*head_ref == temp) {
        *head_ref = temp->next;
    }
    if (temp->next != Null_For_pointer) {
        temp->next->prev = temp->prev;
    }
    if (temp->prev != Null_For_pointer) {
        temp->prev->next = temp->next;
    }
    delete(temp);
}
// node inserting method
void sim_mem::insertNode(Node** head_ref, int new_data) {
    Node* new_node = new Node();
    new_node->data = new_data;
    new_node->prev = Null_For_pointer;
    new_node->next = (*head_ref);
    if ((*head_ref) != Null_For_pointer) {
        (*head_ref)->prev = new_node;
    }
    (*head_ref) = new_node;
}
// node searching method
bool sim_mem::search(Node** head_ref, int x)
{
    // Stores head Node
    Node* temp = *head_ref;
    if(temp==Null_For_pointer) return false;

    // Stores position of the integer
    // in the doubly linked list
    // Traverse the doubly linked list
    while (temp->data != x && temp->next != Null_For_pointer) {
        // Update temp
        temp = temp->next;
    }
    // If the integer not present
    // in the doubly linked list
    if (temp->data != x)
        return false;

    // If the integer present in
    // the doubly linked list
    return true;
}
// doing swap using linked list (sending the pager to the swap)
void sim_mem::swap_using_linked_list(int page)
{
    // if the page is already exist , then delete it .
if(search(&head, page)) {
    deleteNode(&head, page);
}
// inserting the new page
    insertNode(&head, page);
}
// filling the swap (putting the file in the swap)
int sim_mem::write_in_swap(Node** head_ref)
{
    int i =0;
    int the_page_text = text_size / page_size;
    int the_swap_size = page_size * (num_of_pages - the_page_text);
    char swap_text [page_size]; // handling the text i want to swap
    Node* temp;
    for(temp = *head_ref;temp->next!= Null_For_pointer;temp = temp->next);
    int the_page = temp->data;
    int frame = page_table[the_page].frame;
    if(page_table[the_page].D == 0){
        while(i<page_size){
            main_memory[page_size*frame + i] = '0';
            i++;
        }
        page_table[the_page].V = 0;
        page_table[the_page].frame =-1 ;
    }
    if(page_table[the_page].D == 1){
        while(i<page_size){
            swap_text[i]=main_memory[page_size*frame + i];
            i++;
        }
        swap_text[i]='\0';
        i=0;
        while(i<page_size){
            main_memory[page_size*frame + i] = '0';
            i++;
        }
        i=0;
        while(i<the_swap_size/page_size){
            if(array_swap[i]==-1){
                page_table[the_page].swap_index=i;
                array_swap[i]=the_page;
                page_table[the_page].frame=-1;
                page_table[the_page].V=0;
                break;
            }
            i++;
        }
        // reposition read/write file offset
        lseek(swapfile_fd,page_size*page_table[the_page].swap_index,SEEK_SET);
            if(write(swapfile_fd,swap_text,page_size)<=-1){
                perror("ERROR, cannot write ");
                return -1;
        }
    }
    deleteNode(&head, the_page);
    return frame;
}
 // printing the memory
void sim_mem::print_memory() {
    int i;
    printf("\n Physical memory\n");
    for(i = 0; i < MEMORY_SIZE; i++) {
        printf("[%c]\n", main_memory[i]);
    }
}
// printing the swap
void sim_mem::print_swap() const {
    char* str = static_cast<char *>(malloc(this->page_size * sizeof(char)));
    int i;
    printf("\n Swap memory\n");
    lseek(swapfile_fd, 0, SEEK_SET); // go to the start of the file
    while(read(swapfile_fd, str, this->page_size) == this->page_size) {
        for(i = 0; i < page_size; i++) {
            printf("%d - [%c]\t", i, str[i]);
        }
        printf("\n");
    }
    free(str);
}
// bringing the page from the logical memory to physical memory
char sim_mem::load(int address){
    int the_page=address/page_size;
    int offset= address%page_size;
    int the_physical_address = 0;
    if(the_page>=num_of_pages){
        fprintf(stderr,"ERROR, need more pages !");
        return '\0';
    }
    char tmp[page_size];
    if(page_table[the_page].V==1){
        swap_using_linked_list(the_page);
        the_physical_address= (page_table[the_page].frame * page_size) + offset;
        return main_memory[the_physical_address];
    }
    else if(page_table[the_page].D==0 && the_page>=((text_size+data_size)/page_size)){
        fprintf(stderr,"ERROR, cannot load  ");
        return '\0';
    }
    else if(page_table[the_page].D==0){
        swap_using_linked_list(the_page);
        lseek(program_fd,page_size*the_page,SEEK_SET);
        if(read(program_fd,tmp,page_size)!=page_size){
            perror("ERROR, cannot read  ");
            return '\0';
        }
        int i=0,save;
        while(i<MEMORY_SIZE){
            if(main_memory[i]=='0') break;
            i=i+page_size;
        }
        if(i!=MEMORY_SIZE){
            page_table[the_page].frame=i/page_size;
            save=i;
        } else{
            page_table[the_page].frame=write_in_swap(&head);
            save=page_table[the_page].frame*page_size;
        }
        i=save;
        int j=0;
        while (j<page_size){
            main_memory[i]=tmp[j];
            i++;
            j++;
        }
        page_table[the_page].V=1;
    }
    else if(page_table[the_page].D==1){
        swap_using_linked_list(the_page);
        int j=0,save=0,swap_size=page_size*(num_of_pages-(text_size/page_size));
        while(j<swap_size){
            if (array_swap[j]==the_page){
                save=j;
                array_swap[j]=-1;
                break;
            }
            j++;
        }
        page_table[the_page].swap_index=-1;
        lseek(swapfile_fd,page_size*save,SEEK_SET);
        if(read(swapfile_fd,tmp,page_size)!=page_size){
            perror("ERROR, cannot read  ");
            return '\0';
        }
        lseek(swapfile_fd,page_size*save,SEEK_SET);
        j=0;
        while (j<page_size){
            if(write(swapfile_fd,"0",1)<=-1){
                perror("ERROR, cannot write ");
                return '\0';
            }
            j++;
        }
        int i=0;
        while(i<MEMORY_SIZE){
            if(main_memory[i]=='0') break;
            i=i+page_size;
        }
        if(i!=MEMORY_SIZE){
            page_table[the_page].frame=i/page_size;
            save=i;
        } else{
            page_table[the_page].frame=write_in_swap(&head);
            save=page_table[the_page].frame*page_size;
        }
        i=save;
        j=0;
        while (j<page_size){
            main_memory[i]=tmp[j];
            i++;
            j++;
        }
        page_table[the_page].V=1;
    }
    return main_memory[the_physical_address];
}
// bringing the page form logical memory to physical memory after the editing
void sim_mem::store(int address, char value) {
    int the_page=address/page_size;
    if(the_page>=num_of_pages){
        fprintf(stderr,"ERROR, need more pages !");
        return;
    }
    if(page_table[the_page].P==0){
        fprintf(stderr,"ERROR, only read permission!");
        return;
    }
    int offset= address%page_size;
    int the_physical_address;
    swap_using_linked_list(the_page);
    char tmp[page_size];
    if(page_table[the_page].V==1){
        the_physical_address= (page_table[the_page].frame * page_size) + offset;
        main_memory[the_physical_address]= value;
        return;
    } else if(page_table[the_page].V==0&&page_table[the_page].D==0 && the_page>=((text_size+data_size)/page_size)){
        int i=0,save;
        while(i<MEMORY_SIZE){
            if(main_memory[i]=='0') break;
            i=i+page_size;
        }
        if(i!=MEMORY_SIZE){
            page_table[the_page].frame=i/page_size;
            save=i;
        } else{
            page_table[the_page].frame=write_in_swap(&head);
            printf(" frame %d\n",page_table[the_page].frame);
            save=page_table[the_page].frame*page_size;
        }
        i=save;
        int j=0;
        while (j<page_size){
            main_memory[i]='0';
            i++;
            j++;
        }
        page_table[the_page].D=1;
        page_table[the_page].V=1;
        the_physical_address= (page_table[the_page].frame * page_size) + offset;
        main_memory[the_physical_address]= value;
        return;
    }
    else if(page_table[the_page].D==0&&the_page<((text_size+data_size)/page_size)){
        lseek(program_fd,page_size*the_page,SEEK_SET);
        if(read(program_fd,tmp,page_size)!=page_size){
            perror("ERROR, cannot read  ");
            return;
        }
        int i=0,save;
        while(i<MEMORY_SIZE){
            if(main_memory[i]=='0') break;
            i=i+page_size;
        }
        if(i!=MEMORY_SIZE){
            page_table[the_page].frame=i/page_size;
            save=i;
        } else{
            page_table[the_page].frame=write_in_swap(&head);
            save=page_table[the_page].frame*page_size;
        }
        i=save;
        int j=0;
        while (j<page_size){
            main_memory[i]=tmp[j];
            i++;
            j++;
        }
        page_table[the_page].D=1;
        page_table[the_page].V=1;
        the_physical_address= (page_table[the_page].frame * page_size) + offset;
        main_memory[the_physical_address]= value;
    }
    else if(page_table[the_page].D==1){
        int j=0,save=0,swap_size=page_size*(num_of_pages-(text_size/page_size));
        while(j<swap_size){
            if (array_swap[j]==the_page){
                save=j;
                array_swap[j]=-1;
                break;
            }
            j++;
        }
        page_table[the_page].swap_index=-1;
        lseek(swapfile_fd,page_size*save,SEEK_SET);
        if(read(swapfile_fd,tmp,page_size)!=page_size){
            perror("ERROR, cannot read  ");
            return;
        }
        lseek(swapfile_fd,page_size*save,SEEK_SET);
        j=0;
        while (j<page_size){
            if(write(swapfile_fd,"0",1)<=-1){
                perror("ERROR, cannot write ");
                return;
            }
            j++;
        }
        int i=0;
        while(i<MEMORY_SIZE){
            if(main_memory[i]=='0') break;
            i=i+page_size;
        }
        if(i!=MEMORY_SIZE){
            page_table[the_page].frame=i/page_size;
            save=i;
        } else{
            page_table[the_page].frame=write_in_swap(&head);
            save=page_table[the_page].frame*page_size;
        }
        i=save;
        j=0;
        while (j<page_size){
            main_memory[i]=tmp[j];
            i++;
            j++;
        }
        page_table[the_page].V=1;
        the_physical_address= (page_table[the_page].frame * page_size) + offset;
        main_memory[the_physical_address]= value;
    }



}
 // print the table
void sim_mem::print_page_table() const {
    int i;
    printf("\n page table \n");
    printf("Valid\t Dirty\t Permission \t Frame\t Swap index\n");
    for (i = 0; i < num_of_pages; i++) {
        printf("[%d]\t\t[%d]\t\t\t[%d]\t\t\t[%d]\t\t\t[%d]\n",
               page_table[i].V,
               page_table[i].D,
               page_table[i].P,
               page_table[i].frame,
               page_table[i].swap_index);
    }

}
