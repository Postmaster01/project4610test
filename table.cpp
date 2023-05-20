#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "block.h"
#include "sdisk.h"
#include "filesys.h"
#include "Shell.h"
#include "table.h"

using namespace std;

Table::Table(string diskname, int numberofblocks, int blocksize, string flatfile, string indexfile):Filesys(diskname,  numberofblocks,  blocksize)
{
	this -> flatfile = flatfile.substr(0,5);
	this -> indexfile = indexfile.substr(0,5);
}
/*
Table::Table():Filesys("sdisk.txt", 256, 128)
{

	        return ;
}
*/

int Table::build_table(string input_file)
{
	ifstream infile;
	infile.open(input_file.c_str());
	newfile(flatfile);
	newfile(indexfile);
	ostringstream ostream;
	string record, key;
	getline(infile, record);

	while(infile.good())
	{
		key = record.substr(0,5);
		vector<string>blocks = block(record, getblocksize());
		int b = addblock(flatfile, blocks[0]);
		ostream<<key<<" "<<b<<" ";
		getline(infile, record);
	}
	
	ostream<<"kangaroo"<<""<<"?"<<"";
	string buffer = ostream.str();
	vector<string> blocks2= block(buffer, getblocksize());
	for(int i=0; i<blocks2.size(); i++)
	{
		addblock(indexfile, blocks2[i]);
	}
	return 1;
	
}

int Table::indexsearch(string key)
{
	//returns -1 if record not found
	string buffer, buffer2;
	int b = getfirstblock(indexfile);
	//error checking
	while(b != 0)
	{
		readblock(indexfile, b, buffer);
		buffer2 += buffer;
		b = nextblock(indexfile, b);
	}
	istringstream istream;
	istream.str(buffer2);
	string ikey;
	int iblock;
	istream>>key>>iblock;
	while(ikey != "kangaroo")
	{
		if(ikey == key)
		{
			return iblock;
		}
		istream>>ikey>>iblock;
	}
	return -1;
}

int Table::search(string key)
{
	int b= indexsearch(key);
	if(b==-1)
	{
		cout<<"record not found";
		return 1;
	}
	else
	{
		string buffer;
		readblock(flatfile, b, buffer);
		cout<<buffer;
	}
	return 1;
}
