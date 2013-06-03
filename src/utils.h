/*
 * utils.h
 *
 *  Created on: 28 May 2013
 *      Author: Pavan
 */

#ifndef UTILS_H_
#define UTILS_H_

enum Registers {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12};

typedef struct states{
 uint32_t PC;
 //uint32_t *data_mem; //where contents of the binary file are loaded;
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
void execute(state *s,cycle *c,uint32_t instr);
void start(state *s,cycle *c);
void writeRegisters(state s);
uint32_t getBits(uint32_t x,int initial, int final);
int bitCheck(uint32_t inst,int index);


#endif /* UTILS_H_ */
