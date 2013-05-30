/*
 * data_process_methods.c
 *
 *  Created on: 29 May 2013
 *      Author: Pavan
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "utils.h"
#include "data_process.h"

 // Operand 1 : bits 16 - 19;
 // Operand 2 : bits 0 - 11;
 //if the S flag is set high then the CPRS reg should be updated;
 //if the I flag is set high then operand 2 is an immediate constant;


//Test it.
uint32_t rotateRight(uint32_t i, int bits) {
 return ((i >> bits) | (i << (32 - bits)));
}

uint32_t zero_extend(uint8_t immval){
  return (uint32_t)immval;
}

void and(state *st, uint32_t inst,int I, int S){


}

void eor(state *st, uint32_t inst,int I, int S){

}

void sub(state *st, uint32_t inst,int I, int S){

}

void rsb(state *st, uint32_t inst,int I, int S){

}

void add(state *st, uint32_t inst,int I, int S){

}

void tst(state *st, uint32_t inst,int I, int S){

}

void teq(state *st, uint32_t inst,int I, int S){

}

void cmp(state *st, uint32_t inst,int I, int S){

}

void orr(state *st, uint32_t inst,int I, int S){

}

void mov(state *st, uint32_t inst,int I, int S){

}
