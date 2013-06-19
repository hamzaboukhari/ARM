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
 int size;
 FILE *fp = fopen(argv[1],"rb");
 //printf("Opening file...\n");
 //printf("Set buffer...\n");
 state current_state;
 cycle current_cycle;
 if(fp == NULL){
  perror("File not found");
  exit(EXIT_FAILURE);
 }
 current_state = initState(); // initialises states (sets registers to 0);
 current_cycle = initCycle(); // initialises cycle
 fseek(fp, 0, SEEK_END); //sets the file pointer to the end of the file;
 size = ftell(fp); // returns the possition of the file pointer;
 fseek(fp, 0, SEEK_SET); //sets the file pointer to the start;
 fread(current_state.ARM_mem, sizeof(uint32_t), size/sizeof(uint32_t), fp);
 fclose(fp);
 //execute(&current_state,instr);
 //printf("Executing...\n");
 startEmulation(&current_state,&current_cycle);
 //printf("Finished...\n\n");
 outputStateTEMP(current_state);
 return 0;
}
