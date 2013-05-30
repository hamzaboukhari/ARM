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

uint32_t multiply(uint32_t i, state *s) {
	uint32_t op1 = getBits(i, 0, 3);
	uint32_t op2 = getBits(i, 8, 11);
	uint32_t op3 = getBits(i, 12, 15);

	if (getAccumulate(i) == 0) {
		return binaryMultiply(op1, op2);
	} else {
		return addBinary(binaryMultiply(op1, op2), op3);
	}
}


