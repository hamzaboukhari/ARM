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


int main(int argc, char **argv){

 long size;
 FILE *fp = fopen(argv[1],"rb");
 //printf("Opening file...\n");
 //FILE *out = fopen("test01.out","w");
 //printf("Set buffer...\n");
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
 fread(current_state.ARM_mem, 4, sizeof(uint32_t)*100, fp);
 fclose(fp);
 //execute(&current_state,instr);
 //printf("Executing...\n");
 start(&current_state,&current_cycle);
 //printf("Finished...\n\n");
 outputStateTEMP(current_state);
 //printf("\n");
 //printFile_Memory(current_state);
 //printFile_Memory(current_state);
 //printARM_Memory(&current_state);
 return 0;
}
