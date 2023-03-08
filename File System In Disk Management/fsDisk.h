#ifndef EX7_HAKAM_FSDISK_H
#define EX7_HAKAM_FSDISK_H
#include <iostream>
#include <vector>
#include <map>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "FileDescriptor.h"
using namespace std;
#define DISK_SIZE 256
class fsDisk {
    FILE *sim_disk_fd;
    bool is_formated;
    // BitVector - "bit" (int) vector, indicate which block in the disk is free

    //              or not.  (i.e. if BitVector[0] == 1 , means that the

    //             first block is occupied.
    int BitVectorSize;

    int *BitVector;

    // filename and one fsFile.



    // MainDir;
    // Unix directories are lists of association structures,
    // each of which contains one filename and one inode number.
    map<string , FileDescriptor*>  MainDir ;

    // OpenFileDescriptors --  when you open a file,

    // the operating system creates an entry to represent that file

    // This entry number is the file descriptor.



    // OpenFileDescriptors;
    // OpenFileDescriptors --  when you open a file,
    // the operating system creates an entry to represent that file
    // This entry number is the file descriptor.
    map<int,FileDescriptor* > OpenFileDescriptors;



    int block_size;

    int maxSize;

    int freeBlocks;


public:
    // ------------------------------------------------------------------------
    fsDisk() ;
    // ------------------------------------------------------------------------
    ~fsDisk();
    // ------------------------------------------------------------------------
    void listAll();
    // ------------------------------------------------------------------------
    void fsFormat( int blockSize);
    // ------------------------------------------------------------------------
    int CreateFile(string fileName);
    // ------------------------------------------------------------------------
    int OpenFile(string fileName);
    // ------------------------------------------------------------------------
    string CloseFile(int fd);
    // ------------------------------------------------------------------------
    int WriteToFile(int fd, char *buf, int len ) ;
    // ------------------------------------------------------------------------
    int DelFile( string FileName );
    // ------------------------------------------------------------------------
    int ReadFromFile(int fd, char *buf, int len );
};
#endif //EX7_HAKAM_FSDISK_H
