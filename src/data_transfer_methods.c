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
#include "data_process.h"

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

//-----------------------------------------------------------------------

uint32_t shiftedReg(uint32_t offset){
 //convert offset into reg value;
 return  0; //TODO: Implement;
}

//-----------------------------------------------------------------------

uint32_t immediate(uint32_t offset){
 //convert offset into immediate value;
 return rotateRight(getBits(offset,0,7),2*getBits(offset,8,11));
}

//-----------------------------------------------------------------------

uint32_t getOffset(uint32_t inst){
 int I_Bit = bitCheck(inst,25); // check if I bit is high;
 uint32_t offset = getBits(inst,0,11);
 if(I_Bit){
  return shiftedReg(offset);
 }
  return immediate(offset);
}

//TEST FUNCTION:
uint32_t getOffset_2(uint32_t inst){
 return getBits(inst,0,7);
}

uint32_t getContents(state *s, uint32_t r){
 uint32_t res = s -> reg[r];
 return res;
}

void writeReg(state *s, uint32_t r, uint32_t val){
 s -> reg[r] = val;
}

void writeMem(state *s, uint32_t rd_Val, uint32_t mem_loc){
 s -> ARM_mem[mem_loc/4] = rd_Val;
}

//-----------------------------------------------------------------------

void pre(state *s,int U_Bit,int L_Bit,uint32_t inst){
 printf("Pre \n");
 uint32_t rd 	 = getRD(inst);
 uint32_t rn 	 = getRN(inst);
 uint32_t offset = getOffset_2(inst);
 uint32_t rnValue = getContents(s,rn);
 uint32_t rdValue = getContents(s,rd);

 printf("Contents of r%d before (source): 0x%x \n",rn,rnValue);
 printf("Contents of r%d before (value)  : 0x%x \n",rd,rdValue);

 if(U_Bit == 1){
  printf("UBit = 1 \n");
  uint32_t memLoc = getContents(s,rn)+offset;
  printf("Memory Loc: 0x%x\n",memLoc);
  //mem[r1Val+(offset)] = r0Val
  //offset = r2Val * #

  //r0Val = mem[all that shit];
  switch(L_Bit){
   case(1) :  writeReg(s,rd,s -> ARM_mem[memLoc]);printf("Case 1\n"); break; // Sets the dest reg with rn current mem value;
   case(0) :  writeMem(s,rdValue,memLoc);printf("Case 0\n"); break;
   default : perror("Error in pre"); break;
  }

 }else{
  uint32_t memLoc = getContents(s,rn)-offset;
  switch(L_Bit){
   case(1) :  writeReg(s,rd,s -> ARM_mem[memLoc]); break; // Sets the dest reg with rn current mem value;
   case(0) :  writeMem(s,rdValue,memLoc); break;
   default : perror("Error in pre"); break;
  }
 }

 printf("Contents of r%d after (source): 0x%x \n",rn,getContents(s,rn));
 printf("Contents of r%d after (val)  : 0x%x \n",rd,getContents(s,rd));

}

//-----------------------------------------------------------------------

void post(state *s,int U_Bit,int L_Bit,uint32_t inst){
  printf("Post \n");
 uint32_t rd 	 = getRD(inst);
 uint32_t rn 	 = getRN(inst);
 uint32_t offset = getOffset_2(inst);
 uint32_t rnValue = getContents(s,rn);
 uint32_t rdValue = getContents(s,rd);

  printf("Contents of r%d before (source): 0x%x \n",rn,rnValue);
  printf("Contents of r%d before (dest)  : 0x%x \n",rd,rdValue);

 //assert: U_Bit is either 0 or 1;
 uint32_t memLoc = getContents(s,rn);
 s-> reg[rd] =  s -> ARM_mem[getContents(s,rn)]; // Sets the dest reg with rn current mem value;
 if(U_Bit == 1){
  printf("UBit = 1 \n");
  switch(L_Bit){
   case(1) :  writeReg(s,rd,s -> ARM_mem[memLoc]); add_offset(s,rn,offset); break;
   case(0) :  writeMem(s,rdValue,memLoc); add_offset(s,rn,offset); break;
   default : perror("Error in post"); break;
  }
 }else{
  printf("UBit = 0 \n");
  switch(L_Bit){
   case(1) :  writeReg(s,rd,s -> ARM_mem[memLoc]); add_offset(s,rn,offset); break;
   case(0) :  writeMem(s,rdValue,memLoc); subtract_offset(s,rn,offset); break;
   default : perror("Error in post"); break;
  }
 }
 printf("Contents of r%d after (source): 0x%x \n",rn,getContents(s,rn));
 printf("Contents of r%d after (dest)  : 0x%x \n",rd,getContents(s,rd));
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

 printf("Data Transfer: 0x%x \n\n",inst);


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
 printf("\n");
 //printRegisters(*s);
 //printARM_Memory(*s);
}

//-----------------------------------------------------------------------
