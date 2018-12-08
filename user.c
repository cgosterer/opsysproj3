#include "fatheader.h"

int ls(FILE* disk, int currentDirectory, fatstruct fs, char * dirname)
{
  int cluster;
  fileData dir = getFileData(dirname, disk, fs, currentDirectory);
  if(dir.size == -1)
    {
      printf("Directory does not exist\n");
      return 1;
    }
  if((dir.attr%16)>>4 != 1)
    {
      printf("The file is not a directory\n");
      return 1;
    }
  do
    {
      listDirClusterFiles(disk, fs.BPB_BytsPerSec * fs.BPB_SecPerClus);
      cluster = fnextclus(disk, cluster, fs);
    }while(cluster != 0xFFFFFFFF);
  return 0;
}

int cd(FILE * disk, int currentDirectory, fatstruct fs, char * dirname)
{
  
}
