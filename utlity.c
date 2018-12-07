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

int nextClus(int c)			// grab next cluster
{

	return 0;
}

bool addCluster(int end, int num)
{



	return true;
}

bool removeClusters(int n)		// delete na nd all clusters after n, ste data to all zeroes
{




}

// jons functions

bool openfile(int clus, char * mode)
{


	return true;
}

bool closefile(int clus)
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

bool closeall()
{



	return true;
}



