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
 //int32_t instr = 0xE3A01001;

 char input_buffer[100];
 char output_buffer[100];
 char fileName[80];
 scanf("%s",fileName);

 char *input_file   =  fileName;
 strcpy(input_buffer,"tests/");
 strcat(input_buffer,input_file);

 strcpy(output_buffer,"results/");
 strcat(output_buffer,strcat(input_file,".out"));

 printf("Input File Path: %s\n",input_buffer);
 printf("Output File Path: %s\n\n",output_buffer);

 FILE *fp = fopen(input_buffer,"rb");
 FILE *out = fopen(output_buffer,"w");
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
 start(&current_state,&current_cycle);
 outputState(current_state,out);
 printf("\n");
 //printFile_Memory(current_state);
 //printFile_Memory(current_state);
 return 0;
}
