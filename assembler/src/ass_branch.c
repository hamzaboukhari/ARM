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
#include "LinkedList.h";

uint32_t calcOffset(char *c) {
	uint32_t offset = - 8 - atoi(c);
	offset >>= 2;
	offset = setBits(offset, 25, 31, 0);
	return offset;
}



uint32_t ass_branch(char *inst[], table_t *table) {
	uint32_t bin = 0x0;
	uint32_t binCond = getValue(table,inst[0]) << 28;
	uint32_t offset = calcOffset(inst[2]);
	setBit(bin, 27, 1);
	setBit(bin, 26, 0);
	setBit(bin, 25, 1);
	setBit(bin, 24, 0);

	return bin | binCond | offset;
}
