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

	instruction = setBit(instruction,25,I); //set I

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

	instruction = setBit(instruction,25,I); //set I Bit
	instruction = setBit(instruction,20,1); //set S to 1


	return instruction;
}

uint32_t ass_mov(char *inst[]){
	uint32_t OpCode = mov << 21;
	uint32_t Rd = getConst(inst[1]) << 12;
	uint32_t Op2 = calcOp2(inst[2]);
	int I = evalI(inst[2]);

	uint32_t instruction = OpCode | Rd | Op2;

	instruction = setBit(instruction,25,I); //set I to 1
	instruction = setBit(instruction,20,0); //set S to 0

	return instruction;
}
//1110 00 1 1101 0 0000 0000 000000000010 -- actual
//1110 00 1 1101 0 0000 0000 000000000010 -- generated

//1110 00 0 1010 1 0010 0000 000000000000 -- generated
//1110 00 0 1010 1 0010 0000 000000000000 -- actual

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
	///printf("Executing Data Process...\n");

	int OpCode = getValue(table,inst[0]);
	//printf("Value: %i\n",OpCode);
	uint32_t res = 14 << 28; //set Cond to 1110 (al)


	//Debugger Code:
/*	switch(OpCode){
		case(ADD): printf("Executing add...\n"); break;
		case(sub): printf("Executing sub...\n"); break;
		case(rsb): printf("Executing rsb...\n"); break;
		case(and): printf("Executing and...\n"); break;
		case(eor): printf("Executing eor...\n"); break;
		case(orr): printf("Executing orr...\n"); break;

		case(mov): printf("Executing mov...\n"); break;

		case(tst): printf("Executing tst...\n"); break;
		case(teq): printf("Executing teq...\n"); break;
		case(cmp): printf("Executing cmp...\n"); break;
	} */


	switch(OpCode){
		//instructions that compute results:
		case(ADD): res = res | ass_computeRes(ADD,inst); break;
		case(sub):printf("RES BEFORE: \n");printBits(res);res = res | ass_computeRes(sub,inst); 	printf("GENERATED INSTRUCTION SUB: ");
	printBits(res); break;
		case(rsb): res = res | ass_computeRes(rsb,inst); break;
		case(and): res = res | ass_computeRes(and,inst); break;
		case(eor): res = res | ass_computeRes(eor,inst); break;
		case(orr): res = res | ass_computeRes(orr,inst); break;
		//mov:
		case(mov): res = res | ass_mov(inst);break;
		//instructions that do not compute results:
		case(tst): res = res | ass_computeNoRes(tst,inst); break;
		case(teq): res = res | ass_computeNoRes(teq,inst); break;
		case(cmp): res = res | ass_computeNoRes(cmp,inst);
		printf("GENERATED INSTRUCTION: COMP ");
	printBits(res); break;break;
		//Special Instructions:
		case(andeq): res = 0; break;
		case(lsl)  : res = res | ass_lsl(inst); break;
	}

	return res;

	//1110 00 0 1010 1 0010 0000 000000000000 -- original (cmp);
	//1110 00 1 1010 1 0010 0000 000000000000 -- generated
}

////0xe26a8001 - sub
//1110 00 1 0010 0 0100 0100 000000000001 - actual
//1110 00 1 0010 0 0100 0100 000000000001 - generated


//1110 00 0 0000 0 0000 0000 000000000000
//0000 00 1 0010 0 0100 0100 000000000001
//1110 00 1 0010 0 0100 0100 000000000001

