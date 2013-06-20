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
#include "a_linked_list.h";

uint32_t evalI(char *op2){

	if(isConst(op2)){
		return 1;
	}else{
		return 0;
	}
}

uint32_t calcOp2(char *op2){

	if(isConst(op2)){
		//Is a constant
		uint32_t op2Val = getConst(op2);
		int rotate = 0;
		if(op2Val > 255){
			for(int i=0;bitCheck(op2Val,0)==0 && bitCheck(op2Val,1)==0;i++){
				rotate += 2;
				op2Val >>= 2;
			}
		}
		uint32_t rot = (rotate >> 1) << 8;
		rot = ~rot + 1;
		rot = rot & 0xF00;
		return rot | op2Val;
	}else{
		//Is a register
		return getConst(op2);
	}
}

uint32_t ass_computeRes(int cmd, char *inst[]){
	uint32_t OpCode = cmd << 21;
	uint32_t Rd = getConst(inst[1]) << 12;
	uint32_t Rn = getConst(inst[2]) << 16;
	uint32_t Op2 = calcOp2(inst[3]);
	int I = evalI(inst[3]);

	uint32_t instruction = OpCode | Rd | Rn | Op2;

	instruction = setBit(instruction,25,I);

	return instruction;
}

uint32_t ass_computeNoRes(int cmd, char *inst[]){
	uint32_t OpCode = cmd << 21;
	uint32_t Rn = getConst(inst[1]) << 16;
	uint32_t Op2 = calcOp2(inst[2]);
	int I = evalI(inst[2]);

	uint32_t instruction = OpCode | Rn | Op2;

	instruction = setBit(instruction,25,I);
	instruction = setBit(instruction,20,1);


	return instruction;
}

uint32_t ass_mov(char *inst[]){
	uint32_t OpCode = mov << 21;
	uint32_t Rd = getConst(inst[1]) << 12;
	uint32_t Op2 = calcOp2(inst[2]);
	int I = evalI(inst[2]);

	uint32_t instruction = OpCode | Rd | Op2;

	instruction = setBit(instruction,25,I);
	instruction = setBit(instruction,20,0);

	return instruction;
}

uint32_t ass_lsl(char *inst[]){
	uint32_t OpCode = mov << 21;
	uint32_t Rn = getConst(inst[1]) << 12;
	uint32_t Op2 = getConst(inst[1]);
	uint32_t Shift = getConst(inst[2]) << 7;

	if(inst[1][2]=='x'){
		Shift = ~Shift + 1;
		Shift = Shift & 0xF00;
	}

	uint32_t instruction = OpCode | Rn | Shift | Op2;

	return instruction;
}

uint32_t ass_data_process(char *inst[], table_t *table){

	int OpCode = getValue(table,inst[0]);
	uint32_t res = 14 << 28;

	switch(OpCode){
		case(add): res = res | ass_computeRes(add,inst); break;
		case(sub): res = res | ass_computeRes(sub,inst); break;
		case(rsb): res = res | ass_computeRes(rsb,inst); break;
		case(and): res = res | ass_computeRes(and,inst); break;
		case(eor): res = res | ass_computeRes(eor,inst); break;
		case(orr): res = res | ass_computeRes(orr,inst); break;
		case(mov): res = res | ass_mov(inst);break;
		case(tst): res = res | ass_computeNoRes(tst,inst); break;
		case(teq): res = res | ass_computeNoRes(teq,inst); break;
		case(cmp): res = res | ass_computeNoRes(cmp,inst); break;
		case(andeq): res = 0; break;
		case(lsl)  : res = res | ass_lsl(inst); break;
	}

	return res;
}
