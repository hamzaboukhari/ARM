#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>
#include "a_linked_list.h"

void readToArray(FILE *fp,char array[100][100]){
 int i=0;
 while(!feof(fp)){
  fscanf(fp,"%s",(array+i)[100]);
  i++;
 }
 fclose(fp);
}

void rstrip(char *string)
{
  int l;
  if (!string)
    return;
  l = strlen(string) - 1;
  while (isspace(string[l]) && l >= 0) {
    string[l--] = 0;
  }
}

void lstrip(char *string)
{
  int i, l;
  if (!string)
    return;
  l = strlen(string);
  while (isspace(string[(i = 0)]))
    while(i++ < l)
      string[i-1] = string[i];
}

void strip(char *string)
{
  lstrip(string); /*Removes excess spaces/tabs on the start of the string; */
  rstrip(string); /*Removes excess spaces/tabs on the end of the string; */
}


void printArray(char** array){
 for(int i=0;strcmp(array[i],"\0") != 0; i++){
 }
}

char *removeSpaces(char *string){
 int size = strlen(string);
 int j=0;
 char *res = malloc(sizeof(char) * size + 1);
 if(res == NULL){
  perror("Maloc Failure removeSpaces \n");
  exit(EXIT_FAILURE);
 }
 for(int i=0; i<size;i++){
  res[j] = string[i];
  if(string[i] != ' '){
   j++;
  }
 }
 res[j] = '\0';
 return res;
}

char *checkTransferInst(char *string){
 int len = strlen(string);
 for(int i=0; i<len; i++){
  if(string[i] == '['){
   strcpy((string+i),removeSpaces(string+i));
   for(int j=i ; j<len; j++){
	string[j] = string[j] == ',' ? '.' : string[j];
   }
  }
 }
 return string;
}

char **tokeniser(char str[],char *seperator){
   char *token = (char *)malloc(100);
   char **res  = (char **)malloc(20 * sizeof(char *));

   for(int i=0; i<10; i++){
    res[i] = (char *) malloc(100 * sizeof(char));
   }
   int j=0;

   char *modified = checkTransferInst(str);
   token = strtok(modified, seperator);
   while(token != NULL)
    {
      strcpy(res[j],token);
      strip(res[j]);
      token = strtok(NULL, seperator);
      j++;
    }
    strcpy(res[j],"\0");
    return res;
}

int numOfLines(char* input){
	FILE *temp = fopen(input,"r");
    int numLines=0;

	while(!feof(temp)){
     char *line = malloc(sizeof(char) * 100);
     fgets(line,100,temp);
     strip(line);
     if(line != NULL && line[0] != '\0'){
      numLines++;
     }
    }
	fclose(temp);
	return numLines;
}

int numOfLoops(char*** resultArray, int len, table_t *table){

	int j = 0;
	for(int i=0; i<len ;i++){
		if(getType(table, resultArray[i][0]) == Label){
			j++;
		}
	}


	return j;
}

char **readFromFile(char* input){
 FILE *fp = fopen(input,"r");
 int j=0;
 char **result = (char **)malloc(100 * sizeof(char *));
 for(int i=0; i<100; i++){
  result[i] = malloc(100 * sizeof(char));
 }
 while(!feof(fp) && result[j] != NULL){
   result[j] = fgets(result[j],100,fp);
   strip(result[j]);
   if(result[j] != NULL && result[j][0] != '\0'){
	j++;
   }
 }
  return result;
}

char *getLabelName(char c[]) {
	int length = strlen(c);
	char *res = malloc(sizeof(char) * length+1);
	strncpy(res,c, length - 1);
	return res;
}

int checkForLabel(char c[]) {
	int length = strlen(c);
	for (int i = 0 ; i < length ; i++) {
		if (c[i] == ':') {
			return 1;
		}
	}
	return 0;
}

char ***fileTokeniser(char** resultFromFile, int numLines, table_t *table){
 char ***result = (char ***)malloc(100 * sizeof(char**));
 //allocates dynamic memory for the 3D array result;
 for(int i=0; i<100; i++){
  result[i] = (char **)malloc(100 * sizeof(char *));
  for(int j=0; j<100; j++){
   result[i][j] = (char *)malloc(100 * sizeof(char));
  }
 }

 int address = 0;
 for(int i=0; i<numLines; i++){
  char *inst = resultFromFile[i];
  if(checkForLabel(inst) != 0){
	  char* labelName = getLabelName(inst);
	  result[i][0] = "Label";
	  result[i][1] = labelName;
	  int j = 0;
	  	  while (j < numLines) {
	  		  if ((getType(table, result[j][0]) == Branch) && (strcmp(result[j][1], result[i][1]) == 0)) {
	  			  sprintf(result[j][2], "%d", address - atoi(result[j][2]));
	  			  break;
	  		  }
	  		  j++;
	  	  }
	  	  if (j == numLines) {
	  		sprintf(result[i][2], "%d",(address + 4));
	  	  }
  } else {

	  address += 4;
	  char **entry = tokeniser(resultFromFile[i]," ");
  result[i] = entry;

  char **entry2 = tokeniser(result[i][1],",");
  for(int j=0;strcmp(entry2[j],"\0") != 0; j++){
	  if(i==0){
	  }
	  result[i][j+1] = entry2[j];
  }

    if(getType(table, result[i][0]) == 3) {
	  int j = 0;
	  while (j < numLines) {
		  if ((strcmp(result[j][0], "Label") == 0) && (strcmp(result[j][1], result[i][1]) == 0)) {
			  sprintf(result[i][2], "%d", atoi(result[j][2]) - address - 4); //decrement address
			  break;
		  }
		  j++;
	  }
	  if (j == numLines) {
		  sprintf(result[i][2], "%d", address);
	  }

    }
  }

 }

 return result;
}


