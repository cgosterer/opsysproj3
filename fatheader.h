typedef struct fatstruct{
int firstdatasec;		// first data sector
int BPB__BytsPerSec;		// count of bytes per sector
int BPB_SecPerClus;		// number of sectors per allocation unit
int BPB_RsvdSecCnt;		// Number of reserved sectors in the Reserved region of the volumestarting at the first sector of the volume.
int BPB_NumFATs;		// The count of FAT data structures on the volume.
int BPB_FATSz32;		// This field is the FAT32 32-bit count of sectors occupied by ONE FAT
int BPB_RootClus;
}fatstruct;

struct fatstruct fs;			// our variable for a fatstruct that stores all necessary info about file system
FILE * gfp;				// the global file pointer(gfp)?

typedef struct dir			// directory struct;
{
	int d;


}dir;

typedef struct openfile			// file struct
{
	char fname[60];			// im assuming we are going to need filename in this struct at some point
	int firstclusnum;
	int  writemode;			// 1 for read 2 for write 3 for read and write

}openfile;

void exit()		// exits the program and frees memory
{

}

int getinfo(int offset, int size)	// actually gets the info, given an offset and a size given in fatspec
{
	offset = offset -1;
	char readchar;
	int info;

	while(size > 0)
	{
		fseek(gfp, pos, SEEK_SET);
		fseek(gfp, size, SEEK_CUR);
		size--;
		readchar = fgetc(gfp);
		info = info | readchar;
		if(size != 0)
			info = info << 8;
	}
	return info;
}

void info()				// gets the info per first function in slides
{
					// need to use the offset and the size to get each one of these per the fatspec
	fs.BPB__BytsPerSec = getinfo(11,2);
	fs.BPB_SecPerClus = getinfo(13,1);
	fs.BPB_RsvdSecCnt = getinfo(14,2);
	fs.BPB_NumFATs = getinfo(16,1);
	fs.BPB_FATSz32 = getinfo(36,4);
	fs.BPB_RootClus = getinfo(44,4);
	fs.firstdatasec = fs.BPB_RsvdSecCnt + (fs.BPB_NumFATs +  fs.BPB_FATSz32  );

					// now for printing
	printf("BPB__BytsPerSec: %d\n", fs.BPB__BytsPerSec);
	printf("BPB_SecPerClus: %d\n", fs.BPB_SecPerClus);
	printf("BPB_RsvdSecCnt: %d\n", fs.BPB_RsvdSecCnt);
	printf("BPB_NumFATs: %d\n", fs.BPB_NumFATs);
	printf("BPB_FATSz32: %d\n", fs.BPB_FATSz32);
	printf("BPB_RootClus: %d\n", fs.BPB_RootClus);
}

void cd( char * dirname)	// change directory to dirname
{


}


}

void ls()			// basic ls command like usual command line
{


}

void read(char * fname)			// read file with filename fname
{
	// the basic read function
}

void readsize(char * fname, int offset, int size)	// read filename at offset , read size bytes from offset
{
							// the complex read function

	//if(offset > sizeof(fname))
		// printf("error\n");
	//if(size > sizeof(fname))
		// print(entire file);
	// if(offset + size > sizeof(fname))
		//print(sizeof(fname) - offset)


}

void writesize( char * fname, int offset, int size, char * str )	// write to fname at offset, with size = sizem the string str
{
	//the complex write function



}

void write()
{
	// the easy write function
}

void close(char * fname)		// close open file with filename fname
{
	// if (good)
		// close file
	// if (bad)
		// print error or return error signifying int or something similar

}

void open(char * fname, char * mode)		// should take in filname and a strinmg from command line for the mode ie open file123 R|RW|WR|W
{

	//if (badfile)
		//print or return error

}


void size(char * fname)				// prints size of the filename passed in
{


	printf("%s has size %d bytes\n", fname, f->file_size );

}

void creat(char * fname)			// create file with fname
{
	// create a file



}

void mkdir(char * dname)			// create directory with dname
{






}

void rm(char * fname)				// remove file fname
{




}

void rmdir(char * dname)			// remove directory dname
{


}

void exit()		//may need thisfunction or just use break statement in the main loop of command line
{			// may need to put additonal deallocations here




}
