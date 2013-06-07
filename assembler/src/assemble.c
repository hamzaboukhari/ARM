#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "utils.h"
#include "LinkedList.h"

/*
char ***getInstructions(char** resultFromFile, table_t *table){

	char ***result = (char ***)malloc(100 * sizeof(char**));
	 //allocates dynamic memory for the 3D array result;
	 for(int i=0; i<100; i++){
	  result[i] = (char **)malloc(100 * sizeof(char *));
	  for(int j=0; j<100; j++){
	   result[i][j] = (char *)malloc(100 * sizeof(char));
	  }
	 }

	int len = strlen(resultFromFile);

	int j = 0;
	for(int i=1;i < len;i++){
		if(getType(table, resultFromFile[0]) != Label){
			j++;
		}
	}

	return
}
*/

int main(int argv, char** args){
 /*printf("Starting...\n");
 char *fileName = "add01.s";
 printf("Finding NLines...\n");
 int numLines = numOfLines(fileName);
 printf("Found NLines...\n");
 table_t table;
 init(&table);
 buildSymTable(&table);
 char **resultFromFile = readFromFile(fileName);
 printf("\nConverting into a 2D string array... \n");
 //printList(table);
 //printf("%d \n",getType(&table,"mov"));

 char ***finalArr = fileTokeniser(resultFromFile,numLines, &table);
 printArray(finalArr[0]);
 printArray(finalArr[1]);
 printArray(finalArr[2]);
 printArray(finalArr[3]);
 printArray(finalArr[4]);
 printArray(finalArr[5]);
 printArray(finalArr[6]);

 /*
 printf("Testing Data Process...\n");
 uint32_t res = ass_data_process(finalArr[0], &table);
 printBits(res);
 */

 //char ***instructionArray = getInstructions(finalArr, &table);

 assembler instState;
 instState = initASM();
 char buffer[] = "r2,[r1,#3]";
 printf("You have entered: %s \n","ldr r2,[r1]");
 char **operands = tokeniser(buffer,",");
 strip(operands[0]);
 strip(operands[1]);
 printf("Operand 1: %s \n",operands[0]);
 printf("Operand 2: %s \n\n",operands[1]);

 char *testStrip[];
 testStrip[0] = "    Pavan";
 testString[1]
 printf("%s\n",testStrip[0]);
 removeWhiteSpace(testStrip);
 printf("%s\n",testStrip[0]);


 DataTransfer(0,operands,instState);

 printf("\nFinished...");
 return 0;
}

