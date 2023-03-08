#ifndef SIM_MEM_H
#define SIM_MEM_H
//#include <bits/stdc++.h>
#include <iostream>
#include <list>
#include <iterator>
#define MEMORY_SIZE 10

using namespace std;
extern char main_memory[MEMORY_SIZE];

class sim_mem {
    int swapfile_fd;
    int program_fd;
    int text_size;
    int data_size;
    int bss_size;
    int heap_stack_size;
    int num_of_pages;
    int page_size;
public:
    sim_mem(char exe_file_name[], char swap_file_name[], int text_size, int data_size, int bss_size,int heap_stack_size, int num_of_pages, int page_size);
    ~sim_mem();
    char load(int address);
    void store(int address, char value);
    static void print_memory();
    void print_swap() const ;
    void print_page_table() const;
    void swap_using_linked_list(int page);
    // building the linked list
    struct Node {
        int data;
        Node *prev, *next;
    };
    static void insertNode(Node **head_ref, int new_data);
    static void deleteNode(Node **head_ref, int del);
    static bool search(Node **head_ref, int x);
    int write_in_swap(Node** head_ref);
    typedef struct page_descriptor {
        int V; // valid
        int D; // dirty
        int P; // permission
        int frame; //the number of a frame if in case it is page-mapped
        int swap_index; // where the page is located in the swap file.
    }page_descriptor;

    page_descriptor *page_table;
    Node* head ; //  creating head for  linked list
    int * array_swap; // array helping in filling the swap file


};
#endif //SIM_MEM_H