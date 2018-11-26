#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char ** parseLine();
char ** addToken(char ** bucket, char * token);
int executeCommand(char ** tokens);
void freebucket(char ** tokens);
void prompt();

int main() {
  char ** tokens;
  while(1) { //Infinite loop until break condition
    tokens = parseLine();
    if(executeCommand(tokens)) //Execute Command returns 1 only on exit
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

int executeCommand(char ** tokens)
{
  printf("Executing command %s\n", tokens[0]);
  if(strcmp(tokens[0], "exit") == 0)
    {
      freebucket(tokens);
      return 1;
    }
  return 0;
}
