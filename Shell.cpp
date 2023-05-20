#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "sdisk.h"
#include "filesys.h"
#include "Shell.h"


using namespace std;

Shell::Shell(string diskname, int numberofblocks, int blocksize):Filesys(diskname, numberofblocks, blocksize)
{

}


int Shell::dir() 
{
	vector<string> flist = ls();
	for (int i = 0; i < flist.size(); i++) 
	{
		cout << flist[i] << endl; 
	}
	return 1; 
}
int Shell::add(string file) 
{
	newfile(file); 
	string buffer;
	for (int i = 0; i < getblocksize(); i++) 
	{
		buffer += "R"; 
	}
	vector<string> blocks=block(buffer, getblocksize()); 
	for (int i = 0; i < getblocksize(); i++)
	{
		addblock(file, blocks[i]); 
	}
	return 1; 
}

int Shell::del(string file)
{
	int block = getfirstblock(file);
	while (block > 0)
	{
		delblock(file, block);
		block = getfirstblock(file);
	}
	rmfile(file);
	return 1;
}

int Shell::type(string file)
{
	int block = getfirstblock(file);
	string buffer;
	while (block > 0)
	{
		string t;
		int error = readblock(file, block, t);
		buffer += t;
		block = nextblock(file, block);
	}
	cout<<buffer;
	return 1;
}

int Shell::copy(string file1, string file2)
{
	int block = getfirstblock(file1);
	int code = newfile(file2);
	while (block > 0)
	{
		string buffer;
		code = readblock(file1, block, buffer);
		code = addblock(file2, buffer);
		block = nextblock(file1, block);
	}
	return 1;

}





