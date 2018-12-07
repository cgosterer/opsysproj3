#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
// global variables
//int currentdirclus;		// int for the cluster of current directory
//FILE * disk;
//directory cwd;                        // the current working directory

// structs
typedef struct fatstruct{
  int BS_jmpBoot:24;		// Unused for project
  char BS_OEMName[8];		// Unused for project
  int BPB_BytsPerSec:16;	// count of bytes per sector
  int BPB_SecPerClus:8;		// number of sectors per allocation unit
  int BPB_RsvdSecCnt:16;       	// Number of reserved sectors in the Reserved region of the volumestarting at the first sector of the volume
  int BPB_NumFATs:8;		// The count of FAT data structures on the volume.
  int BPB_RootEntCnt:16;	// Unused by FAT32
  int BPB_TotSec16:16;		// Unused by FAT32
  int BPB_Media:8;		// Unused for project
  int BPB_FATSz16:16;		// Unused by FAT32
  int BPB_SecPerTrk:16;		// Unused for project
  int BPB_NumHeads:16;		// Unused for project
  int BPB_HiddSec:32;		// Unused for project
  int BPB_TotSec32:32;		// Total number of sectors
  int BPB_FATSz32:32;		// This field is the FAT32 32-bit count of sectors occupied by ONE FAT
  int BPB_ExtFlags:16;		// Unused for project
  int BPB_FSVer:16;		// File System Version Number
  int BPB_RootClus:32;		// Cluster number of first cluster of root (should be 2)
}__attribute__((packed, aligned(1))) fatstruct;

typedef struct file                     // file struct
{
        char fname[12];                 // im assuming we are going to need filename in this struct at some point should only be 12
        int firstclusnum;               // number of first cluster for file
        int mode;                       // 1 for read 2 for write 3 for read and write, -1 for not open?
        int fattr;                      // file attribute
        int size;                       // fiel size

}file;

typedef struct directory
{
	char DIR_Name[12];
	int DIR_Attr:8;
	int DIR_NTRes:8;
	int DIR_CrtTimeTenth:8;
	int DIR_CrtTime:16;
	int DIR_CrtDate:16;
	int DIR_LstAccDate:16;
	int DIR_FstClusHI:16;
	int DIR_WrtTime:16;
	int DIR_WrtDate:16;
	int DIR_FstClusLO:16;
	int DIR_FileSize:32;
	int numfiles;
	file filesindir[100];				// stores 100 files for make this dynamic later
}__attribute__((packed, aligned(1))) directory;

typedef struct dir			// directory struct most likely wont need this one
{
	int numfiles;			// number of files in driectory
	file filesindir[100];		// array of files in the directory could also dynamically allocate it if need be with file * filesindir

}dir;

//utility functions
void printdir(directory d);
int isbitset(char ch, int x);
int getsecaddr(int sector, fatstruct f);                                // need to pass in boot here for the boot sector info move to utility.c
char * goupper(char * s);
char * noquotes(char * s);
directory getdir( int clustnum, fatstruct f);
int  firstSecClus(fatstruct fs, int clus); 				// finds first "data" sector of cluster clus
int getData(FILE * gfp, int offset, int size); 				// actually gets the info, given an offset and a size given in fatspec
fatstruct getInfo(FILE * gfp); // get info from boot section needed for rest of program

int nextCluster(int n);
bool addCluster(int end, int n );
bool removeCluster(int n);
bool openFile(int clus, char * mode);
bool closeFile(int clus);
bool canRead(int clus);
bool canWrite(int clus);
bool closeAll();

// end utility functions


void info(fatstruct fs); // displays the info per first function in slides
int cd(char * dirname, fatstruct f); //Change directory to dirname, returns first cluster of directory
/*{
	file * next;
	if(currentdirclus == 0)
	{
		currentdirclus = f.BPB_RootClus;
		cwd = getdir(currentdirclus, f);
	}

	dirname =  goupper(dirname);
	next = findfile(dirname);
	if(next = NULL)
	{
		printf("Directory is not in current directory\n");
		return 0;
	}

	if(isbitset(next->fattr,4))
	{
		printf("Not a directory\n");
		return 0;
	}

	currentdirclus = next->firstclusnum;
	clearcwd();
	cwd = getdir(currentdirclus, f);

}*/
int ls(FILE * disk, fatstruct fs, char * dirname); // basic ls command like usual command line removed int currentdir from parameter list
void read(FILE * disk, fatstruct fs, char * fname); // read file with filename fname
void readsize(FILE * disk, fatstruct fs, char * fname, int offset, int size); // read filename at offset , read size bytes from offset
void writesize(FILE * disk, fatstruct fs, char * fname, int offset, int size, char * str ); // write to fname at offset, with size = sizem the string str
void write(FILE * disk, fatstruct fs, char * fname, char * str); // the easy write function
void close(FILE * disk, fatstruct fs, char * fname); // close open file with filename fname
void open(FILE * disk, fatstruct fs, char * fname, char * mode); // should take in filname and a strinmg from command line for the mode ie open file123 R|RW|WR|W
void creat(FILE * disk, fatstruct fs, char * fname); // create file with fname
void mkdir(FILE * disk, fatstruct fs, char * dname); // create directory with dname
void rm(FILE * disk, fatstruct fs, char * fname); // remove file fname
void rmdir(FILE * disk, fatstruct fs, char * dname); // remove directory dname
void showdir(FILE * disk , directory dir);              // probably wont need disk here?
int size(FILE * disk, fatstruct fs, char * fname); // prints size of the filename passed in
/*{
	file * f;
	if(currentdirclus == 0)
	{
		currentdirclus = fs.BPB_RootClus;
		cwd = getdir(currentdirclus, fs);
	}

	fname = goupper(fname);
	f = findfile(fname);
	if (f == NULL)
	{
		printf("File Not found in Current Directory\n");
		return 0;
	}
	printf("Size of the file %s is %d", fname, f->size);
	return 1;

}*/

directory getdir(int clustnum, fatstruct f);					// pass boot in here need the info gets the contents of a directory files and sub directories included
/*{	file dirfile;								// to be used as a file in the directory
	int totalbytes = (f.BPB_BytsPerSec * f.BPB_SecPerClus);
	directory retdir;							// dir to be returned
	int firstdirsec;							// first sector of directory
	int addrdir;								// address of the directory
	int hiloclus;
	int filecount;								// count of file in each directory

	firstdirsec = firstSecClus(f,clustnum);
	addrdir = getsecaddr(firstdirsec, f);

	directory ds;
        directory * dp = &ds;
	int x;
	for(x = 0; x < totalbytes; x+=32)
	{
        	fread(dp, sizeof(directory), 1, disk);
		if( dp->DIR_Attr == 15)						// skip long entries for now
			continue;

		dp->DIR_FstClusHI << 16;
		hiloclus = dp->DIR_FstClusHI | dp->DIR_FstClusLO;
		strcpy(dirfile.fname, dp->DIR_Name);
		dirfile.firstclusnum = hiloclus;
		dirfile.size = dp->DIR_FileSize;
		dirfile.fattr = dp->DIR_Attr;
		retdir.filesindir[filecount++] = dirfile;
	}

	retdir.numfiles = filecount;
	return retdir;
}*/

//int ls( FILE * disk, fatstruct boot, char * dirname, int currentdir);
/*{
	file * next;
	if (currentdirclus == 0)
	{
		currentdirclus = boot.BPB_RootClus;
		cwd = getdir( currentdirclus, boot);
	}

	fflush(disk);
	dirname = goupper(dirname);

	if(cwd.numfiles == 0)						// if cwd is empty
	{
		printf("The current directory is empty");
		return 0;
	}

	if( currentdirclus = boot.BPB_RootClus && strcmp(dirname, ".") == 0 )
	{
		printdir(cwd);
		return 1;
	}

	next = findfile(dirname);

	if (next == NULL)
	{
		printf("Not found in directory");
		return 0;
	}

	if(!isbitset(next->fattr,4))
	{
		printf("Error Not a Directory");
		return 0;
	}
	printdir(getdir(next->firstclusnum, boot));
}*/


int findopenfile(char * fname);						// finds a file in the open file list
/*{
        file * temp;
        int x;
        for (x=0; x < cwd.numfiles; x++)
        {
                temp = &cwd.filesindir[x];
                if(strcmp(fname, temp->fname) == 0)
                        return x;
        }
        return -1;
}*/


