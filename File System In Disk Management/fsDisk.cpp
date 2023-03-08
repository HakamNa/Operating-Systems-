#include <iostream>
#include <vector>
#include <map>
#include <cassert>
#include <cstring>
#include <cmath>
#include <fcntl.h>
#include "fsDisk.h"
using namespace std;
#define DISK_SIZE 256
int TOINT(char &c) { // this method convert Binary to decimal
    int toINT = 0;
    int Const=2;
    int Const1;
    Const1 = pow(Const, 3);
    int Save,Save1,Save2;
    for (int R = 0; R < Const1; ++R) {
        Save=(c >> R);
        Save1=(1u &Save);
        Save2=pow(Const, R);
        toINT += (Save1 * Save2);
    }
    return toINT;
}
char decToBinary(int n , char &c)
{
    // array to store binary number
    int binaryNum[8];
    // counter for binary array
    int i = 0;
    while (n > 0) {
        // storing remainder in binary array
        binaryNum[i] = n % 2;
        n = n / 2;
        i++;
    }
    c=c&0;
    // printing binary array in reverse order
    for (int j = i - 1; j >= 0; j--) {
        if (binaryNum[j]==1)
            c = c | 1u << j;
    }
    return c;
}
    fsDisk::fsDisk() {

        sim_disk_fd = fopen(DISK_SIM_FILE , "w+");

        assert(sim_disk_fd);

        for (int i=0; i < DISK_SIZE ; i++) {

            int ret_val = fseek ( sim_disk_fd , i , SEEK_SET );

            ret_val = fwrite( "\0" ,  1 , 1, sim_disk_fd);

            assert(ret_val == 1);

        }

        fflush(sim_disk_fd);

        block_size = 0;

        is_formated = false;

    }
    fsDisk::~fsDisk() { // deletes the disk and the remainder of the open ojects
    for(auto it = MainDir.cbegin(); it != MainDir.cend(); ++it)
    {
        delete(it->second->getfsfile());
        delete(it->second);
    }
    MainDir.clear();
    OpenFileDescriptors.clear();
    delete [] BitVector;
    fclose(sim_disk_fd);
}
    // ------------------------------------------------------------------------
    void fsDisk::listAll() {
            int i = 0;
            for(auto it = MainDir.cbegin(); it != MainDir.cend(); ++it)
            {
                cout << "index: " << i << ": FileName: " << it->first <<  " , isInUse: " << it->second->getinuse() << endl;
                i++;
            }
            cout <<"OpenFileDescriptor"<<endl;
            for(auto it = OpenFileDescriptors.cbegin(); it != OpenFileDescriptors.cend(); ++it)
            {
                if(it->second!=NULL)
                cout << "index: " << it->first << ": FileName: " << it->second->getFileName() <<  " , isInUse: " << it->second->getinuse() << endl;
            }
            char bufy;
            cout << "Disk content: '" ;
            for (i=0; i < DISK_SIZE ; i++) {
                int ret_val = fseek ( sim_disk_fd , i , SEEK_SET );
                ret_val = fread(  &bufy , 1 , 1, sim_disk_fd );
                cout << bufy;
            }
            cout << "'" << endl;
    }
    // ------------------------------------------------------------------------
    void fsDisk::fsFormat( int blockSize) {
        if(is_formated){
            for(auto it = MainDir.cbegin(); it != MainDir.cend(); ++it)
            {
                delete(it->second->getfsfile());
                delete(it->second);
            }
            MainDir.clear();
            OpenFileDescriptors.clear();
        }
        this->is_formated = true;
        this->block_size=blockSize;
        this->BitVectorSize=DISK_SIZE/blockSize;
        this->BitVector=new int [BitVectorSize];
        freeBlocks=DISK_SIZE/blockSize;
        for(int i=0;i<BitVectorSize;i++)
        {
            BitVector[i]=0;//all blocks are free at the start
        }
        cout <<"FORMAT DISK: number of blocks: " <<(DISK_SIZE/block_size) <<endl;
        maxSize=blockSize*block_size;
    }
    // ------------------------------------------------------------------------
    int fsDisk::CreateFile(string fileName) {
        if(!is_formated)
            return -1;
        std::map<string, FileDescriptor*>::iterator it1;
        it1 = MainDir.find(fileName);//find the file we need to delete
        if(it1 != MainDir.end()){
            cout << "File already exists" << endl;
            return -1;
        }
        FsFile *fs=new FsFile(block_size);
        FileDescriptor *fd = new FileDescriptor(fileName,fs);//creat new file
        MainDir.insert(std::pair<string ,FileDescriptor*>(fileName,fd));//put file in the main directory
        int Index=0;bool ISNull=false;
        std::map<int, FileDescriptor*>::iterator it2;
        for(auto it = OpenFileDescriptors.cbegin(); it != OpenFileDescriptors.cend(); ++it)
        {
            if(it->second==NULL){
                ISNull= true;
                break;
            }
            Index++;
        }
        if(ISNull){
            it2 = OpenFileDescriptors.find(Index);
            it2->second=fd;
            return Index;
        }
        OpenFileDescriptors.insert(std::pair<int ,FileDescriptor*>(OpenFileDescriptors.size(),fd));//put file in the main directory
        return OpenFileDescriptors.size()-1;
    }
    // ------------------------------------------------------------------------
    int fsDisk::OpenFile(string fileName) {
        if(!is_formated)
            return -1;
        std::map<string, FileDescriptor*>::iterator it1;
        it1 = MainDir.find(fileName);//find the file we need to delete
        if(it1 == MainDir.end()){
            cout << "File Is Not Found" << endl;
            return -1;
        }
            if(it1->second->getinuse()){
                cout << "File Is already opened" << endl;
                return -1;
            }
            int Index=0;bool ISNull=false;
            it1->second->setinuse(true);
            std::map<int, FileDescriptor*>::iterator it2;
            for(auto it = OpenFileDescriptors.cbegin(); it != OpenFileDescriptors.cend(); ++it)
            {
                if(it->second==NULL){
                    ISNull= true;
                    break;
                }
                Index++;
            }
            if(ISNull){
                it2 = OpenFileDescriptors.find(Index);
                it2->second=it1->second;
                return Index;
            }
            OpenFileDescriptors.insert(std::pair<int ,FileDescriptor*>(OpenFileDescriptors.size(),it1->second));//put file in the main directory
            return OpenFileDescriptors.size()-1;
    }
    // ------------------------------------------------------------------------
    string fsDisk::CloseFile(int fd) {
        if(!is_formated)
            return "-1";
        if(fd>=OpenFileDescriptors.size()||fd<0){
            return "-1";
        }
        std::map<int, FileDescriptor*>::iterator it2;
        it2 = OpenFileDescriptors.find(fd);//find the file we need to delete
        if(it2->second==NULL){
            cout << "File Is already closed" << endl;
            return "-1";
        }
        if(it2->second!=NULL){
            it2->second->setinuse(false);
            it2->second=NULL;
        }
        return "1";
    }
    // ------------------------------------------------------------------------
    int fsDisk::WriteToFile(int fd, char *buf, int len ) {
        if(!is_formated)
            return -1;
        std::map<int, FileDescriptor*>::iterator it2;
        it2 = OpenFileDescriptors.find(fd);//find the file we need to delete
        if(it2 == OpenFileDescriptors.end()){
            cout << "File Is Not IN OpenFileDescriptors" << endl;
            return -1;
        }
        if(it2->second==NULL){
            cout << "File Is already closed" << endl;
            return -1;
        }
        if(block_size-it2->second->getfsfile()->getfile_size()==maxSize){
            cout << "File Is Full" << endl;
            return -1;
        }
        if(block_size*block_size-it2->second->getfsfile()->getfile_size()<len){
            len=block_size*block_size-it2->second->getfsfile()->getfile_size();
                cout << "there is no space" << endl;
        }
        char d;int Iwrite=0;int i;bool isEmpty= false;
        if(it2->second->getfsfile()->getindexblock()==-1){
            for ( i = 0; i < BitVectorSize; ++i) {
                if(BitVector[i]==0){
                    BitVector[i]=1;
                    freeBlocks--;
                    it2->second->getfsfile()->setindexblock(i);
                    isEmpty= true;
                    break;
                }
            }
            if(!isEmpty){
                cout << "there is no space" << endl;
                return -1;
            }
            if(len > freeBlocks*block_size){ // if the num of the blocks that I need is bigger than the empty block i have now, then we cant write
                len=freeBlocks*block_size;
                cout << "there is no space" << endl;
            }
            for(i=0;;i++){
                if(len==Iwrite) break;
                if(BitVector[i]==0){
                    BitVector[i]=1;
                    freeBlocks--;
                    fseek(sim_disk_fd, (it2->second->getfsfile()->getindexblock() * block_size)+it2->second->getfsfile()->getblock_in_use(), SEEK_SET);
                    d =decToBinary(i, d);
                    fwrite(&d, 1, 1, sim_disk_fd);
                    it2->second->getfsfile()->setblock_in_use(it2->second->getfsfile()->getblock_in_use()+1);
                    fseek(sim_disk_fd, (i * block_size), SEEK_SET);
                        for (int j = 0; j < block_size; ++j) {
                            if(len==Iwrite) break;
                            fwrite(&buf[Iwrite], 1, 1, sim_disk_fd);
                            Iwrite++;
                        }
                }
            }
            it2->second->getfsfile()->setfile_size(len);
        } else{
            Iwrite=0;
            if(it2->second->getfsfile()->getfile_size()!=(it2->second->getfsfile()->getblock_in_use()*block_size)){
                fseek(sim_disk_fd, it2->second->getfsfile()->getindexblock()* block_size +  it2->second->getfsfile()->getblock_in_use() - 1, SEEK_SET);
                fread(&d, 1, 1, sim_disk_fd);
                int location = TOINT(d);
                fseek(sim_disk_fd, location * block_size + it2->second->getfsfile()->getfile_size()%block_size, SEEK_SET);
                fwrite(&buf[Iwrite], block_size-(it2->second->getfsfile()->getfile_size()%block_size), 1, sim_disk_fd);
                Iwrite+=block_size-(it2->second->getfsfile()->getfile_size()%block_size);
            }
            if(len-Iwrite>freeBlocks*block_size){
                len=freeBlocks*block_size+Iwrite;
                cout << "there is no space" << endl;
            }
                for(i=0;;i++){
                    if(len==Iwrite) break;
                    if(BitVector[i]==0){
                        BitVector[i]=1;
                        freeBlocks--;
                        fseek(sim_disk_fd, (it2->second->getfsfile()->getindexblock() * block_size)+it2->second->getfsfile()->getblock_in_use(), SEEK_SET);
                        d =decToBinary(i, d);
                        fwrite(&d, 1, 1, sim_disk_fd);
                        it2->second->getfsfile()->setblock_in_use(it2->second->getfsfile()->getblock_in_use()+1);
                        fseek(sim_disk_fd, (i * block_size), SEEK_SET);
                        for (int j = 0; j < block_size; ++j) {
                            if(len==Iwrite){
                                break;
                            }
                            fwrite(&buf[Iwrite], 1, 1, sim_disk_fd);
                            Iwrite++;
                        }
                    }
                }
                it2->second->getfsfile()->setfile_size(it2->second->getfsfile()->getfile_size()+len);
        }
        return len;
    }
    // ------------------------------------------------------------------------
    int fsDisk::DelFile( string FileName ) {
    if(!is_formated)
        return -1;
    char d;
    std::map<string, FileDescriptor*>::iterator it2;
    it2 = MainDir.find(FileName);//find the file we need to delete
    if(it2 == MainDir.end()){
        cout << "File Is Not Found" << endl;
        return -1;
    }
        if(it2->second->getinuse()){
            cout << "File Is open" << endl;
            return -1;
        }
    if(it2->second->getfsfile()->getindexblock()==-1){
        delete(it2->second->getfsfile());
        delete(it2->second);
        MainDir.erase(it2);
    }
    else{
        for (int i = 0; i < it2->second->getfsfile()->getblock_in_use(); ++i) {
            fseek(sim_disk_fd, it2->second->getfsfile()->getindexblock()* block_size +  i, SEEK_SET);
            fread(&d, 1, 1, sim_disk_fd);
            int location = TOINT(d);
            BitVector[location]=0;
            fseek(sim_disk_fd, location * block_size, SEEK_SET);
            for (int j = 0; j < block_size; ++j) {
                fwrite("\0", 1, 1, sim_disk_fd);
            }
        }
        for(int i=0;i<block_size;i++){
            BitVector[it2->second->getfsfile()->getindexblock()]=0;
            fseek(sim_disk_fd, it2->second->getfsfile()->getindexblock()* block_size +  i, SEEK_SET);
            fwrite("\0", 1, 1, sim_disk_fd);
        }
        delete(it2->second->getfsfile());
        delete(it2->second);
        MainDir.erase(it2);
    }
    return 1;
    }
    // ------------------------------------------------------------------------
    int fsDisk::ReadFromFile(int fd, char *buf, int len ) {
    if(!is_formated){
        buf[0]='\0';
        return -1;
    }
    if(len<0){
        buf[0]='\0';
        return -1;
    }
    std::map<int, FileDescriptor*>::iterator it2;
    it2 = OpenFileDescriptors.find(fd);//find the file we need to delete
    if(it2 == OpenFileDescriptors.end()){
        cout << "File Is Not IN OpenFileDescriptors" << endl;
        return -1;
    }
    if(len==0){
        buf[0]='\0';
        return 0;
    }
    int Index=0;
    char d;
    for (int j = 0; j < it2->second->getfsfile()->getblock_in_use()&& Index<len; j++) {
        fseek(sim_disk_fd, (it2->second->getfsfile()->getindexblock() * block_size) + j , SEEK_SET);
        fread(&d, 1, 1, sim_disk_fd);
        int location = TOINT(d);
        fseek(sim_disk_fd, location * block_size, SEEK_SET);
        for (int i = 0; i < block_size; ++i) {
            if(Index==len) break;
            fread(&buf[Index],1,1,sim_disk_fd);
            Index++;
        }
    }
    buf[Index] = '\0';
    return fd;
}