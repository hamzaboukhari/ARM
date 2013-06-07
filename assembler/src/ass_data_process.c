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
#include "LinkedList.h"

uint32_t ass_computeRes(int cmd, char *inst[]){
	uint32_t OpCode = cmd << 21;
	uint32_t Rd = getConst(inst[1]) << 12;
	uint32_t Rn = getConst(inst[2]) << 16;
	char *Op2 = inst[3];
	uint32_t Op2Val;

	if(isConst(Op2)){
		Op2Val = getConst(Op2);
	}else{
		//OPTIONAL: is a register
	}

	uint32_t instruction = OpCode | Rd | Rn | Op2Val;

	instruction = setBit(instruction,25,1); //set I to 1

	return instruction;
}

uint32_t ass_computeNoRes(int cmd, char *inst[]){
	uint32_t OpCode = cmd << 21;
	uint32_t Rn = getConst(inst[1]) << 16;
	char *Op2 = inst[2];
	uint32_t Op2Val;

	if(isConst(Op2)){
		Op2Val = getConst(Op2);
	}else{
		//OPTIONAL: is a register
	}

	uint32_t instruction = OpCode | Rn | Op2Val;

	instruction = setBit(instruction,25,1); //set I to 1
	instruction = setBit(instruction,20,1); //set S to 1

	return instruction;
}

uint32_t ass_mov(char *inst[]){
	uint32_t OpCode = mov << 21;
	uint32_t Rd = getConst(inst[1]) << 12;
	char *Op2 = inst[2];
	uint32_t Op2Val;

	if(isConst(Op2)){
		Op2Val = getConst(Op2);
	}else{
		//OPTIONAL: is a register
	}

	uint32_t instruction = OpCode | Rd | Op2Val;

	instruction = setBit(instruction,25,1); //set I to 1

	return instruction;
}

uint32_t ass_data_process(char *inst[], table_t *table){
	printf("Executing Data Process...\n");

	int OpCode = getValue(table,inst[0]);
	printf("Value: %i\n",OpCode);
	uint32_t res = 14 << 28; //set Cond to 1110 (al)

	switch(OpCode){
		case(add): printf("Executing and...\n"); break;
		case(sub): printf("Executing and...\n"); break;
		case(rsb): printf("Executing and...\n"); break;
		case(and): printf("Executing and...\n"); break;
		case(eor): printf("Executing and...\n"); break;
		case(orr): printf("Executing and...\n"); break;

		case(mov): printf("Executing mov...\n"); break;

		case(tst): printf("Executing and...\n"); break;
		case(teq): printf("Executing and...\n"); break;
		case(cmp): printf("Executing and...\n"); break;
	}

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
