/*
 * instruction.c
 *
 *  Created on: 30 May 2013
 *      Author: Hakeem
 */

#include <stdio.h>
#include <stdint.h>
#include "utils.h"

int checkB(uint32_t inst){
	if(getBits(inst,25,27)==5){ return 1; }
	return 0;
}

int checkSDT(uint32_t inst){
	if(getBits(inst,26,27)==1){ return 1; }
	return 0;
}

int checkDP(uint32_t inst){
	if(getBits(inst,25,27)==1){ return 1; }
	if(bitCheck(inst,4)==0){ return 1; }
	if(bitCheck(inst,7)==0){ return 1; }
	return 0;
}

int getInstruction(uint32_t i){
	enum instructionType {DP = 1, M = 2, SDT = 3, B = 4};

	if(checkB(i) == 1){ return B; }
	if(checkSDT(i) == 1){ return SDT; }
	if(checkDP(i) == 1){ return DP; }
	return M;

}


void runInstruction(uint32_t inst, state *s,cycle *c){
	enum instructionType {DP = 1, M = 2, SDT = 3, B = 4};
	switch(getInstruction(inst)){
		case(DP)  : data_process(inst,s); break;
		case(M)   : multiply(inst,s); break;
		case(SDT) : data_transfer(inst,s); break;
		case(B)   : branch(inst,s,c); break;
	}

}
