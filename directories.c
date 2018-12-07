#include <string.h>
#include <stdio.h>
#include "fatheader.h"

void getNameFromLongDir(char * checkName, longDirEntry dir)
{
  char nameCopy[260];
  int i, j;
  for(i = 0, j = 0; j < 5; i++, j++)
    {
      nameCopy[j] = dir.Name1[i].c2;
      if(nameCopy[j] == '\0')
	{
	  j = 999;
	  break;
	}
    }
  for(i = 0; j < 11; i++, j++)
    {
      nameCopy[j] = dir.Name2[i].c2;
      if(nameCopy[j] == '\0')
	{
	  j = 999;
	  break;
	}
    }
  for(i = 0; j < 13; i++, j++)
    {
      nameCopy[j] = dir.Name2[i].c2;
      if(nameCopy[j] == '\0')
	{
	  j = 999;
	  break;
	}
    }
  strcat(nameCopy, checkName);
  strcpy(checkName, nameCopy);
}

void scrapeFileData(fileData * data, shortDirEntry dir)
{
  data->size = dir.FileSize;
  data->firstCluster = dir.FstClusHI;
  data->firstCluster = data->firstCluster << 16;
  data->firstCluster += dir.FstClusLO;
}

fileData getFileFromDir(char * filename, FILE * disk, int clusterSize)
{
  dirEntry file;
  char checkName[260];
  int i, y;
  fileData ret;
  
  for(i = 0; i <= 260; i++)
    ret.fileName[i] = '\0';
  ret.size = 0;
  ret.firstCluster = 0;
  
  for(i = 0; i<260; i++)
    checkName[i] = '\0';
  for(i = 0; i<clusterSize; i+=32, fseek(disk, 32, SEEK_CUR))
    {
      fread(&file, 32, 1, disk);
      y = file.ln.Order;
      //Deal with file entries that are blank, or end of directory
      if(y == 0xEF)
	break;
      if(y == 0)
	return ret;
      y = file.sh.Attr;
      if(y%16 == 0xF)
	{
	  checkName[0] = '\0';
	  do
	    {
	      getNameFromLongDir(checkName, file.ln);
	      fseek(disk, 32, SEEK_CUR);
	      fread(&file, 32, 1, disk);
	      i += 32;
	    } while(file.sh.Attr%16 == 0xF);
	  if(strcmp(checkName, filename) == 0)
	    {
	      strcpy(ret.fileName, checkName);
	      scrapeFileData(&ret, file.sh);
	      return ret;
	    }
	}
      else
	{
	  for(i = 0; file.sh.Name[i] != ' ' && i < 8; i++)
	    {
	      checkName[i] = file.sh.Name[i];
	    }
	  if(file.sh.Name[8] != ' ')
	    {
	      checkName[i] = '.';
	      i++;
	    }
	  for(y = 8; file.sh.Name[y] != ' ' && y < 3; y++, i++)
	    {
	      checkName[i] = file.sh.Name[y];
	    }
	  checkName[i] = '\0';
	  if(strcmp(checkName, filename) == 0)
	    {
	      strcpy(ret.fileName, checkName);
	      scrapeFileData(&ret, file.sh);
	      return ret;
	    }
	}
    }
  return ret;
}
