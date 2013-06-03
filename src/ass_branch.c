/*
 * ass_branch.c
 *
 *  Created on: 3 Jun 2013
 *      Author: hj512
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "utils.h"

enum cond {
	beq = 0,
	bne = 1,
	bge = 10,
	blt = 11,
	bgt = 12,
	ble = 13,
	b = 15
};

uint32_t setBinCond(char *cond) {
	if (!strcmp(cond, "beq")) {
		return 0;
	} if (!strcmp(cond, "bne")) {
		return bne << 28;
	} if (!strcmp(cond, "bge")) {
		return bge << 28;
	} if (!strcmp(cond, "blt")) {
		return blt << 28;
	} if (!strcmp(cond, "bgt")) {
		return bgt << 28;
	} if (!strcmp(cond, "ble")) {
		return ble << 28;
	} if (!strcmp(cond, "b") | !strcmp(cond, "bal")) {
		return b << 28;
	}
	perror("Condition not recognised");
	exit(EXIT_FAILURE);
}

uint32_t calcOffset(char c) {
	return 0;
}



uint32_t ass_branch(char *inst[]) {
	uint32_t bin = 0x0;
	uint32_t binCond = setBinCond(inst[0]);
	uint32_t offset = calcOffset(inst[1]);
	setBit(bin, 27, 1);
	setBit(bin, 26, 0);
	setBit(bin, 25, 1);
	setBit(bin, 24, 0);

	return bin | binCond | offset;
}
