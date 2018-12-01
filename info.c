#include <stdio.h>
#include "./fatheader.h"

int  firstsecclus(fatstruct fs, int clus)							// finds first "data" sector of cluster clus
{
  return fs.firstdatasec  + ((clus - 2) * fs.BPB_SecPerClus);		// staright form slides part 1
}

int getData(FILE * gfp, int offset, int size)	// actually gets the info, given an offset and a size given in fatspec
{
	offset = offset -1;
	char readchar;
	int info;

	while(size > 0)
	{
		fseek(gfp, offset, SEEK_SET);
		fseek(gfp, size, SEEK_CUR);
		size--;
		readchar = fgetc(gfp);
		info = info | readchar;
		if(size != 0)
			info = info << 8;
	}
	return info;
}

fatstruct getInfo(FILE * gfp)				// gets the info per first function in slides
{
					// need to use the offset and the size to get each one of these per the fatspec
	fatstruct fs;
	fs.BPB__BytsPerSec = getData(gfp, 11,2);
	fs.BPB_SecPerClus = getData(gfp, 13,1);
	fs.BPB_RsvdSecCnt = getData(gfp, 14,2);
	fs.BPB_NumFATs = getData(gfp, 16,1);
	fs.BPB_FATSz32 = getData(gfp, 36,4);
	fs.BPB_RootClus = getData(gfp,44,4);
	fs.firstdatasec = fs.BPB_RsvdSecCnt + (fs.BPB_NumFATs + fs.BPB_FATSz32);
	return fs;
}

void info(fatstruct fs){
	// now for printing
	printf("BPB__BytsPerSec: %d\n", fs.BPB__BytsPerSec);
	printf("BPB_SecPerClus: %d\n", fs.BPB_SecPerClus);
	printf("BPB_RsvdSecCnt: %d\n", fs.BPB_RsvdSecCnt);
	printf("BPB_NumFATs: %d\n", fs.BPB_NumFATs);
	printf("BPB_FATSz32: %d\n", fs.BPB_FATSz32);
	printf("BPB_RootClus: %d\n", fs.BPB_RootClus);
}
