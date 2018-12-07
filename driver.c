#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./fatheader.h"

char ** parseLine();
char ** addToken(char ** bucket, char * token);
int executeCommand(char ** tokens, FILE * disk, fatstruct boot);
void freebucket(char ** tokens);
void prompt();

int main() {
  char ** tokens;
  FILE * disk;
  int currentdirclus;             // int for the cluster of current directory
  int cwd;                  // the current working directory's cluster number


  disk = fopen("fat32.img", "rb+");
  fatstruct boot = getInfo(disk);
  while(1) { //Infinite loop until break condition
    tokens = parseLine();
    if(executeCommand(tokens, disk, boot)) //Execute Command returns 1 only on exit
      break;
    freebucket(tokens);
    tokens = 0;
  }
}

void prompt() {
  printf("Enter Command: ");
}

//Grabs a single line and breaks it into array of tokens, where each token is a single word from the instruction
char ** parseLine() {
  const int INST_SIZE = 256;
  char instruction[INST_SIZE];	//holds instruction
  char ** bucket;		//array that holds all instruction tokens
  int i, j, start, end;
  prompt();
  bucket = (char **) malloc(1 * sizeof(char **));
  bucket[0] = NULL;
  do{
    scanf("%s", instruction);
    bucket = addToken(bucket, instruction);
  } while(getchar() != '\n');
  return bucket;
}

char ** addToken(char ** bucket, char * token)
{
  int numTokens;
  char ** newBucket;
  for(numTokens = 0; bucket[numTokens] != NULL; numTokens++){}
  newBucket = (char **) malloc((numTokens + 2) * sizeof(char **));
  for(numTokens = 0; bucket[numTokens] != NULL; numTokens++)
    newBucket[numTokens] = bucket[numTokens];
  newBucket[numTokens] = (char *) malloc(strlen(token) * sizeof(char));
  strcpy(newBucket[numTokens], token);
  newBucket[numTokens + 1] = NULL;
  free(bucket);
  return newBucket;
}

void freebucket(char ** bucket)
{
  int i;
  for(i = 0; bucket[i] != NULL; i++)
    {
      free(bucket[i]);
      bucket[i] = NULL;
    }
  free(bucket);
  bucket = NULL;
}

int executeCommand(char ** tokens, FILE * disk, fatstruct boot)
{
  printf("Executing command %s\n", tokens[0]);
  if(strcmp(tokens[0], "exit") == 0)
    {
      freebucket(tokens);
      fclose(disk);
      return 1;
    }
  else if(strcmp(tokens[0], "info") == 0)
    {
      info(boot);
    }
  return 0;
}
