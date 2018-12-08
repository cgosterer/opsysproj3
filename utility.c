#include <stdlib.h>
#include "./fatheader.h"

struct openfile openfiles[100000];
int opencounter= 0;

char * goupper(char * s)	                                        // the directory names must be in upper case when stored in file system in utility.c
{
        int t;
        int length = strlen(s);
        for (t = 0; t < length; t++)
        {
                s[t]= toupper((char)s[t]);
        }
        return s;
}

int isbitset(char ch, int x)                                            // checks if the x bit is set to 1 in utility.c file
{
        return ((1 << x) & ch);
}

int getsecaddr(int sector, fatstruct f)                                         // need to pass in boot here for the boot sector info move to utility.c
{
        int ret = sector * (f.BPB_BytsPerSec * f.BPB_SecPerClus);
        return ret;
}

char * noquotes(char * s)
{
        char * retstr;
        retstr = &s[1];
        retstr[strlen(s)-1] = '\0';
        return retstr;
}

int nextCluster(int c)			// grab next cluster
{
	int val;
	//f32Read(c, &val);
	return val;
}

bool addCluster(int end, int num)
{



	return true;
}

bool removeCluster(int n)		// delete na nd all clusters after n, ste data to all zeroes
{



	return true;
}

// jons functions
bool openFile( char * fname, char * mode)			//probably need to make an open file struct with both name and mode
{
	int x;
	for (x = 0; x < 100000; x++)
	{
		if(strcmp(openfiles[x].name,"\0") == 0)
		{
			strcpy( openfiles[x].name, fname);
			strcpy( openfiles[x].mode, mode);
			return true;
		}
	}
	return false;
}

bool closeFile(char * fname)
{
	int x = 0;
	for (x = 0; x < 100000; x++)
	{
		if(strcmp(openfiles[x].name,fname) == 0)
		{
			strcpy(openfiles[x].name, "\0");
			strcpy(openfiles[x].mode, "\0");
			return true;
		}
	}
	return false;
}

bool canRead(FILE * disk, char * name, fatstruct fs, int currdir)
{
	int x;				// needs rework
	for(x=0; x< 100000; x++)
	{
		x = 5;
	}
	return true;
}


bool canWrite(FILE * disk, char * name, fatstruct fs, int cwd )		// needs to be reworked
{
	shortDirEntry sh;
	//int FatOffset = clus * 4;
	int FatOffset = 8;		// temproary fix to avoid compiler error real value is above
        int ThisFatSecNum = fs.BPB_RsvdSecCnt + (FatOffset / fs.BPB_BytsPerSec);
        int ThisFatEntOffset = FatOffset % fs.BPB_BytsPerSec;
        int byteaddress = (ThisFatSecNum * fs.BPB_BytsPerSec) + ThisFatEntOffset;
	fseek(disk, 32, SEEK_SET);
        fread(&sh,sizeof(shortDirEntry), 1, disk);

	if(sh.Attr == 1)
		return false;
	else
		return true;
}

bool closeAll()
{

	return true;
}

/*
int f32_readFAT(int cluster, int *value, fatstruct fs)
{
  int start = fs.BPB_RsvdSecCnt;
  int BPSector = fs.BPB_BytsPerSec;
  int fSecClusters = BPSector / 4;
  int FATsize =  fs.BPB_FATSz32;
  int logicalLBA;
  int index;
  int val;
  logicalLBA = start + ((cluster * 4) / BPSector); // FAT sector that contains the cluster 
  index = (cluster % fSecClusters); // index in the sector of FAT table 
  int *cacheFsec =(int *)malloc(sizeof(int) * fSecClusters);
  val = cacheFsec[index] & 0x0fffffff;
  *value = val;
  return 0;
}
*/


int fnextclus( FILE * disk, int cluster, fatstruct fs)					// get next clsuter using this
{
	int FatOffset = cluster * 4;
	int ThisFatSecNum = fs.BPB_RsvdSecCnt + (FatOffset / fs.BPB_BytsPerSec);
	int ThisFatEntOffset = FatOffset % fs.BPB_BytsPerSec;
	int byteaddress = (ThisFatSecNum * fs.BPB_BytsPerSec) + ThisFatEntOffset;
	int nclus;
	//fsetpos(disk, byteaddress);
	fseek(disk, byteaddress, SEEK_SET);
	fread(&nclus,sizeof(int), 1, disk);
	return nclus;
}

//void open(FILE * disk, fatstruct fs,fileData fd,  char * mode)
//{
//	openFile(fd.fileName, mode);
//}

//void close(FILE * disk, fatstruct fs, char * fname)
//{
//	closeFile(fname);
//	opencounter--;

//}



