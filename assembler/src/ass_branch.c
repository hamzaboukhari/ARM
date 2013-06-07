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
#include "leak_detector_c.h"

enum {
	PC_OFFSET = 8
};

uint32_t calcOffset(char *c) {
	uint32_t offset = atoi(c) - PC_OFFSET; //Offset between current address and label, taking into account 8 byte PC offset
	offset >>= 2; //Offset shifted right two bits
	offset = setBits(offset, 24, 31, 0); //Offset extended to 32 bits
	return offset;
}



uint32_t ass_branch(char *inst[], table_t *table) {
	uint32_t bin = 0x0;
	uint32_t binCond = getValue(table,inst[0]) << 28; //Cond is bits 31 to 28 so it is shifted 28
	uint32_t offset = calcOffset(inst[2]);
	bin = setBit(bin, 27, 1); //Bits 27, 26, 25 set to 101
	bin = setBit(bin, 26, 0); //Bits 27, 26, 25 set to 101
	bin = setBit(bin, 25, 1); //Bits 27, 26, 25 set to 101
	bin = setBit(bin, 24, 0); //Bit 24 set to 0
	return bin | binCond | offset;
}
