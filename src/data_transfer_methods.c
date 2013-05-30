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

void add_offset(state *s,uint32_t baseReg, uint32_t offset){
 //printf("Reg val before: %x\n",s -> reg[baseReg]);
 int index = baseReg;
 uint32_t regVal = s -> reg[index];
 regVal += offset;
 s -> reg[index] = regVal;
}

void subtract_offset(state *s,uint32_t baseReg, uint32_t offset){
 int index = baseReg;
 uint32_t regVal = s -> reg[index];
 regVal -= offset;
 s -> reg[index] = regVal;
}

uint32_t getRD(uint32_t inst){
 return getBits(inst,12,15);
}

uint32_t getRN(uint32_t inst){
 return getBits(inst,16,19);
}

uint32_t shiftedReg(uint32_t offset){
 //convert offset into reg value;
 return  0; //TODO: Implement;
}

uint32_t immediate(uint32_t offset){
 //convert offset into immediate value;
 return rotateRight(getBits(offset,0,7),2*getBits(offset,8,11));
}

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

int32_t getContents(state *s, uint32_t r){
 uint32_t res = s -> reg[r];
 return res;
}

void writeReg(state *s, uint32_t r, uint32_t val){
 s -> reg[r] = val;
}

void pre(state *s,int U_Bit,uint32_t inst){
 uint32_t rd 	 = getRD(inst);
 uint32_t rn 	 = getRN(inst);
 uint32_t offset = getOffset_2(inst);
 uint32_t rnValue = getContents(s,rn);
 uint32_t rdValue = getContents(s,rd);

 printf("Contents of r%d before (source): 0x%x \n",rn,rnValue);
 printf("Contents of r%d before (dest)  : 0x%x \n",rd,rdValue);

 if(U_Bit == 1){
  printf("UBit = 1 \n");
  uint32_t memContents = s -> ARM_mem[getContents(s,rn)+offset];
  writeReg(s,rd,memContents); // Sets the dest reg with rn current mem value;
 }else{
  uint32_t memContents = s -> ARM_mem[getContents(s,rn)-offset];
  writeReg(s,rd,memContents); // Sets the dest reg with rn current mem value;
 }

 printf("Contents of r%d after (source): 0x%x \n",rn,getContents(s,rn));
 printf("Contents of r%d after (dest)  : 0x%x \n",rd,getContents(s,rd));

}

void post(state *s,int U_Bit,uint32_t inst){
 uint32_t rd 	 = getRD(inst);
 uint32_t rn 	 = getRN(inst);
 uint32_t offset = getOffset_2(inst);
 uint32_t rnValue = getContents(s,rn);
 uint32_t rdValue = getContents(s,rd);

  printf("Contents of r%d before (source): 0x%x \n",rn,rnValue);
  printf("Contents of r%d before (dest)  : 0x%x \n",rd,rdValue);

 //assert: U_Bit is either 0 or 1;
 s-> reg[rd] =  s -> ARM_mem[getContents(s,rn)]; // Sets the dest reg with rn current mem value;
 if(U_Bit == 1){
  printf("UBit = 1 \n");
  add_offset(s,rn,offset); //adds the offset to the register;
 }else{
  printf("UBit = 0 \n");
  subtract_offset(s,rn,offset); //subtracts the offset to the register;
 }
 printf("Contents of r%d after (source): 0x%x \n",rn,getContents(s,rn));
 printf("Contents of r%d after (dest)  : 0x%x \n",rd,getContents(s,rd));
}

void indexing(state *s,int P_Bit,int U_Bit,uint32_t inst){
 if(P_Bit == 1){
  printf("Pre \n");
  pre(s,U_Bit,inst);
 }else{
  printf("Post \n");
  post(s,U_Bit,inst);
 }
}

void data_transfer(uint32_t inst, state *s){
 int U_Bit = bitCheck(inst,23); // check if UP_BIT is set high;
 int P_Bit = bitCheck(inst,24); // check if P_BIT is set high;
 //int L_Bit = bitCheck(inst,20); // check if L bit is high;
 //int I_Bit = bitCheck(inst,25); // check if I bit is high;

 uint32_t rd 	 = getRD(inst);
 uint32_t rn 	 = getRN(inst);
 //uint32_t offset = getOffset(inst);

 printf("Data Transfer: 0x%x \n\n",inst);

 //1110 01 0 1 1 0 0 0 0001 0000 000000001100
 //Copy the value of memory location 0

 //Offset = 0;
 //ARM_mem[0x200] = 0x05;
 //ARM_mem[0x20c] = 0x30;
 //rn = 0x200;
 //rd = 0;

 s -> ARM_mem[0x200] = 0x05;
 s -> ARM_mem[0x20c] = 0x30;
 s -> reg[rn] = 0x200;
 s -> reg[rd] = 0;

 //printARM_Memory(*s);
 indexing(s,P_Bit,U_Bit,inst);
 printf("\n");
 printRegisters(*s);
 //printARM_Memory(*s);
}

