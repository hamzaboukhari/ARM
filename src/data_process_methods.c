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


uint32_t shift(uint32_t op2, int type, int n){

	if(type==0){//Logical Left
		op2 = op2 << n;
	} else if(type==1){//Logical Right
		op2 = op2 >> n;
	} else if(type==2){//Arithmetic Right
		op2 = op2 >> n;
		op2 = setBits(op2,32-n,31,bitCheck(op2,32-n));
	} else if(type==3){//Rotate Right
		op2 = rotateRight(op2, n);
	}

	return op2;

}

int shiftCarryOut(uint32_t op2, int type, int n){

	int carryOut;

	if(type==0){//Logical Left
		carryOut = bitCheck(op2,32-n);
	} else if(type==1){//Logical Right
		carryOut = bitCheck(op2,n-1);
	} else if(type==2){//Arithmetic Right
		carryOut = bitCheck(op2,n-1);
	} else if(type==3){//Rotate Right
		carryOut = bitCheck(op2,n-1);
	}

	return carryOut;

}

uint32_t and(state *s, uint32_t op1, uint32_t op2, int Rd){
	uint32_t res = op1 & op2;
	s->reg[Rd] = res;
	return res;
}

uint32_t eor(state *s, uint32_t op1, uint32_t op2, int Rd){
	uint32_t res = op1 ^ op2;
	s->reg[Rd] = res;
	return res;
}

uint32_t sub(state *s, uint32_t op1, uint32_t op2, int Rd){
	uint32_t res = addBinary(op1,negBinary(op2));
	s->reg[Rd] = res;
	return res;
}

uint32_t rsb(state *s, uint32_t op1, uint32_t op2, int Rd){
	uint32_t res = addBinary(op2,negBinary(op1));
	s->reg[Rd] = res;
	return res;
}

uint32_t add(state *s, uint32_t op1, uint32_t op2, int Rd){
	uint32_t res = addBinary(op1,op2);
	s->reg[Rd] = res;
	return res;
}

uint32_t tst(state *s, uint32_t op1, uint32_t op2){
	return op1 & op2;
}

uint32_t teq(state *s, uint32_t op1, uint32_t op2){
	return op1 ^ op2;
}

uint32_t cmp(state *s, uint32_t op1, uint32_t op2){
	return addBinary(op1,negBinary(op2));
}

uint32_t orr(state *s, uint32_t op1, uint32_t op2, int Rd){
	uint32_t res = op1 | op2;
	s->reg[Rd] = res;
	return res;
}

uint32_t mov(state *s, uint32_t op2, int Rd){
	s->reg[Rd] = op2;
	return op2;
}

void data_process(uint32_t inst, state *s){

	 uint32_t op1 = getBits(inst,16,19);
	 uint32_t op2;

	 int opcode = getOpCode(inst);
	 int I = bitCheck(inst, 25);
	 int S = bitCheck(inst, 20);
	 int Rd = getBits(inst,12,15);
	 int N;
	 int Z;
	 int C;
	 uint32_t res;

	 if(I == 1){
		//Operand2 is an immediate value
		op2 = rotateRight(getBits(inst,0,7),2*getBits(inst,8,11));
	 } else {
		//Operand2 is a register
		int Rm = getBits(inst,0,11);
		op2 = s -> reg[Rm];
		if(bitCheck(inst,4)==0){
			//Shift Specified By A Constant Amount
			op2 = shift(op2,
						getBits(inst,5,6),
						getBits(inst,7,11));
			C = shiftCarryOut(op2,
							  getBits(inst,5,6),
							  getBits(inst,7,11));
		}else{
			//optional: Shift Specified By A Register
			op2 = shift(op2,
						getBits(inst,5,6),
						s -> reg[getBits(inst,8,11)]);
			C = shiftCarryOut(op2,
							  getBits(inst,5,6),
							  s -> reg[getBits(inst,8,11)]);
		}
	 }


	 switch(opcode){
	  case(0) : res = and(s,op1,op2,Rd);break;
	  case(1) : res = eor(s,op1,op2,Rd);break;
	  case(2) : res = sub(s,op1,op2,Rd);break;
	  case(3) : res = rsb(s,op1,op2,Rd);break;
	  case(4) : res = add(s,op1,op2,Rd);break;
	  case(8) : res = tst(s,op1,op2);break;
	  case(9) : res = teq(s,op1,op2);break;
	  case(10): res = cmp(s,op1,op2);break;
	  case(12): res = orr(s,op1,op2,Rd);break;
	  case(13): res = mov(s,op2,Rd);break;
	  default: perror("data_process opcode error\n");break;
	 }

	 //debugger:
	 switch(opcode){
	 	  case(0) : printf("DP: and\n");break;
	 	  case(1) : printf("DP: eor\n");break;
	 	  case(2) : printf("DP: sub\n");break;
	 	  case(3) : printf("DP: rsb\n");break;
	 	  case(4) : printf("DP: add\n");break;
	 	  case(8) : printf("DP: tst\n");break;
	 	  case(9) : printf("DP: teq\n");break;
	 	  case(10): printf("DP: cmp\n");break;
	 	  case(12): printf("DP: orr\n");break;
	 	  case(13): printf("DP: mov\n");break;
	 	  default: perror("data_process opcode error\n");break;
	 }printf("Result: ");printHex(res);

	 //Run S Check
	 if(S==1){
		 if(res == 0){
			 Z = 1;
		 }

		 N = bitCheck(res,31);

		 s->CPSR = setBit(s->CPSR,31,N);
		 s->CPSR = setBit(s->CPSR,30,Z);
		 s->CPSR = setBit(s->CPSR,29,C);
	 }


}
