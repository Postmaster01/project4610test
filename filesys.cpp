#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "sdisk.h"
#include "filesys.h"


Filesys::Filesys(string diskname, int numberofblocks, int blocksize):Sdisk(diskname, numberofblocks, blocksize)
{
	//int blocksize = getblocksize();
	//int numberofblocks = getnumberofblocks();
	int rootsize = (blocksize / 12);
	this-> fatsize = (numberofblocks * 6) / (blocksize + 1);
	string buffer;
	int error = getblock(1,buffer);
	if(buffer[0] == '#')
	{
		buildfs();
		fssynch();
	}
	else
	{
		readfs();
	}
	return ;
}


Filesys::Filesys():Sdisk("sdisk.txt", 256, 128)
{
      
        return ;
}



int Filesys::buildfs()
{
	for(int i = 0; i < rootsize; i++)
	{
		filename.push_back("$$$$$");
		firstblock.push_back(0);
	}

	fat.push_back(2+fatsize);
	fat.push_back(-1);

	for(int i = 0; i < fatsize; i++)
	{
		fat.push_back(-1);
	}

	for(int i = 2 + fatsize; i < getnumberofblocks(); i++)
	{
		fat.push_back(i+1);
	}

	fat[fat.size()-1] = 0;

	return 1;
}
	

int Filesys::fsclose()
{
        return 1;
}
int Filesys::fssynch()
{
        ostringstream ostream;
        for(int i=0; i<filename.size(); i++)
        {
                ostream << filename[i]<< " " << firstblock[i]<< " ";
        }

        string buffer1 = ostream.str();
        vector<string>blocks1 = block(buffer1, getblocksize());
        int error = putblock(1,blocks1[0]);

        ostringstream ostream2;
        for(int i = 0; i<fat.size(); i++)
        {
                ostream2<<fat[i]<<" ";
        }

        string buffer2 = ostream2.str();
        vector<string>blocks2 = block(buffer2, getblocksize());

        for (int i = 0; i<blocks2.size(); i++)
        {
                putblock(2+i, blocks2[i]);
        }
        return 1;
}
int Filesys::readfs()
{
	string buffer;
	getblock(1,buffer);
	istringstream instream1;
	instream1.str(buffer);
	for(int i = 0; i<rootsize; i++)
	{
		string s; int b;
		instream1>>s>>b;
		filename.push_back(s);
		firstblock.push_back(b);
	}
	string buffer2, b;
	for(int i=0; i<fatsize; i++)
	{
		getblock(2+i, b);
		buffer2 += b;
	}
	istringstream instream2;
	instream2.str(buffer2);

	for(int i=0; i<getnumberofblocks(); i++)
	{
		int n;
		instream2>>n;
		fat.push_back(n);
	}
	return 1;

}

int Filesys::newfile(string file)
{
	for(int i = 0; i<filename.size(); i++)
	{
		if(filename[i]==file)
		{
			cout<<"file exists";
			return 0;
		}
	}
	for(int i = 0; filename.size(); i++)
	{
		if(filename[i]=="$$$$$")
		{
			filename[i]=file;
			firstblock[i]=0;
			fssynch();
			return 1;
		}
		return 0;
	}
	return 1;

}

int Filesys::getfirstblock(string file)
{
	for(int i = 0; i < filename.size(); i++)
	{
		if(filename[i]==file)
		{
			return firstblock[i];
		}
	}
	return -1;
}

int Filesys::addblock(string file, string block)
{
	int allocate = fat[0];
	if(allocate == 0)
	{
		cout<<"No space in root";
		return 0;
	}

	int b = getfirstblock(file);
	if(b==-1)
	{
		cout<<"File does not exist";
		return 0;
	}

	fat[0]=fat[allocate];
	fat[allocate]=0;
	if(b==0)
	{
		for(int i=0; i<filename.size(); i++)
		{
			if(filename[i]==file)
			{
				firstblock[i] = allocate;
			}
		}
	}
	else
	{
		while(fat[b] != 0)
		{
			b = fat[b];
		}
		fat[b] = allocate;
	}
	putblock(allocate,block);
	fssynch();
	return allocate;
}

bool Filesys::checkfileblock(string file, int blocknumber)
{
	//returns true if block belongs to file
	////false otherwise
	int blockid = getfirstblock(file);
	if(blockid<=0)
	{
		return false;
	}
	while (blockid != 0)
	{
		if(blockid == blocknumber)
		{
			return true;
		}
		else
		{
			blockid = fat[blockid];
		}
	}
	return false;
}
int Filesys::readblock(string file, int blocknumber, string& buffer)
{
	if(checkfileblock(file, blocknumber))
	{
		getblock(blocknumber, buffer);
		return 1;
	}
	else
	{
		return 0;
	}
}

int Filesys::writeblock(string file, int blocknumber, string buffer)
{
	if(checkfileblock(file, blocknumber))
	{
		putblock(blocknumber, buffer);
		return 1;
	}
	else
	{
		return 0;
	}

}
int Filesys::nextblock(string file, int blocknumber)
{
	if(checkfileblock(file, blocknumber))
        {
               return fat[blocknumber];
        }
        else
        {
                return -1;
        }
}

int Filesys::delblock(string file, int block)
{
        int deallocate=block;
        if(not checkfileblock(file, block))
        {
                return 0;
        }
        int b=getfirstblock(file);
        if(b=block)
        {
                for(int i=0; i<filename.size(); i++)
                {
                        if(filename[i] == file)
                        {
                                firstblock[i]=fat[block];
                                break;
                        }
                }
        }
        else
        {
                while(fat[b]!=deallocate)
                {
                        b=fat[b];
                        fat[b]=fat[block];
                        fat[block]=fat[0];
                        fat[0]=block;
                        fssynch();
                }
        }
	return 0;
}
int Filesys::rmfile(string file)
{
	for(int i =0; i < filename.size(); i++)
	{
		if(filename[i] == file)
		{
			filename[i] = "$$$$$";
			fssynch();
			return 1;
		}
	}
	return 0;
}

vector<string> Filesys::ls()
{ 
	vector <string> flist;
	for (int i=0; i<filename.size(); i++)
	{
		if (filename[i] != "XXXXX")
		{
			flist.push_back(filename[i]);
		}
	}
  	return flist;
}
