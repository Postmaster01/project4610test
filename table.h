#ifndef TABLE_H
#define TABLE_H

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "filesys.h"

using namespace std;

class Table : public Filesys
{
public:
Table(string diskname,int blocksize, int numberofblocks, string flatfile, string indexfile);
int build_table(string input_file);
int search(string value);

private:
string flatfile;
string indexfile;
int numberofrecords;
Filesys filesystem;
int indexsearch(string value);
};

#endif
