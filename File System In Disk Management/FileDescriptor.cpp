#include "FileDescriptor.h"
using namespace std;

#define DISK_SIZE 256
FileDescriptor::FileDescriptor(string FileName, FsFile* fsi) {
    file_name = FileName;
    fs_file = fsi;
    inUse = true;
}
string FileDescriptor::getFileName() {
    return file_name;
}
void FileDescriptor::setFileName(string FileName){
    file_name=FileName;
}

FsFile* FileDescriptor::getfsfile() {
    return fs_file;
}
int FileDescriptor::getinuse(){
    return inUse;
}
void FileDescriptor::setinuse(bool inuse){
    inUse=inuse;
}