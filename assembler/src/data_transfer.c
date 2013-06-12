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

int isMov = 0;

//enum transfer_type{ldr = 0, str = 1};


char *removeBrackets(char *operand){
 printf("\nORIGINAL:%s\n",operand);
 int size = strlen(operand);
 char *res = malloc(sizeof(char) * (size-2));
 strncpy(res,operand+1,size-2);
 printf("BRACKETS REMOVED:%s\n\n",res);
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
   isMov = 1;
   printf("mov instruction \n\n");
   char **inst = malloc(sizeof(char*)*3);
   for(int i=0; i<3; i++){
	inst[i] = malloc(sizeof(char)*20);
   }
   strcpy(inst[0],"mov");
   strcpy(inst[1],Rd);
   strcpy(inst[2],(address+1));
   inst[2] = prepend(inst[2],'#');

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
   return val_mask | rd;
   //1110 01 0 1 1 00 1 1111 0000 000000000000;

  }
 }
 char **splitAddress = tokeniser(address,",");

 if(address[0] == '[' && address[4] != ']' && address[3] != ']'){
  //PRE instruction;
  printf("\npre 1\n");
  P_Bit = 1;
  char *pre_address = removeBrackets(address);
  char **expression = tokeniser(pre_address,".");
  //arguments[2][0] = '[';
  uint32_t rn = getConst(expression[0]);
  uint32_t transfer_mask1 = 0xE5900000;
 // printf("INCVAL:%s\n",expression[1]);
  uint32_t incVal = getConst(expression[1]);
  //1110 01 1 1 1 00 1 rn rd getConst(expression[1]);
  uint32_t offset = incVal; //TODO: ONCE MOV is fixed replace this with the val generated by mov
  printf("OFFSET:%d\n",offset);
  //printBits(transfer_mask1);
  //printBits(offset);
  //printBits(rn);
  //printBits(rd);
  rn <<= 16;
  rd <<= 12;
  return transfer_mask1 | rn | rd | offset;
  //1110 01 1 1 1 00 1 0001 0010 000000001100

  //1110 01 0 1 1 00 0 0001 0000 000000000001; -- generated
  //1110 01 0 1 1 00 0 0001 0000 000000011100; -- actual

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
  return transfer_mask2 | rn | rd;
  //1110 01 0 1 1 00 1 0000 0000 000000000000

 }else{
  printf("post 3\n");
  //Post instruction;
  P_Bit = 0;
  uint32_t transfer_mask3 = 0xE6900000;
  //1110 01 1 0 1 00 1 rn rd getConst(splitAddress[1]);
  char **expression = tokeniser(address,".");
  uint32_t rn = getConst(removeBrackets(expression[0]));
  uint32_t offset = getConst(expression[1]);
  printf("OFFSET:%d\n",offset);
  //printBits(transfer_mask3);
  //printBits(rn);
  //printBits(rd);
  //printBits(offset);
  rn <<= 16;
  rd <<= 12;
  return transfer_mask3 | rn | rd | offset;
 }
}

//1110 01 0 1 1 00 0 0000 0010 000000000000 -- generated



uint32_t DataTransfer(int type,char **arguments,assembler *instState,table_t *t){
 if(type == ldr){ //LDR defined the other way arround for data_tranfer;
  printf("\nLDR instruction \n\n");
  uint32_t ldrRes = transfer(arguments,instState,t);
  if(!isMov){
   ldrRes = setBit(ldrRes,20,1);
  }
  printf("0x%08x \n",ldrRes);
  return ldrRes;
 }else{
  printf("\nSTR instruction \n\n");
  uint32_t strRes = transfer(arguments,instState,t);
  strRes = setBit(strRes,20,0);
  printf("0x%08x \n",strRes);
  return strRes;
 }
}


uint32_t Dt_differentiate(char *instruction[], table_t *t,assembler *instState){
  ///printArray(instruction+1);
  int type = strcmp(instruction[0],"ldr");
  uint32_t result = DataTransfer(type,(instruction+1),instState,t);
  //printf("result worked \n");
  //printBits(result);
  return result;
}



//1110 01 0 1 1 00 1 0001 0011 000000001000 -- actual
//1110 01 1 1 1 00 1 0001 0011 000000001000 -- generated

//11100101100100000000000000000000


//1110 01 0 1 1 00 0 0001 0000 000000000001 -generated
//1110 01 0 1 1 00 0 0001 0000 000000011100 -actual









