#include <string.h>
#include <stdio.h>
#include "fatheader.h"

//Concatinates long directory entry name onto important
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

//Gets important file data from a short directory entry
void scrapeFileData(fileData * data, shortDirEntry dir)
{
  data->size = dir.FileSize;
  data->firstCluster = dir.FstClusHI;
  data->firstCluster = data->firstCluster << 16;
  data->firstCluster += dir.FstClusLO;
}

//Given a file entry from a directory, get its name
//Disk is passed so that subsequent entries can be read for long entries
//Return value is number of bytes disk was moved forward
int getFileName(char * destination, FILE * disk, dirEntry file)
{
  int y, i;
  i = 0;
  y = file.sh.Attr;
  if(y%16 == 0xF)
    {
      destination[0] = '\0';
      do
	{
	  getNameFromLongDir(destination, file.ln);
	  fseek(disk, 32, SEEK_CUR);
	  fread(&file, 32, 1, disk);
	  i += 32;
	} while(file.sh.Attr%16 == 0xF);
      return i;
    }
  else
    {
      for(i = 0; file.sh.Name[i] != ' ' && i < 8; i++)
	{
	  destination[i] = file.sh.Name[i];
	}
      if(file.sh.Name[8] != ' ')
	{
	  destination[i] = '.';
	  i++;
	}
      for(y = 8; file.sh.Name[y] != ' ' && y < 3; y++, i++)
	{
	  destination[i] = file.sh.Name[y];
	}
      destination[i] = '\0';
      return 0;
    }
}

//Gets the information on a file given the filename and pointer to cluster of a directory, returns empty data if filename isn't in this cluster
//Leaves disk pointing to first directory entry of file (important for long entries)
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
      y = getFileName(checkName, disk, file);
      i += y;
      if(strcmp(checkName, filename) == 0)
	{
	  strcpy(ret.fileName, checkName);
	  scrapeFileData(&ret, file.sh);
	  // Move disk to first associated directory entry for long entries
	  fseek(disk, 0-y, SEEK_CUR);
	  return ret;
	}
    }
  return ret;
}

//Removes Directory entry from directory cluster
//Returns 0 on success, -1 on failure (file not in cluster)
int removeDirectoryEntry(char * filename, FILE * disk, int clustersize)
{
  //getFileData leaves the file pointer where we need it, and tells us whether the file was actually in this cluster
  fileData fd = getFileFromDir(filename, disk, clustersize);
  dirEntry cleared;
  dirEntry check;
  int i;
  
  cleared.ln.Order = 0xEF;
  cleared.ln.Attr = cleared.ln.Type = cleared.ln.Checksum = cleared.ln.FstClusLO = 0;
  for(i = 0; i < 2; i++)
    cleared.ln.Name3[i].c2 = cleared.ln.Name2[i].c2 = cleared.ln.Name1[i].c2 = '\0';
  for(; i < 5; i++)
    cleared.ln.Name2[i].c2 = cleared.ln.Name1[i].c2 = '\0';
  cleared.ln.Name1[5].c2 = '\0';
  
  if(strcmp(filename, fd.fileName) != 0)
    {
      //The file was not found in the cluster
      return -1;
    }
  
  while(true)
    {
      fread(&check, 32, 1, disk);
      if(check.ln.Attr == 0xF)
	{
	  fwrite(&cleared, sizeof(dirEntry), 1, disk);
	  fseek(disk, sizeof(dirEntry), SEEK_CUR);
	}
      else
	{
	  fwrite(&cleared, sizeof(dirEntry), 1, disk);
	  return 0;
	}
    }
}

//Creates Directory entry from directory cluster
//Returns 0 on success, -1 on failure (no space in cluster for file)
int createDirectoryEntry(char * filename, FILE * disk, int clustersize, shortDirEntry insert)
{
  int i;
  shortDirEntry entry;
  for(i = 0; i < clustersize; fseek(disk, 32, SEEK_CUR))
    {
      fread(&entry, 32, 1, disk);
      if(entry.Name[0] == 0 || entry.Name[0] == 0xE5)
	{
	  i = 1;
	  break;
	}
    }
  if(i == 1)
    return -1;
  fwrite(&insert, 32, 1, disk);
  return 0;
}
