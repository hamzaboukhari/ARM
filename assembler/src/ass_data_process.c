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
		//printf("%s is a constant: %i\n",op2, getConst(op2));
		uint32_t op2Val = getConst(op2);
		int rotate = 0;
		if(op2Val > 255){
			for(int i=0;bitCheck(op2Val,0)==0;i++){
				rotate++;
				op2Val >>= 1;
				//printBits(op2Val);
			}
		}
		uint32_t rot = rotate << 7;
		return rot | op2Val;
		//return op2Val;
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

	instruction = setBit(instruction,25,I); //set I

	return instruction;
}

uint32_t ass_computeNoRes(int cmd, char *inst[]){
	uint32_t OpCode = cmd << 21;
	uint32_t Rn = getConst(inst[1]) << 16;
	uint32_t Op2 = calcOp2(inst[2]);
	int I = evalI(inst[2]);

	uint32_t instruction = OpCode | Rn | Op2;

	instruction = setBit(instruction,25,1); //set I to 1
	instruction = setBit(instruction,20,1); //set S to 1

	return instruction;
}

uint32_t ass_mov(char *inst[]){
	//printf("MOV:");
	uint32_t OpCode = mov << 21;
	uint32_t Rd = getConst(inst[1]) << 12;
	printf("RD: %s = %i \n",inst[1], Rd >> 12);
	uint32_t Op2 = calcOp2(inst[2]);
	int I = evalI(inst[2]);

	uint32_t instruction = OpCode | Rd | Op2;

	instruction = setBit(instruction,25,I); //set I to 1

	return instruction;
}

uint32_t ass_lsl(char *inst[]){
	uint32_t OpCode = mov << 21;
	uint32_t Rn1 = getConst(inst[1]) << 12;
	uint32_t Rn2 = getConst(inst[1]);
	uint32_t Op2Val = getConst(inst[2]) << 8;

	uint32_t instruction = OpCode | Rn1 | Rn2 | Op2Val;

	return instruction;
}

uint32_t ass_data_process(char *inst[], table_t *table){
	///printf("Executing Data Process...\n");

	int OpCode = getValue(table,inst[0]);
	//printf("Value: %i\n",OpCode);
	uint32_t res = 14 << 28; //set Cond to 1110 (al)

/*
	//Debugger Code:
	switch(OpCode){
		case(add): printf("Executing add...\n"); break;
		case(sub): printf("Executing sub...\n"); break;
		case(rsb): printf("Executing rsb...\n"); break;
		case(and): printf("Executing and...\n"); break;
		case(eor): printf("Executing eor...\n"); break;
		case(orr): printf("Executing orr...\n"); break;

		case(mov): printf("Executing mov...\n"); break;

		case(tst): printf("Executing tst...\n"); break;
		case(teq): printf("Executing teq...\n"); break;
		case(cmp): printf("Executing cmp...\n"); break;
	}
*/

	switch(OpCode){
		//instructions that compute results:
		case(Add): res = res | ass_computeRes(Add,inst); break;
		case(sub): res = res | ass_computeRes(sub,inst); break;
		case(rsb): res = res | ass_computeRes(rsb,inst); break;
		case(and): res = res | ass_computeRes(and,inst); break;
		case(eor): res = res | ass_computeRes(eor,inst); break;
		case(orr): res = res | ass_computeRes(orr,inst); break;
		//mov:
		case(mov): res = res | ass_mov(inst); break;
		//instructions that do not compute results:
		case(tst): res = res | ass_computeNoRes(tst,inst); break;
		case(teq): res = res | ass_computeNoRes(teq,inst); break;
		case(cmp): res = res | ass_computeNoRes(cmp,inst); break;
		//Special Instructions:
		case(andeq): res = 0; break;
		case(lsl)  : res = res | ass_lsl(inst); break;
	}

	return res;

}
