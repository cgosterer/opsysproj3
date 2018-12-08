#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./fatheader.h"

char ** parseLine();
char ** addToken(char ** bucket, char * token);
int executeCommand(char ** tokens, FILE * disk, fatstruct boot, int cwd);
void freebucket(char ** tokens);
void prompt();

int main() {
  char ** tokens;
  FILE * disk;
  int currentdirclus;             // int for the cluster of current directory
  int cwd=2;                  // the current working directory's cluster number


  disk = fopen("fat32.img", "rb+");
  fatstruct boot = getInfo(disk);
  while(1) { //Infinite loop until break condition
    tokens = parseLine();
    if(executeCommand(tokens, disk, boot, cwd)) //Execute Command returns 1 only on exit
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
  char removechar;
  prompt();
  bucket = (char **) malloc(1 * sizeof(char **));
  bucket[0] = NULL;
  do{
    //    printf("Loop\n");
    removechar = getchar();
    if (removechar == '"')
      {
	//	printf("if\n");
	//scanf("%c", &removechar);
	//scanf(%[^\"]\"\"%[^\"]\"", instruction);
	scanf(  "%[^\"]\"", instruction);// may need double quote here
	bucket = addToken(bucket, instruction);
	printf("Grabbed Token %s\n", instruction);
	continue;
      }
    else
      {
	//printf("else\n");
	ungetc(removechar, stdin);
	scanf("%s", instruction);
	bucket = addToken(bucket, instruction);
      }
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

int executeCommand(char ** tokens, FILE * disk, fatstruct boot, int cwd)
{
  //printf("Executing command %s\n", tokens[0]);
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
  //else if(strcmp(tokens[0], "ls") == 0)
    //{
      //if(tokens[1] != NULL)
	//ls(disk, cwd, boot, tokens[1]);
      //else
	//printf("Please give a directory (use ls . for cwd)\n");
    //}
  return 0;
}
