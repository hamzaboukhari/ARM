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
#include "a_linked_list.h";

enum {
	PC_OFFSET = 4
};

uint32_t calcOffset(char *c) {
	printf("%i \n", atoi(c));
	uint32_t offset = atoi(c) - PC_OFFSET; //Offset between current address and label, taking into account 8 byte PC offset
	printf("%i \n", offset);
	printBits(offset);
	offset >>= 2; //Offset shifted right two bits
	printBits(offset);
	offset = setBits(offset, 24, 31, 0); //Offset extended to 32 bits
	printBits(offset);
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
