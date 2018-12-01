// structs
typedef struct fatstruct{
int firstdatasec;		// first data sector
int BPB__BytsPerSec;		// count of bytes per sector
int BPB_SecPerClus;		// number of sectors per allocation unit
int BPB_RsvdSecCnt;		// Number of reserved sectors in the Reserved region of the volumestarting at the first sector of the volume.
int BPB_NumFATs;		// The count of FAT data structures on the volume.
int BPB_FATSz32;		// This field is the FAT32 32-bit count of sectors occupied by ONE FAT
int BPB_RootClus;
}fatstruct;

typedef struct dir			// directory struct;
{
	int numfiles;			// number of files in driectory


}dir;

typedef struct file			// file struct
{
	char fname[60];			// im assuming we are going to need filename in this struct at some point
	int firstclusnum;
	int mode;			// 1 for read 2 for write 3 for read and write
	int fattr;			// file attribute
	int size;			// fiel size

}file;


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
