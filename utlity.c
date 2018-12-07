#include "./fatheader.h"

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

void printdir(directory d){
        int z;
        if(d.numfiles == 0)
        {
                printf("Directory is empty");
                return;
        }

        printf("======================");

        for(z = 0; z < d.numfiles; z++)
        {
                file * j = &d.filesindir[z];
                if(!isbitset(j->fattr,4))
                        printf("%s: ", "F");
                else
                        printf("%s: ", "D");
                printf("%s - ", j->fname);
                printf("%i bytes", j->size);
		 printf("(Cluster #: %i)\n", j->firstclusnum);
        }
        printf("===================");
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
	f32Read(c, &val);
	return val;
}

bool addCluster(int end, int num)
{



	return true;
}

bool removeCluster(int n)		// delete na nd all clusters after n, ste data to all zeroes
{




}

// jons functions

bool openFile(int clus, char * mode)
{


	return true;
}

bool closeFile(int clus)
{



	return true;
}

bool canRead(int clus)
{


	return true;
}


bool canWrite( int clus)
{



	return true;
}

bool closeAll()
{



	return true;
}



unsigned short d_readsectors(unsigned long LBAaddress, void * buffer, unsigned short count, unsigned short BPSector)
{
  ssize_t size;
  if (!disk_descriptor) return 0;
  lseek(disk_descriptor, LBAaddress * BPSector, SEEK_SET);
  size = read(disk_descriptor, buffer, count * BPSector);
  return (unsigned short)(size / BPSector);
}
int f32_readFAT(int cluster, int *value, fatstruct fs)
{
  int start = fs.BPB_RsvdSecCnt;
  int BPSector = fs.BPB_BytesPerSec;
  int fSecClusters = BPSector / 4;
  int FATsize =  fs.BPB_FATSz32;
  int logicalLBA;
  int index;
  int val;
 
  logicalLBA = start + ((cluster * 4) / BPSector); /* FAT sector that contains the cluster */
  index = (cluster % fSecClusters); /* index in the sector of FAT table */
  int *cacheFsec =(int *)mallloc(sizeof(int) * fSecClusters);
  val = cacheFsec[index] & 0x0fffffff;
  *value = val;
  return 0;
}

int fnextclus( FILE * disk, int cluster, fatstruct fs)
{
	int FatOffset = cluster * 4;
	int ThisFATSecNum = fs.BPB_ResvdSecCnt + (FatOffset / fs.BPB_BytsPerSec);
	int ThisFatEntOffset = FatOffset % fs.BPB_BytsPerSec;
	int byteaddress = (ThisFatSecNum * fs.BytsPerSec) + ThisFatEntOffset;
	int nclus;
	fsetpos(disk, &byteaddress);
	fread(&nclus,sizeof(int), 1, &disk);
	return nclus;

}







