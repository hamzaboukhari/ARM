/*
 * ass_data_process.c
 *
 *  Created on: 3 Jun 2013
 *      Author: hj512
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "utils.h"

/*
0000 and
0001 eor
0010 sub
0011 rsb
0100 add
1100 orr
1101 mov
1000 tst
1001 teq
1010 cmp
*/



enum DPCommands {
	and = 0,
	eor = 1,
	sub = 2,
	rsb = 3,
	add = 4,
	orr = 12,
	mov = 13,
	tst = 8,
	teq = 9,
	cmp = 10
};

int detectDPOpCode(char *cmd){

	if(!strcmp(cmd,"add")){
		return add;
	} if(!strcmp(cmd,"sub")){
		return sub;
	} if(!strcmp(cmd,"rsb")){
		return rsb;
	} if(!strcmp(cmd,"and")){
		return and;
	} if(!strcmp(cmd,"eor")){
		return eor;
	} if(!strcmp(cmd,"orr")){
		return orr;
	} if(!strcmp(cmd,"mov")){
		return mov;
	} if(!strcmp(cmd,"tst")){
		return tst;
	} if(!strcmp(cmd,"teq")){
		return teq;
	} if(!strcmp(cmd,"cmp")){
		return cmp;
	}

}

uint32_t ass_computeRes(int cmd, char *inst[]){
	uint32_t OpCode = cmd << 21;
	uint32_t Rd = detectReg(inst[1]) << 12;
	uint32_t Rn = detectReg(inst[2]) << 16;
	uint32_t Op2 = atoi(inst[3]);

	uint32_t instruction = OpCode | Rd | Rn | Op2;

	instruction = setBit(instruction,25,1); //set I to 1

	return instruction;
}

uint32_t ass_computeNoRes(int cmd, char *inst[]){
	uint32_t OpCode = cmd << 21;
	uint32_t Rn = detectReg(inst[1]) << 16;
	uint32_t Op2 = atoi(inst[2]);

	uint32_t instruction = OpCode | Rn | Op2;

	instruction = setBit(instruction,25,1); //set I to 1
	instruction = setBit(instruction,20,1); //set S to 1

	return instruction;
}

uint32_t ass_mov(char *inst[]){
	uint32_t OpCode = mov << 21;
	uint32_t Rd = detectReg(inst[1]) << 12;
	uint32_t Op2 = atoi(inst[2]);

	uint32_t instruction = OpCode | Rd | Op2;

	instruction = setBit(instruction,25,1); //set I to 1

	return instruction;
}

uint32_t ass_data_process(char *inst[]){

	int OpCode = detectDPOpCode(inst[0]);
	uint32_t res = 14 << 28; //set Cond to 1110 (al)

	switch(OpCode){
		case(add): res = res | ass_computeRes(add,inst); break;
		case(sub): res = res | ass_computeRes(sub,inst); break;
		case(rsb): res = res | ass_computeRes(rsb,inst); break;
		case(and): res = res | ass_computeRes(and,inst); break;
		case(eor): res = res | ass_computeRes(eor,inst); break;
		case(orr): res = res | ass_computeRes(orr,inst); break;

		case(mov): res = res | ass_mov(inst); break;

		case(tst): res = res | ass_computeNoRes(tst,inst); break;
		case(teq): res = res | ass_computeNoRes(teq,inst); break;
		case(cmp): res = res | ass_computeNoRes(cmp,inst); break;
	}

	return res;

}
