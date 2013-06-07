#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "utils.h"
#include "LinkedList.h"

char ***getInstructions(char*** resultArray, int len, table_t *table){

	char ***result = (char ***)malloc(len * sizeof(char**));
	 //allocates dynamic memory for the 3D array result;
	 for(int i=0; i<100; i++){
	  result[i] = (char **)malloc(100 * sizeof(char *));
	  for(int j=0; j<100; j++){
	   result[i][j] = (char *)malloc(100 * sizeof(char));
	  }
	 }

	int j = 0;
	for(int i=0; i<len ;i++){
		//printf("resultArr[%i]: %s\n",i, resultArray[i][0]);
		if(getType(table, resultArray[i][0]) != Label){
			result[j] = resultArray[i];
			j++;
		}
	}

	return result;
}


void assembleInstructions(char*** resultArray, int len, table_t *table, assembler *output){
	//printf("Starting Executing instructions...\n");

	for(int i = 0; i<len; i++){
		//printf("Executing Instruction[%i]...\n",i);

		if(getType(table,resultArray[i][0]) == Data_Processing){ //printf("Detected DP...\n");

			output->Instructions[i] = ass_data_process(resultArray[i],table);

		} else if(getType(table,resultArray[i][0]) == Multiply){ //printf("Detected M...\n");

			output->Instructions[i] = ass_multiply(resultArray[i],table);

		} else if(getType(table,resultArray[i][0]) == Data_Transfer){ //printf("Detected DT...\n");

			//output->Instructions[i] = DataTransfer(1,resultArray[i],&output);

		} else if(getType(table,resultArray[i][0]) == Branch){ //printf("Detected B...\n");

			output->Instructions[i] = ass_branch(resultArray[i],table);

		}

		output->counter++;
	}

}

void printAllArrays(char*** array, int len){
	for(int i = 0; i<len; i++){
		printf("Instruction[%i]:\n",i);
		printArray(array[i]);
	}
}

int main(int argv, char** args){

 char *fileName = args[1];
 int numLines = numOfLines(fileName);

 table_t table;
 init(&table);
 buildSymTable(&table);
 char **resultFromFile = readFromFile(fileName);

 //Converting into a 2D string array
 char ***finalArr = fileTokeniser(resultFromFile,numLines,&table);

 //Removing Labels
 char ***instructionArray = getInstructions(finalArr,numLines,&table);
 numLines = numLines - numOfLoops(finalArr,numLines,&table);
 printAllArrays(instructionArray,numLines);

 assembler assembledInstructions;
 assembledInstructions = initASM();
 //printf("Executing instructions...\n");
 assembleInstructions(instructionArray,numLines,&table, &assembledInstructions);

 printf("Assembled Instructions:\n");
 printAllBits(&assembledInstructions,numLines);

 //printf("Counter: %d\n", assembledInstructions.counter);
 printf("\nFinished...\n");

 return 0;
}

