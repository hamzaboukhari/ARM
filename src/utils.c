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
#include "emu_data_process.h"
#include "emu_multiply.h"
#include "emu_branch.h"
#include "emu_data_transfer.h"

cycle initCycle(void){
 cycle c = {.current_instr = 1, .prev_instr = 1, .type = 0};
 return c;
}

state initState(void) {
 state st = {.PC = 0, .SP = 0, .LR = 0, .CPSR = 0, .reg = {0}};
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

void outputStateTEMP(state s){
 printf("Registers: \n");
 for(int i=0; i<13; i++){
  printf("$%d\t:%10d (0x%08x)\n", i,s.reg[i],s.reg[i]);
 }
 //------------------------
 printf("PC\t:%10d (0x%08x)\n",s.PC,s.PC);
 //------------------------
 printf("CPSR\t:%10d (0x%08x)\n",s.CPSR,s.CPSR);
 //------------------------
 printf("Non-Zero Memory: \n");
 int j=0;
 while(j < 0x1000){
  if(s.ARM_mem[j] != 0){
   printf("0x%08x: 0x%x\n",4*j,s.ARM_mem[j]);
  }
  j++;
 }
}

void outputState(state s, FILE *fp){
 printf("Registers: \n");
 for(int i=0; i<13; i++){
  printf("$%d:\t %d ", i,s.reg[i]);
  printHex(s.reg[i]);

  fprintf(fp,"$%d:\t %d \t", i,s.reg[i]);
  fprintf(fp,"0x%x \n",s.reg[i]);
 }
 printf("\n");
 fprintf(fp,"\n");
 //------------------------
 printf("PC: \t(%d) ",s.PC);
 printHex(s.PC );
 fprintf(fp,"PC: \t %d ",s.PC);
 fprintf(fp," \t0x%x \n",s.PC);
 //------------------------
 printf("CPSR: \t ");
 printHex(s.CPSR);
 fprintf(fp,"CPSR: \t %d ",s.CPSR);
 fprintf(fp," \t0x%x \n",s.CPSR);
 //------------------------
 printf("LR: \t ");
 printHex(s.LR);
 fprintf(fp,"LR: \t %d",s.LR);
 fprintf(fp," \t0x%x \n",s.LR);
 //------------------------
 printf("SP: \t ");
 printHex(s.SP);
 fprintf(fp,"SP: \t %d ",s.SP);
 fprintf(fp," \t0x%x \n",s.SP);
 //------------------------
 printf("\n");
 fprintf(fp,"\n");

 printf("Non-Zero Memory: \n");
 int j=0;
 while(j < 4096){
  if(s.ARM_mem[j] != 0){
   printf("%d:\t 0x%x \n",4*j,s.ARM_mem[j]);
   fprintf(fp,"%d:\t 0x%x \n",4*j,s.ARM_mem[j]);
  }
  j++;
 }
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


int detectReg(char *cmd){

	if(!strcmp(cmd,"r0")){
		return r0;
	} if(!strcmp(cmd,"r1")){
		return r1;
	} if(!strcmp(cmd,"r2")){
		return r2;
	} if(!strcmp(cmd,"r3")){
		return r3;
	} if(!strcmp(cmd,"r4")){
		return r4;
	} if(!strcmp(cmd,"r5")){
		return r5;
	} if(!strcmp(cmd,"r6")){
		return r6;
	} if(!strcmp(cmd,"r7")){
		return r7;
	} if(!strcmp(cmd,"r8")){
		return r8;
	} if(!strcmp(cmd,"r9")){
		return r9;
	} if(!strcmp(cmd,"r10")){
		return r10;
	} if(!strcmp(cmd,"r11")){
		return r11;
	} if(!strcmp(cmd,"r12")){
		return r12;
	}

}

uint32_t setBinary(int val, int shift){
	uint32_t binary = val << shift;
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

void execute(state *s,cycle *c, uint32_t inst){
	//printf("Executing An Instruction...\n\n");

	enum bit {eq=0,ne=1,ge=10,lt=11,gt=12,le=13,al=14};

	uint32_t cond = getBits(inst,28,31);

	int N = bitCheck(s->CPSR,31);
	int Z = bitCheck(s->CPSR,30);
	int C = bitCheck(s->CPSR,29);
	int V = bitCheck(s->CPSR,28);

	if(cond == eq && Z == 1){ printf("Cond: eq\n");
		runInstruction(inst,s,c);
	} else if(cond == ne && Z == 0){ //printf("Cond: ne\n");
		runInstruction(inst,s,c);
	} else if(cond == ge && N == V){ //printf("Cond: ge\n");
		runInstruction(inst,s,c);
	} else if(cond == lt && N != V){ //printf("Cond: lt\n");
		runInstruction(inst,s,c);
	} else if(cond == gt && (Z == 0 && N == V)){ //printf("Cond: gt\n");
		runInstruction(inst,s,c);
	} else if(cond == le && (Z == 1 || N != V)){ //printf("Cond: le\n");
		runInstruction(inst,s,c);
	} else if(cond == al){// printf("Cond: al\n");
		runInstruction(inst,s,c);
	} else {// printf("Cond: N/A\n");
		if(checkB(inst)){
			s -> PC += 4;
		}
	}
}

//Initally the current_inst and prev_inst = 0x1,
//to enter the while loop;
void start(state *s,cycle *c){
//printf("Started...");
 //int i=0;
 while(c -> current_instr != 0x0){
   uint32_t current_inst = c -> current_instr;
   if(checkB(current_inst)){
	//printf("Branch Detected...\n");
	c -> current_instr = c -> prev_instr;
	c -> prev_instr = s -> ARM_mem[(s -> PC)/4];
	execute(s,c,current_inst);
	//s -> PC += 4;
	//i++;
   }else{
	//printf("Non-Branch Detected...\n");
	execute(s,c,c -> current_instr);
	c -> current_instr = c -> prev_instr;
	c -> prev_instr = s -> ARM_mem[(s -> PC)/4];
	s -> PC += 4;
	//i++;
   }
   //outputStateTEMP(*s);
 }
 //printf("Number of times loop is executed: %d \n",(s -> PC)/4);
}
