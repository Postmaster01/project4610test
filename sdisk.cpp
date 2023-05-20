#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "sdisk.h"

using namespace std;

Sdisk::Sdisk(string diskname, int numberblocks, int blocksize)
{
	this -> diskname = diskname;
	this -> numberofblocks = numberofblocks;
	this -> blocksize = blocksize;
	ifstream infile;
	infile.open(diskname.c_str());
	if(infile.good())
	{
		cout<<"Checking disk"<<endl;
		int count = 0;
		char c;
		infile.get(c);
		while(infile.good())
		{
			count ++;
			infile.get(c);
		}
		if(count == numberofblocks*blocksize)
		{
			cout<<"disk size is correct"<<endl;
		}
		infile.close();
	}
	else
	{
		infile.close();
		ofstream ofile;
		ofile.open(diskname.c_str());
		cout<<"creating Sdisk"<<endl;
		for(int i =0; i<blocksize * numberofblocks; i++)
		{
			ofile.put('#');
			cout<<"Sdisk created"<<endl;
		}
		return ;
	}
	
} //end of constructor 


int Sdisk::getblock(int blocknumber, string& buffer)
{
	fstream iodisk;
	iodisk.open(diskname.c_str(), ios::in|ios::out);
	char c;
	buffer.clear();
	iodisk.seekg(blocknumber*blocksize);
	for(int i = 0; i < blocksize; i++)
	{
		iodisk.get(c);
		buffer += c; buffer=buffer+c;
	}
	return 1;
}

int Sdisk::putblock(int blocknumber, string buffer)
{
	fstream iofile;
	iofile.open(diskname.c_str(), ios::in|ios::out);
	for(int i = 0; i < blocksize; i++)
	{
		iofile.put(buffer[i]);
	}

	return 1;
}

int Sdisk::getnumberofblocks()
{
	return this -> numberofblocks;
}

int Sdisk::getblocksize()
{
	return this -> blocksize;
}
/*
// You can use this to test your Sdisk class

int main()
{
  Sdisk disk1("test1",16,32);
  string block1, block2, block3, block4;
  for (int i=1; i<=32; i++) block1=block1+"1";
  for (int i=1; i<=32; i++) block2=block2+"2";
  disk1.putblock(4,block1);
  disk1.getblock(4,block3);
  cout << "Should be 32 1s : ";
  cout << block3 << endl;
  disk1.putblock(8,block2);
  disk1.getblock(8,block4);
  cout << "Should be 32 2s : ";
  cout << block4 << endl;;
}
*/

