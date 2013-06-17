/*
 * data_transfer_methods.c
 *
 *  Created on: 29 May 2013
 *      Author: Pavan
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "utils.h"

enum {MAX_TYPES = 5};
enum GPIO {NOT_GPIO = 0, ADDRESS1 = 1, ADDRESS2 = 2, ADDRESS3 = 3,
               CLEAR = 4, SET = 5};

//-----------------------------------------------------------------------

void add_offset(state *s,uint32_t baseReg, uint32_t offset){
 //printf("Reg val before: %x\n",s -> reg[baseReg]);
 int index = baseReg;
 uint32_t regVal = s -> reg[index];
 regVal += offset;
 s -> reg[index] = regVal;
}

//-----------------------------------------------------------------------

void subtract_offset(state *s,uint32_t baseReg, uint32_t offset){
 int index = baseReg;
 uint32_t regVal = s -> reg[index];
 regVal -= offset;
 s -> reg[index] = regVal;
}

//-----------------------------------------------------------------------
uint32_t getRD(uint32_t inst){
 return getBits(inst,12,15);
}

//-----------------------------------------------------------------------

uint32_t getRN(uint32_t inst){
 return getBits(inst,16,19);
}

//------------------------------------------------------------------------

uint32_t getContents(state *s, uint32_t r){
 uint32_t res = s -> reg[r];
 return res;
}

void writeReg(state *s, uint32_t r, uint32_t val){
 s -> reg[r] = val;
}

void writeMem(state *s, uint32_t rd_Val, uint32_t mem_loc){
 if(mem_loc%4 == 0){
  s -> ARM_mem[mem_loc/4] = rd_Val;
 }else{
  writeUnalligneWord(mem_loc,s,rd_Val);
 }
}

int typeofAddress(uint32_t val,uint32_t array[5]){
 //POST: returns the type of special value contained in the array (GPIO);
 int i=0;
 for(int i=0; i < MAX_TYPES; i++){
  if(array[i] == val){
   return i+1;
  }
 }
 return i;
}

//-----------------------------------------------------------------------

void handleGPIOTypes(int type){
 switch(type){
  case(ADDRESS1) : printf("One GPIO pin from 20 to 29 has been accessed\n"); break;
  case(ADDRESS2) : printf("One GPIO pin from 10 to 19 has been accessed\n"); break;
  case(ADDRESS3) : printf("One GPIO pin from 0 to 9 has been accessed\n\n"); break;
  case(CLEAR)    : printf("LED OFF!\n");break;
  case(SET)      : printf("LED ON!\n");break;
  default                : printf("Invalid GPIO type !\n"); break;
 }
}
//-----------------------------------------------------------------------

void pre(state *s,int U_Bit,int L_Bit,uint32_t inst){
 //printf("Pre \n");
 int I_Bit = bitCheck(inst,25);
 uint32_t offset = getOp2(s,inst,!I_Bit);
 uint32_t rd     = getRD(inst);
 uint32_t rn     = getRN(inst);
 uint32_t rnValue = getContents(s,rn);
 uint32_t rdValue = getContents(s,rd);

// printf("Contents of r%d before (source): 0x%x \n",rn,rnValue);
// printf("Contents of r%d before (value)  : 0x%x \n\n",rd,rdValue);

 if(U_Bit == 1){
 // printf("UBit = 1 \n");
 // printf("offset = %d \n",offset);
  uint32_t memLoc = (getContents(s,rn)+offset);
  uint32_t memValue;
 // printf("MemLoc Before: 0x%x \n",memLoc);
  if(rn == 15){
   memLoc = s-> PC;
   memLoc += offset;
  }
 // printf("MemLoc After: 0x%x \n\n",memLoc);
  int type = typeofAddress(memLoc,s->Special_Memory);
  if(memLoc > 16380 && type == NOT_GPIO){
   printf("Error: Out of bounds memory access at address 0x%08x\n",memLoc);
   return;
  }
  if(type != 0 && L_Bit == 0){
   handleGPIOTypes(type);
   return;
  }else if(type != 0 && L_Bit == 1){
   memValue = s->Special_Memory[type-1];
   handleGPIOTypes(type);
  }else{
   memValue = (memLoc%4 == 0) ? s-> ARM_mem[memLoc/4] : getUnallignedWord(memLoc,s);
  }
  //printf("Memory Loc: 0x%x\n",memLoc);
  //printf("Memory value: 0x%x \n\n",memValue);

  switch(L_Bit){
   case(0) :  writeMem(s,rdValue,memLoc);/*printf("Writing to mem \n");*/ break;
   case(1) :  writeReg(s,rd,memValue);/*printf("Case 1\n");*/ break; // Sets the dest reg with rn current mem value;
   default : perror("Error in pre"); break;
  }
 }else{
  uint32_t memLoc = getContents(s,rn)-offset;
  uint32_t memValue;
  if(rn == 15){
   memLoc = s-> PC;
   memLoc += offset;
  }
  int type = typeofAddress(memLoc,s->Special_Memory);
  if(memLoc > 16380 && type == NOT_GPIO){
   printf("Error: Out of bounds memory access at address 0x%08x\n",memLoc);
   return;
  }
  if(type != 0 && L_Bit == 0){
   handleGPIOTypes(type);
   return;
  }else if(type != 0 && L_Bit == 1){
   memValue = s->Special_Memory[type-1];
   handleGPIOTypes(type);
  }else{
   memValue = (memLoc%4 == 0) ? s-> ARM_mem[memLoc/4] : getUnallignedWord(memLoc,s);
  }
  switch(L_Bit){
   case(0) :  writeMem(s,rdValue,memLoc); break;
   case(1) :  writeReg(s,rd,memValue); break; // Sets the dest reg with rn current mem value;
   default : perror("Error in pre"); break;
  }
 }

 //printf("Contents of r%d after (source): 0x%x \n",rn,getContents(s,rn));
 //printf("Contents of r%d after (val)  : 0x%x \n",rd,getContents(s,rd));

 //1110 01 0 1 1 00 1 0000 0010 000000000000 -- gpio_0
}

//-----------------------------------------------------------------------

void post(state *s,int U_Bit,int L_Bit,uint32_t inst){

 //11100110 1000 0010 0001 000000000100
 //printf("Post \n");
 int I_Bit = bitCheck(inst,25);
 uint32_t offset = getOp2(s,inst,!I_Bit);
 uint32_t rd     = getRD(inst);
 uint32_t rn     = getRN(inst);
 uint32_t rnValue = getContents(s,rn);
 uint32_t rdValue = getContents(s,rd);

  //printf("Contents of r%d before (source): 0x%x \n",rn,rnValue);
  //printf("Contents of r%d before (dest)  : 0x%x \n",rd,rdValue);

 //assert: U_Bit is either 0 or 1;
 uint32_t memLoc = getContents(s,rn);
 uint32_t memValue;
 if(rn == 15){
  memLoc = s-> PC;
  memLoc += offset;
 }
 int type = typeofAddress(memLoc,s->Special_Memory);
 if(memLoc > 16380 && type == NOT_GPIO){
  printf("Error: Out of bounds memory access at address 0x%08x\n",memLoc);
  return;
 }
 if(type != 0 && L_Bit == 0){
   handleGPIOTypes(type);
   return;
  }else if(type != 0 && L_Bit == 1){
   memValue = s->Special_Memory[type-1];
   handleGPIOTypes(type);
  }else{
   memValue = (memLoc%4 == 0) ? s-> ARM_mem[memLoc/4] : getUnallignedWord(memLoc,s);
  }
 //printf("Offset: %d \n",offset);

 if(U_Bit == 1){
 // printf("UBit = 1 \n");
  switch(L_Bit){
   case(0) :  writeMem(s,rdValue,memLoc); add_offset(s,rn,offset); /*printf("Writing to mem \n");*/ break;
   case(1) :  writeReg(s,rd,memValue); add_offset(s,rn,offset);/* printf("Writing to reg \n"); */ break;
   default : perror("Error in post"); break;
  }
 }else{
 // printf("UBit = 0 \n");
  switch(L_Bit){
   case(0) :  writeMem(s,rdValue,memLoc); subtract_offset(s,rn,offset); break;
   case(1) :  writeReg(s,rd,memValue); add_offset(s,rn,offset); break;
   default : perror("Error in post"); break;
  }
 }
// printf("Contents of r%d after (source): 0x%x \n",rn,getContents(s,rn));
 //printf("Contents of r%d after (dest)  : 0x%x \n",rd,getContents(s,rd));
}

//-----------------------------------------------------------------------

void runTransfer(state *s,int P_Bit,int U_Bit,int L_Bit,uint32_t inst){
 if(P_Bit == 1){
  pre(s,U_Bit,L_Bit,inst);
 }else{
  post(s,U_Bit,L_Bit,inst);
 }
}

void data_transfer(uint32_t inst, state *s){
 int U_Bit = bitCheck(inst,23); // check if UP_BIT is set high;
 int P_Bit = bitCheck(inst,24); // check if P_BIT is set high;
 int L_Bit = bitCheck(inst,20); // check if L bit is high;

// printf("Data Transfer: 0x%x \n\n",inst);


 //1110 01 0 1 1 0 0 0 0001 0000 000000001100
 //Copy the value of memory location 0

 //Offset = 0;
 //ARM_mem[0x200] = 0x05;
 //ARM_mem[0x20c] = 0x30;
 //rn = 0x200;
 //rd = 0;

// s -> ARM_mem[0x200] = 0x05;
// s -> ARM_mem[0x20c] = 0x30;
// s -> reg[rn] = 0x200;
// s -> reg[rd] = 0;

 //printARM_Memory(*s);
 runTransfer(s,P_Bit,U_Bit,L_Bit,inst);
// printf("\n");
 //printRegisters(*s);
 //printARM_Memory(*s);
}

//-----------------------------------------------------------------------
