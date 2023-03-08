#ifndef EX7_HAKAM_FSFILE_H
#define EX7_HAKAM_FSFILE_H
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

using namespace std;

#define DISK_SIZE 256


class FsFile {
int file_size;

int block_in_use;

int index_block;

int block_size;
public:
    FsFile(int _block_size);
    int getfile_size();
    void setfile_size(int fileSize);
    int getblock_in_use();
    void setblock_in_use(int blockinuse);
    int getblocksize();
    void setblocksize(int blocksize);
    int getindexblock();
    void setindexblock(int indexblock);
};
#endif //EX7_HAKAM_FSFILE_H
