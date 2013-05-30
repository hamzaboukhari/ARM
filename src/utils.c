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
#include "data_process.h"
#include "multiply.h"
#include "branch.h"
#include "data_transfer.h"

cycle initCycle(void){
 cycle c = {.current_instr = 1, .prev_instr = 1, .type = 0};
 return c;
}

state initState(void) {
 state st = {.PC = 0, .SP = 0, .LR = 0, .CPSR = 0, .reg = {0}};
 st.data_mem = calloc(sizeof(uint32_t) * 100,4);
 st.ARM_mem  = calloc(sizeof(uint32_t) * 4096,4);
 return st;
 }

void printBits(uint32_t x){
 uint32_t mask = 1 << 31;
 for(int i=0; i<32; i++){
  printf("%i",(x & mask) != 0);
  x <<= 1;
 }
 printf("\n");
}

void printHex(uint32_t x){
 printf("0x%x \n",x);
}

void printRegisters(state s){
 printf("Registers: \n\n");
 for(int i=0; i<13; i++){
  printf("Register %d\t ", i);
  printHex(s.reg[i]);
 }
 printf("\n");

 printf("PC: \t(%d) ",s.PC);
 printHex(s.PC );

 printf("CPSR: \t ");
 printHex(s.CPSR);

 printf("LR: \t ");
 printHex(s.LR);

 printf("SP: \t ");
 printHex(s.SP);
}

void printFile_Memory(state s){
 for(int j=0; j<100; j++){
  printf("Memory #%d : 0x%x \n", j, s.data_mem[j]);
 }
}

void printARM_Memory(state s){
 for(int j=0; j<4096; j++){
  printf("Memory #%d : 0x%x \n", j, s.ARM_mem[j]);
 }
}

void fprintHex(FILE *fp,uint32_t val){
 fprintf(fp,"0x%x \n",val);
}

void writeRegisters(state s){
 FILE *fp = fopen("add01.out","w");
 for(int i=0; i<13; i++){
   fprintf(fp, "Register %d\t ", i);
   fprintHex(fp, s.reg[i]);
  }
  fprintf(fp, "\n");

  fprintf(fp,"PC: \t ");
  fprintHex(fp, s.PC);

  fprintf(fp,"CPSR: \t ");
  fprintHex(fp, s.CPSR); //prints only the first bit of CPSR;

  fprintf(fp,"LR: \t ");
  fprintHex(fp, s.LR);

  fprintf(fp, "SP: \t ");
  fprintHex(fp, s.SP);
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

uint32_t negBit(uint32_t i,int n){
	if(bitCheck(i,n)==1){
		i = setBit(i,n,0);
	} else if(bitCheck(i,n)==0){
		i = setBit(i,n,1);
	}
	return i;
}

uint32_t negBinary(uint32_t i){
for(int k=0;k<32;k++){
i = negBit(i,k);
}
return addBinary(i,1);
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


//this method is only called if the flag bits match;
//N,Z,C,V - 0,1,2,3 indexes;
void flagCheck(int bit, int status, state *s,uint32_t instr){
 uint32_t Z_mask = 0x40000000; //1
 uint32_t N_mask = 0x80000000; //0
 uint32_t V_mask = 0x10000000; //3

 int Z_bitCheck = bitsCheck(s -> CPSR,Z_mask);
 int N_bitCheck = bitsCheck(s -> CPSR,N_mask);
 int V_bitCheck = bitsCheck(s -> CPSR,V_mask);

 if(bit == 2){
  if(Z_bitCheck == status){
  // printf("cond1 \n");
   runInstruction(instr,s);
  }
 }else if(bit == 3){
  if(N_bitCheck == status){
  // printf("cond2 \n");
   runInstruction(instr,s);
  }
 }else if((bit == 4) && (status == 0)){
  if((Z_bitCheck == 0) && (N_bitCheck == V_bitCheck)){
  // printf("cond3\n");
   runInstruction(instr,s);
  }
 }else if((bit == 4) && (status == 1)){
  if((Z_bitCheck == 1) || (N_bitCheck != V_bitCheck)){
  // printf("cond4 \n");
   runInstruction(instr,s);
  }
 }else if((bit == 0) && (status == 0)){
  // printf("cond5 \n");
   runInstruction(instr,s);
 }else{
  perror("Invalid method call \n");
  exit(EXIT_FAILURE);
 }
}

void execute(state *s, uint32_t instr){
 enum bit {NORMAL = 0, ZERO = 2, NEG = 3, OTHER = 4};
 enum status {LOW = 0, HIGH = 1};

 uint8_t cond = getCond(instr);
 //printf("The cond: %d \n", cond);
 switch(cond){

 case(14) : flagCheck(NORMAL,LOW,s,instr); break;
 case(0)  : flagCheck(ZERO,HIGH,s,instr); break;
 case(1)  : flagCheck(ZERO,LOW,s,instr); break;
 case(10) : flagCheck(NEG,HIGH,s,instr); break;
 case(11) : flagCheck(NEG,LOW,s,instr); break;
 //careful:
 case(12) : flagCheck(OTHER,LOW,s,instr); break;
 case(13) : flagCheck(OTHER,HIGH,s,instr); break;
 default  : printf("wont be executed \n"); break;
 }
}

uint32_t getIndex(state s){
 return (s.PC)/4;
}

//Initally the current_inst and prev_inst = 0x1,
//to enter the while loop;
void start(state *s,cycle *c){
 int i=0;
 printf("PC initially: 0x%x \n",s -> PC);
 while(c -> current_instr != 0x00000000){
   uint32_t current_inst = c -> current_instr;
   if(checkB(current_inst)){
	initCycle();
	c -> prev_instr = s -> data_mem[i];
	s -> PC += 4;
	i++;
   }else{
	c -> current_instr = c -> prev_instr;
	c -> prev_instr = s -> data_mem[i];
	s -> PC += 4;
	execute(s,c -> current_instr);
	i++;
   }
 }
 printf("Number of times loop is executed: %d \n",i);
}
