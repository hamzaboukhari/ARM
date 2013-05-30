/*
 * structs.c
 *
 *  Created on: 28 May 2013
 *      Author: Pavan
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "utils.h"


int main(void){
 long size;
 uint32_t instr = 0xE481000C;
 FILE *fp = fopen("add01","rb");
 state current_state;
 cycle current_cycle;
 if(fp == NULL){
  perror("File not found");
  exit(EXIT_FAILURE);
 }
 current_state = initState(); // initialises states (sets registers to 0);
 current_cycle = initCycle(); // initialises cycle

 fseek(fp, 0, SEEK_END);
 size = ftell(fp); // number of instructions
 fseek(fp, 0, SEEK_SET);
 fread(current_state.data_mem, 4, sizeof(uint32_t)*100, fp);
 fclose(fp);
 execute(&current_state,instr);
 //printRegisters(current_state);
 printf("\n");
 //printFile_Memory(current_state);
 return 0;
}
