/*
 * ass_mutliply.c
 *
 *  Created on: 3 Jun 2013
 *      Author: hj512
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "utils.h"


uint32_t ass_multiply(char *inst[]) {
	uint32_t bin = 0x0;
	setBits(bin, 29, 31, 1);
	setBits(bin, 22, 28, 0);
	setBit(bin, 20, 0);
	setBit(bin, 7, 1);
	setBits(bin, 5, 6, 0);
	setBit(bin, 4, 1);
	int RD = detectReg(inst[1]);
	uint32_t binRD = setBinary(RD, 16);
	int RM = detectReg(inst[2]);
	uint32_t binRM = setBinary(RM, 0);
	int RS = detectReg(inst[3]);
	uint32_t binRS = setBinary(RS, 8);
	if (!strcmp(inst[0], "mul")) {
		setBit(bin, 21, 0);
		return bin | binRD | binRM | binRS;
	} else if (!strcmp(inst[0], "mla")) {
		setBit(bin, 21, 1);
		int RN = detectReg(inst[3]);
		uint32_t binRN = setBinary(RN, 12);
		return bin | binRD | binRM | binRS | binRN;
	} else {
		perror("Not multiply");
		exit(EXIT_FAILURE);
	}

}
