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

typedef struct FATDate
{
  int day:5;
  int month:4;
  int year:7;
}__attribute__((packed, aligned(1))) FATDate;

typedef struct FATTime
{
  int seconds:5;
  int minutes:6;
  int hours:5;
}__attribute__((packed, aligned(1))) FATTime;

typedef struct shortDirEntry
{
  char Name[11];
  int Attr:8;
  int NTRes:8; //Unused in project
  int CrtTimeTenth:8; //Milisecond stamp (actual tenth of seconds) for create time
  FATTime CrtTime; //Creation Time
  FATDate CrtDate;
  FATDate LstAccDate;
  int FstClusHI:16; //First Two Bytes of first cluster number
  FATTime WrtTime;
  FATDate WrtDate;
  int FstClusLO:16; //Last Two Bytes of first cluster number
  int FileSize:32; //File's size in bytes
}__attribute__((packed, aligned(1))) shortDirEntry;

typedef struct unicode {
  char c1;
  char c2;
}__attribute__((packed, aligned(1))) unicode; //Might not need packed, but wanted to make sure it's byte aligned and not word aligned

typedef struct longDirEntry
{
  int Order:8;
  unicode Name1[5];
  int Attr:8;
  int Type:8;
  int Checksum:8;
  unicode Name2[6];
  int FstClusLO:16;
  unicode Name3[2];
}__attribute__((packed, aligned(1))) longDirEntry;

typedef union dirEntry
{
  shortDirEntry sh;
  longDirEntry ln;
} dirEntry;

typedef struct fileData
{
  char fileName[260];
  int size;
  int firstCluster;
  int attr;
} fileData;

typedef struct openfile
{
	char name[260];
	char mode[5];
}openfile;


fileData getFileFromDir(char * filename, FILE * disk, int clusterSize, int directoryCluster, fatstruct fs);
int removeDirectoryEntry(char * filename, FILE * disk, int clustersize, int directoryCluster, fatstruct fs);
int createDirectoryEntry(char * filename, FILE * disk, int clustersize, shortDirEntry insert);
fileData getFileData(char * pathname, FILE * disk, fatstruct fs, int currentDirectory);
void listDirClusterFiles(FILE * disk, int clustersize);

//utility functions
int isbitset(char ch, int x);
char * goupper(char * s);
char * noquotes(char * s);
int  firstSecClus(fatstruct fs, int clus); 				// finds first "data" sector of cluster clus
int getData(FILE * gfp, int offset, int size); 				// actually gets the info, given an offset and a size given in fatspec
fatstruct getInfo(FILE * gfp); // get info from boot section needed for rest of program
void moveToCluster(FILE * disk, int cluster, fatstruct fs); //Moves disk to cluster number in data section
int nextCluster(int n);
bool addCluster(int end, int n );
bool removeCluster(int n);
bool openFile(char * fname, char * mode);
bool closeFile(char * fname);
bool canRead(FILE * disk, char * name, fatstruct fs, int currdir);
bool canWrite( FILE * disk, char * name, fatstruct fs, int cwd);
bool closeAll();
int fnextclus( FILE * disk, int cluster, fatstruct fs);
// end utility functions


void info(fatstruct fs); // displays the info per first function in slides
int cd(FILE * disk, int currentDirectory, fatstruct fs, char * dirname); //Change directory to dirname, returns first cluster of directory
int ls(FILE * disk, int currentDirectory, fatstruct fs, char * dirname); // basic ls command like usual command line removed int currentdir from parameter list
void read(FILE * disk, fatstruct fs, char * fname); // read file with filename fname
void readsize(FILE * disk, fatstruct fs, char * fname, int offset, int size); // read filename at offset , read size bytes from offset
void writesize(FILE * disk, fatstruct fs, char * fname, int offset, int size, char * str ); // write to fname at offset, with size = sizem the string str
void write(FILE * disk, fatstruct fs, char * fname, char * str); // the easy write function
void close(FILE * disk, fatstruct fs, char * fname); // close open file with filename fname
void open(FILE * disk, fatstruct fs,fileData fd, char * mode); // should take in filname and a strinmg from command line for the mode ie open file123 R|RW|WR|W
void creat(FILE * disk, fatstruct fs, char * fname); // create file with fname
void mkdir(FILE * disk, fatstruct fs, char * dname); // create directory with dname
void rm(FILE * disk, fatstruct fs, char * fname); // remove file fname
void rmdir(FILE * disk, fatstruct fs, char * dname); // remove directory dname
int size(FILE * disk, fatstruct fs, char * fname); // prints size of the filename passed in
int findopenfile(char * fname);						// finds a file in the open file list
