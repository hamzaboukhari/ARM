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
		//printf("Found a constant\n");
		return 1;
	}else{
		//printf("Found a register\n");
		return 0;
	}
}

uint32_t calcOp2(char *op2){

	if(isConst(op2)){
		//Is a constant
		//printf("%s is a constant: %i\n",op2, getConst(op2));
		uint32_t op2Val = getConst(op2);
		int rotate = 0;
		if(op2Val > 255){
			for(int i=0;bitCheck(op2Val,0)==0 && bitCheck(op2Val,1)==0;i++){
				printf("Rotate Val: %i\n",rotate);
				rotate += 2;
				op2Val >>= 2;
				//printBits(op2Val);
			}
		}
		uint32_t rot = (rotate >> 1) << 8;
		rot = ~rot + 1;
		rot = rot & 0xF00;
		return rot | op2Val;
		//return op2Val;
	}else{
		//Is a register
		return getConst(op2);
	}
}

uint32_t ass_computeRes(int cmd, char *inst[]){
	uint32_t OpCode = cmd << 21;
	printf("INST1 DP:%s\n",inst[1]);
	printf("INST2 DP:%s\n",inst[2]);
	printf("INST3 DP:%s\n\n",inst[3]);
	uint32_t Rd = getConst(inst[1]) << 12;
	uint32_t Rn = getConst(inst[2]) << 16;
	uint32_t Op2 = calcOp2(inst[3]);
	int I = evalI(inst[3]);

	uint32_t instruction = OpCode | Rd | Rn | Op2;

	instruction = setBit(instruction,25,I);

    printBits(instruction);
	return instruction;
}

uint32_t ass_computeNoRes(int cmd, char *inst[]){
	printf("Operand 1:%s\n",inst[1]);
	printf("Operand 2:%s\n",inst[2]);
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
		case(sub):printf("RES BEFORE: \n");printBits(res);res = res | ass_computeRes(sub,inst); 	printf("GENERATED INSTRUCTION SUB: ");
	    printBits(res); break;
		case(rsb): res = res | ass_computeRes(rsb,inst); break;
		case(and): res = res | ass_computeRes(and,inst); break;
		case(eor): res = res | ass_computeRes(eor,inst); break;
		case(orr): res = res | ass_computeRes(orr,inst); break;
		case(mov): res = res | ass_mov(inst);break;
		case(tst): res = res | ass_computeNoRes(tst,inst); break;
		case(teq): res = res | ass_computeNoRes(teq,inst); break;
		case(cmp): res = res | ass_computeNoRes(cmp,inst);
		printf("GENERATED INSTRUCTION: COMP ");
	    printBits(res); break;break;
		case(andeq): res = 0; break;
		case(lsl)  : res = res | ass_lsl(inst); break;
	}

	return res;
}
