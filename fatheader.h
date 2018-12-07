// structs
typedef struct fatstruct{
  int BS_jmpBoot:24;		// Unused for project
  char BS_OEMName[8];		// Unused for project
  int BPB__BytsPerSec:16;	// count of bytes per sector
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
} fileData;

fileData getFileFromDir(char * filename, FILE * disk, int clusterSize);

int  firstSecClus(fatstruct fs, int clus); // finds first "data" sector of cluster clus
int getData(FILE * gfp, int offset, int size); // actually gets the info, given an offset and a size given in fatspec
fatstruct getInfo(FILE * gfp); // get info from boot section needed for rest of program
void info(fatstruct fs); // displays the info per first function in slides
int cd(char * dirname); //Change directory to dirname, returns first cluster of directory
void ls(FILE * disk, fatstruct fs, char * dirname, int currentDir); // basic ls command like usual command line
void read(FILE * disk, fatstruct fs, char * fname); // read file with filename fname
void readsize(FILE * disk, fatstruct fs, char * fname, int offset, int size); // read filename at offset , read size bytes from offset
  void writesize(FILE * disk, fatstruct fs, char * fname, int offset, int size, char * str ); // write to fname at offset, with size = sizem the string str
void write(FILE * disk, fatstruct fs, char * fname, char * str); // the easy write function
void close(FILE * disk, fatstruct fs, char * fname); // close open file with filename fname
void open(FILE * disk, fatstruct fs, char * fname, char * mode); // should take in filname and a strinmg from command line for the mode ie open file123 R|RW|WR|W
void size(FILE * disk, fatstruct fs, char * fname); // prints size of the filename passed in
void creat(FILE * disk, fatstruct fs, char * fname); // create file with fname
void mkdir(FILE * disk, fatstruct fs, char * dname); // create directory with dname
void rm(FILE * disk, fatstruct fs, char * fname); // remove file fname
void rmdir(FILE * disk, fatstruct fs, char * dname); // remove directory dname
