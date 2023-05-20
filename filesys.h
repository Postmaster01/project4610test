#ifndef FILESYS_H
#define FILESYS_H

#include "block.h"
#include "sdisk.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Filesys: public Sdisk
{
public:
Filesys(string diskname, int numberofblocks, int blocksize);
int fsclose();
int newfile(string file);
int rmfile(string file);
int getfirstblock(string file);
int addblock(string file, string block);
int delblock(string file, int blocknumber);
int readblock(string file, int blocknumber, string& buffer);
int writeblock(string file, int blocknumber, string buffer);
int nextblock(string file, int blocknumber);
vector<string> ls();
Filesys();

private:
int rootsize; // maximum number of entries in ROOT
int fatsize; // number of blocks occupied by FAT
vector<string> filename; // filenames in ROOT
vector<int> firstblock; // firstblocks in ROOT
vector<int> fat; // FAT
int buildfs();
int readfs();
int fssynch();
bool checkfileblock(string file, int blocknumber);
};

#endif 
