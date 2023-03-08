#include "FsFile.h"
using namespace std;

#define DISK_SIZE 256

 FsFile:: FsFile(int _block_size) {
    file_size = 0;

    block_in_use = 0;

    block_size = _block_size;

    index_block = -1;

}
int FsFile::getfile_size(){
    return file_size;
}
void FsFile::setfile_size(int fileSize){
     file_size=fileSize;
 }
 int FsFile::getblock_in_use(){
     return block_in_use;
 }
 void FsFile::setblock_in_use(int blockinuse){
     block_in_use=blockinuse;
 }
 int FsFile::getblocksize(){
     return block_size;
 }
 void FsFile::setblocksize(int blocksize){
     block_size=blocksize;
 }
 int FsFile::getindexblock(){
     return index_block;
 }
 void FsFile::setindexblock(int indexblock){
     index_block=indexblock;
 }

