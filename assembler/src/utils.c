/*
 * utils.c
 *
 *  Created on: 29 May 2013
 *      Author: Pavan
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "utils.h"
#include "LinkedList.h"
#include "leak_detector_c.h"


void writeToBinaryFile(char *filePath,assembler *assInstState,int nmemb){
 FILE *fp = fopen(filePath,"w");
 if(fp == NULL){
  perror("Writing binary file error");
  exit(EXIT_FAILURE);
 }
  for(int i=0; i<nmemb; i++){
   fwrite(&(assInstState->Instructions[i]),sizeof(uint32_t),1,fp);
  }
  fclose(fp);
  //printf("Writing done \n");

}

void printOriginal(char *loc){
 //printf("%s \n",loc);
 FILE *fp = fopen(loc,"rb");
 uint32_t *bitArray = calloc(sizeof(uint32_t)*100,4);
 int i=0;
 fseek(fp,0,SEEK_END);
 int size = ftell(fp);
 fseek(fp,0,SEEK_SET);
 while(!feof(fp)){
  fread(&(bitArray[i]),sizeof(uint32_t)*100,1,fp);
 }
 for(int j=0; j < (size/4); j++){
  printBits(bitArray[j]);
 }
}

int maxLength(char **stringArray){
 int i=0;
 while(stringArray[i] != NULL){
  i++;
 }
 printf("Max array length = %d \n",i);
 return i;
}

int getMaxIndex(uint32_t *arr){
 int i=0;
 while(arr[i] != 0x0){
  i++;
 }
 return i;
}

int finishedExecution(assembler *instState,int last_poss){
 printf("ended ex \n");
 int i=0;
 iterator it = start(instState->BigVals);
 iterator foot = end(instState->BigVals);

 printf("\nPinting list: \n");
 printList(instState->BigVals);
 printf("\n\n");

 while(it != foot){
  int inc = last_poss+i;
  uint32_t offset = (inc - it->type);
  offset = 4*(inc-2); //since PC is ahead by 2 instructions
  printf("\ninc:%d\n",last_poss);
  printf("offset: 0x%08x \n",offset);
  printf("Val: 0x%08x\n\n",it->value);
  instState->Instructions[last_poss] = it->value; //TODO: CHECK;
  last_poss++;
  uint32_t transfer_inst = instState->Instructions[it->type];
  transfer_inst = transfer_inst ^ offset;
  instState->Instructions[it->type] = transfer_inst;
  it = next(it);
  i++;
 }
 return last_poss;
}

int isConst(char* theConstant){
	if(theConstant[0] == '#') return 1;
	return 0;
}

uint32_t getConst(char* theConstant){
	int len = strlen(theConstant);
	//printf("Converting theConstant[%i]: %s\n",len,theConstant);
	if(theConstant[len-1] == '\n'){
		len--;
	}
	char constant[len-1];
	for(int i=1;i < len;i++){
		//printf("Loading Char: %c\n", theConstant[i]);
		constant[i-1] = theConstant[i];
	}

	int res = strtol(constant,NULL,0);
	//printf("res: %i",res);
	return res;
}

cycle initCycle(void){
 cycle c = {.current_instr = 1, .prev_instr = 1, .type = 0};
 return c;
}

state initState(void) {
 state st = {.PC = 0, .SP = 0, .LR = 0, .CPSR = 0, .reg = {0}};
 st.ARM_mem  = calloc(sizeof(uint32_t) * 4096,4);
 return st;
 }

assembler initASM(void){
 assembler instState = {.counter = 0};
 instState.Instructions = calloc(sizeof(uint32_t) * 1000,4);
 instState.BigVals = malloc(sizeof(table_t));
 initLinkedList(instState.BigVals);
 return instState;
}

void printBits(uint32_t x){
 uint32_t mask = 1 << 31;
 for(int i=0; i<32; i++){
  printf("%i",(x & mask) != 0);
  x <<= 1;
 }
 printf("\n");
}

void printAllBits(assembler *output, int len){
	for(int i = 0; i<len; i++){
		printBits(output->Instructions[i]);
	}
}


void printHex(uint32_t x){
 printf("0x%x \n",x);
}

void outputStateTEMP(state s){
 printf("Registers:\n");
 for(int i=0; i<10; i++){
  printf("$%d  :%11d (0x%08x)\n", i,s.reg[i],s.reg[i]);
 }
 printf("$%d :%11d (0x%08x)\n", 10,s.reg[10],s.reg[10]);
 printf("$%d :%11d (0x%08x)\n", 11,s.reg[11],s.reg[11]);
 printf("$%d :%11d (0x%08x)\n", 12,s.reg[12],s.reg[12]);
 //------------------------
 printf("PC  :%11d (0x%08x)\n",s.PC,s.PC);
 //------------------------
 printf("CPSR:%11d (0x%08x)\n",s.CPSR,s.CPSR);
 //------------------------
 printf("Non-zero memory:\n");
 int j=0;
 while(j < 0x1000){
  if(s.ARM_mem[j] != 0){
   printf("0x%08x: 0x%08x\n",4*j,s.ARM_mem[j]);
  }
  j++;
 }
}

uint32_t switchEndian(uint32_t val){
 uint32_t r1 = (0x000000FF & val) << 24;
 uint32_t r2 = (0x0000FF00 & val) << 8;
 uint32_t r3 = (0x00FF0000 & val) >> 8;
 uint32_t r4 = (0xFF000000 & val) >> 24;
 return r1 ^ r2 ^ r3 ^ r4;
}


uint32_t getUnallignedWord(int memLoc,state *s){

// printf("Unalligned Initial memLoc: 0x%08x \n",memLoc);
 int excess = memLoc % 4;
 int initialIndex = (memLoc/4);
 int nextIndex = initialIndex + 1;

// printf("Initial Index: 0x%08x \n",initialIndex);

 uint32_t startValue = switchEndian(s -> ARM_mem[initialIndex]);
// printf("Start value: 0x%08x \n",startValue);
 uint32_t endValue   = switchEndian(s -> ARM_mem[nextIndex]);
// printf("End value: 0x%08x \n\n",endValue);

 int num_bytes1 = 4 - excess;
// printf("Excess: %d \n",num_bytes1);

 startValue <<= 8*excess;
// printf("Shifted Start: 0x%08x \n",startValue);
 endValue >>= 8*num_bytes1;
// printf("Shifted End: 0x%08x \n",endValue);

 uint32_t result = startValue ^ endValue;

// printf("Unaligned Resul: 0x%08x \n",switchEndian(result));
 return switchEndian(result);

}

void writeUnalligneWord(int memLoc,state *s,uint32_t rd_Val){
  printf("Writing to an unalligned memory address \n");

  int excess = memLoc % 4;
  int initialIndex = (memLoc/4);
  int nextIndex = initialIndex + 1;

  uint32_t startValue = switchEndian(s -> ARM_mem[initialIndex]);
   printf("Start value: 0x%08x \n",startValue);
  uint32_t endValue   = switchEndian(s -> ARM_mem[nextIndex]);
   printf("End value: 0x%08x \n",endValue);
   printf("Reg value: 0x%08x \n\n",rd_Val);

  uint32_t start_mask;
  uint32_t reg_mask;
  uint32_t end_mask;
  uint32_t reg_mask2;
  switch(excess){
   case(1): start_mask = 0xFF000000; reg_mask = 0xFFFFFF00;
             end_mask  = 0x00FFFFFF; reg_mask2 = 0x000000FF; break;

   case(2): start_mask = 0xFFFF0000; reg_mask = 0xFFFF0000;
             end_mask  =  0x0000FFFF; reg_mask2 = 0x0000FFFF; break;

   case(3): start_mask = 0xFFFFFF00; reg_mask = 0xFF000000;
             end_mask  = 0x000000FF; reg_mask2 = 0x00FFFFFF; break;

   default: perror("Invalid allignment \n"); break;
  }
  uint32_t startResult 		= startValue & start_mask;printBits(startResult);
  uint32_t regStartResult   = (rd_Val & reg_mask); printBits(regStartResult);
  uint32_t firstAddress 	= startResult ^ regStartResult;
  printf("Start value: 0x%08x \n\n",startResult);

  uint32_t endResult 		= endValue & end_mask; printBits(endResult);
  uint32_t regEndResult     = (rd_Val & reg_mask2) << (4-excess)*8; printBits(regEndResult);
  uint32_t secondAddress    = endResult ^ regEndResult;

  printf("End value: 0x%08x \n",secondAddress);

  firstAddress = switchEndian(firstAddress);
  firstAddress <<= excess * 8;
  secondAddress = switchEndian(secondAddress);
  firstAddress =  firstAddress ^ (secondAddress & reg_mask2);
  secondAddress = secondAddress & reg_mask;

  s -> ARM_mem[initialIndex] = firstAddress;
  s -> ARM_mem[nextIndex] 	 = secondAddress;
}


void printARM_Memory(state s){
 for(int j=0; j<4096; j++){
  printf("Memory #%d : 0x%x \n", j, s.ARM_mem[j]);
 }
}

void writeState(state s){
 FILE *fp = fopen("add01.out","w");
 fwrite(stdout,200,1000,fp);
}


uint8_t getCond(uint32_t inst){
 uint32_t mask = 0xF0000000;
 uint32_t res = mask & inst;
 return res >> 28;
}

int bitCheck(uint32_t inst,int index){
 uint32_t mask = 1 << index;
 if(!(mask & inst)){
  return 0;
 }
  return 1;
}

int bitsCheck(uint32_t inst, uint32_t mask){
  if(!(mask ^ inst)){
   return 0;
  }
   return 1;
}

uint32_t getBits(uint32_t x,int initial, int final){
	x = x << (31-final);
	x = x >> (31-(final-initial));
	return x;
}

uint32_t setBit(uint32_t i, int n, int newBit){
	uint32_t mask = newBit << n;
	if(bitCheck(i,n)==1 && newBit==0){
		uint32_t lBits = getBits(i,n+1, 31) << (n+1);
		uint32_t rBits = getBits(i,0, n-1);
		return (lBits | rBits) | mask;
	} else if(bitCheck(i,n)==0 && newBit==1){
		return (i | mask);
	}
	return i;
}

char *prepend(char *initialString,char k){
 char *result = malloc(sizeof(char)* (strlen(initialString)+1));
 result[0] = k;
 strcpy((result+1),initialString);
 free(initialString);
 return result;
}

uint32_t setBits(uint32_t i, int initial, int final, int newBit){
	for(int k=initial;k<=final;k++){
		i = setBit(i,k,newBit);
	}
	return i;
}

uint32_t addBinary(uint32_t op1, uint32_t op2){
    uint32_t c = 0;
    uint32_t result = 0;
    int value = 0;
    for(int i = 0; i < 32 ; i++) {
        value = ((bitCheck(op1, i) ^ bitCheck(op2, i)) ^ c);
        result = setBit(result, i, value);
        c = ((bitCheck(op1, i) & bitCheck(op2, i)) | (bitCheck(op1, i) & c)) | (bitCheck(op2, i) & c);
    }
    return result;
 }

uint32_t negBinary(uint32_t i){
	return ~i + 1;
}

void updateNZinCPSR(state *s, uint32_t res, int S){
	int N;
	int Z;

	if(S==1){
		 if(res == 0){
			 Z = 1;
		 }

		 N = bitCheck(res,31);

		 s->CPSR = setBit(s->CPSR,31,N);
		 s->CPSR = setBit(s->CPSR,30,Z);
	 }
}

