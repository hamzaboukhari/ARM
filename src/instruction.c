/*
 * instruction.c
 *
 *  Created on: 30 May 2013
 *      Author: Hakeem
 */

#include <stdio.h>
#include <stdint.h>
#include "utils.h"
#include "data_transfer.h"
#include "multiply.h"
#include "branch.h"
#include "data_process.h"

/*

Some code i used to test it works:

	uint32_t inst;

	//DP: 1110 00 1 0001 1 0000 0001 0001 00000001
	inst = 0xE2301101;

	//SDT: 1110 01 0 1 1 0 0 0 0001 0000 000000001100
	//inst = 0xE581000C;

	//B: 1110 1010 1 0 0 0 0001 0000 000000001100
	//inst = 0xEA81000C;

	//M: 1110 000000 0 0 0000 0000 000010010001
	//inst = 0xE0000091;

	runInstruction(inst);

*/

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


void runInstruction(uint32_t inst, state *s){
	enum instructionType {DP = 1, M = 2, SDT = 3, B = 4};

	/*
	switch(getInstruction(inst)){
		case(DP)  : data_process(inst,s); break;
		case(M)   : multiply(inst,s); break;
		case(SDT) : data_transfer(inst,s); break;
		case(B)   : branch(inst,s); break;
	}
	*/

	//debugger:
	switch(getInstruction(inst)){
		case(DP)  : data_process(inst,s); break;
		case(M)   : multiply(inst,s); break;
		case(SDT) : data_transfer(inst,s); break;
		case(B)   : branch(inst,s); break;
	}

}
