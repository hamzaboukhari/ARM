/*
 * multiply_methods.c
 *
 *  Created on: 29 May 2013
 *      Author: Pavan
 */


#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "multiply.h"
#include "utils.h"

int getAccumulate(uint32_t i) {
	return bitCheck(i, 21);
}

uint32_t getDestReg(uint32_t inst){
 return getBits(inst,16,19);
}

uint32_t binaryMultiply(uint32_t op1, uint32_t op2) {
	int bits = sizeof(int) * 8;
    uint32_t mask = 0;
    uint32_t temp = 0;
    uint32_t result = 0;

    for (int i = 0; i < bits; i++) {
        mask = 1 << i;
        if ((op1 & mask) == mask) {
             temp = op2 << i;
             result += temp;
        }
    }

    return result;
}

void multiply(uint32_t i, state *s) {
	//s -> reg[12] = 0;
	//s -> reg[0] = 0xFFFFFFFF;
	//s -> reg[1] = 0xFFFFFFFD;
	//s -> reg[2] = 0x0000000F;
	uint32_t op1 = s -> reg[getBits(i, 0, 3)];
	uint32_t op2 = s -> reg[getBits(i, 8, 11)];
	uint32_t op3 = s -> reg[getBits(i, 12, 15)];
	uint32_t RD = getDestReg(i);
	uint32_t result = 0;
	printf("Reg val before: 0x%x\n",s -> reg[RD]);

	if (getAccumulate(i) == 0) {
		 result = binaryMultiply(op1, op2);
		 s -> reg[RD] = result;
	} else {
		 result = addBinary(binaryMultiply(op1, op2), op3);
		 s -> reg[RD] = result;
	}

	updateNZinCPSR(s, result, bitCheck(i, 20));
	printf("Reg val after: 0x%x\n",s -> reg[RD]);
}


