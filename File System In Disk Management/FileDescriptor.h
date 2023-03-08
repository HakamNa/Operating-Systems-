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
#include "FsFile.h"
#ifndef EX7_HAKAM_FILEDESCRIPTOR_H
#define EX7_HAKAM_FILEDESCRIPTOR_H
using namespace std;
#define DISK_SIZE 256
class FileDescriptor {
string file_name;
FsFile* fs_file;
bool inUse;
public:
    FileDescriptor(string FileName, FsFile* fsi);
    string getFileName();
    void setFileName(string FileName);
    FsFile* getfsfile();
    int getinuse();
    void setinuse(bool inuse);
};
#define DISK_SIM_FILE "DISK_SIM_FILE.txt"
#endif //EX7_HAKAM_FILEDESCRIPTOR_H
