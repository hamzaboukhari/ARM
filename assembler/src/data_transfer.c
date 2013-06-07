/*
 * data_transfer.c
 *
 *  Created on: 5 Jun 2013
 *      Author: pp2112
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "LinkedList.h"
#include "utils.h"


//enum transfer_type{ldr = 0, str = 1};

char *removeBrackets(char *operand){
 int size = strlen(operand);
 char *res = malloc(sizeof(char) * (size-2));
 strncpy(res,operand+1,size-2);
 return res;
}

uint32_t transfer(char **arguments, assembler *instState,table_t *t){
 printf("transfer \n");
 int P_Bit;
 char *Rd = arguments[0];
 char *address = arguments[1];
 uint32_t rd = getConst(Rd);

 printf("RD : 0x%08x \n",rd);
 printf("address : %s \n",arguments[1]);
 int addrlen = strlen(address);
 printf("Address Length: %d \n",addrlen);

 if(*address == '='){
  //numeric constant;
  printf("Numeric Constant \n");
  int val = getConst(address);

  if(val <= 0xFF){
   //mov instruction;
   printf("mov instruction \n\n");
   char **inst = malloc(sizeof(char*)*3);
   for(int i=0; i<3; i++){
	inst[i] = malloc(sizeof(char)*20);
   }
   strcpy(inst[0],"mov");
   strcpy(inst[1],Rd);
   strcpy(inst[2],(address+1));

   printf("inst1:%s\n",inst[0]);
   printf("inst2:%s\n",inst[1]);
   printf("inst3:%s\n\n",inst[2]);

   return ass_data_process(inst,t);
  }else{
   printf("big ldr instruction \n\n");
   strip(arguments[1]);
   insertExpression(instState->BigVals,arguments[1],val,instState->counter);
   rd <<= 12;
   uint32_t val_mask = 0xE59F0000;
   return val_mask ^ rd;
   //1110 01 0 1 1 00 1 1111 0000 000000000000;
  }
 }
 char **splitAddress = tokeniser(address,",");

 if(address[0] == '[' && address[4] != ']' && address[3] != ']'){
  //PRE instruction;
  printf("pre 1 \n");
  P_Bit = 1;
  char *pre_address = removeBrackets(address);
  printf("brackets working: %s \n",pre_address);
  char **expression = tokeniser(pre_address,".");
  printf("expression working: %s \n",expression[0]);
  uint32_t rn = getConst(expression[0]);
  uint32_t transfer_mask1 = 0xE7900000;
  //1110 01 1 1 1 00 1 rn rd getConst(expression[1]);
  uint32_t offset = getConst(expression[1]);
  //printBits(transfer_mask1);
  //printBits(offset);
  //printBits(rn);
  //printBits(rd);
  rn <<= 16;
  rd <<= 12;
  return transfer_mask1 ^ rn ^ rd ^ offset;
  //1110 01 1 1 1 00 1 0001 0010 000000001100

 }else if(addrlen <= 5){
  //PRE instruction;
  printf("pre 2\n");
  P_Bit = 1;
  char *Rn = removeBrackets(splitAddress[0]);
  uint32_t rn = getConst(Rn);
  uint32_t transfer_mask2 = 0xE5900000;
  //printBits(transfer_mask2);
  //printBits(rn);
  //printBits(rd);
  //1110 01 1 1 1 00 1 rn rd 000000000000;
  rn <<= 16;
  rd <<= 12;
  return transfer_mask2 ^ rn ^ rd;
  //1110 01 0 1 1 00 1 0000 0000 000000000000

 }else{
  printf("post 3\n");
  //Post instruction;
  P_Bit = 0;
  uint32_t transfer_mask3 = 0xE6900000;
  //1110 01 1 0 1 00 1 rn rd getConst(splitAddress[1]);
  uint32_t rn = getConst(removeBrackets(splitAddress[0]));
  uint32_t offset = getConst(splitAddress[1]);
  offset = offset & 0x000000FF;
  //printBits(transfer_mask3);
  //printBits(rn);
  //printBits(rd);
  //printBits(offset);
  rn <<= 16;
  rd <<= 12;
  return transfer_mask3 ^ rn ^ rd ^ offset;
 }
}

uint32_t DataTransfer(int type,char **arguments,assembler *instState,table_t *t){
 if(type == ldr){
  //printf("\nLDR instruction \n\n");
  uint32_t ldrRes = transfer(arguments,instState,t);
  printf("0x%08x \n",ldrRes);
  return ldrRes;
 }else{
 // printf("\nSTR instruction \n\n");
  uint32_t strRes = transfer(arguments,instState,t);
  setBit(strRes,20,0);
  printf("0x%08x \n",strRes);
  return strRes;
 }
}


uint32_t Dt_differentiate(char *instruction[], table_t *t,assembler *instState){
  ///printArray(instruction+1);
  int type = !getType(t,instruction[0]);
  uint32_t result = DataTransfer(type,(instruction+1),instState,t);
  //printf("result worked \n");
  //printBits(result);
  return result;
}















