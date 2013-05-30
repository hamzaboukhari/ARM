/*
 * utils.h
 *
 *  Created on: 28 May 2013
 *      Author: Pavan
 */

#ifndef UTILS_H_
#define UTILS_H_

typedef struct states{
 uint32_t PC;
 uint32_t *data_mem; //where contents of the binary file are loaded;
 uint32_t *ARM_mem;  //emulated memory of an ARM machine;
 uint32_t CPSR;
 int32_t reg[13];
 int32_t SP;
 int32_t LR;
}state;

typedef struct pipeline{
 int32_t current_instr; //current instruction to execute;
 int32_t prev_instr; //next instruction in the pipeline;
 int type; //instruction type
} cycle;

state initState(void);
cycle initCycle(void);
void printRegisters(state s);
void execute(state *s,uint32_t instr);
void writeRegisters(state s);
uint32_t getBits(uint32_t x,int initial, int final);
int bitcheck(uint32_t inst,int index);


#endif /* UTILS_H_ */
