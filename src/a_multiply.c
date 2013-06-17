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
#include "a_linked_list.h";


uint32_t ass_multiply(char *inst[], table_t *table) {

	uint32_t RD = getConst(inst[1]) << 16; //Register Rd address
	uint32_t RM = getConst(inst[2]); //Register Rm address
	uint32_t RS = getConst(inst[3]) << 8; //Register Rs address

	uint32_t bin = 0x0;
	bin = 14 << 28; //set Cond to 1110
	//bin = setBits(bin, 29, 31, 1); //Bits 31 to 29 of Cond
	bin = setBits(bin, 22, 27, 0); //Bits 22 to 27
	bin = setBit(bin, 20, 0); //S bit
	bin = setBit(bin, 7, 1); //Bit 7
	bin = setBits(bin, 5, 6, 0); //Bit 5 and 6
	bin = setBit(bin, 4, 1); //Bit 4

	if (!strcmp(inst[0], "mul")) {
		//printf("detected mul \n");
		bin = setBit(bin, 21, 0); //Set A to 0
		return bin | RD | RM | RS;
	} else if (!strcmp(inst[0], "mla")) {
		//printf("detected mla \n");
		bin = setBit(bin, 21, 1); //Set A to 1
		int RN = getConst(inst[4]) << 12; //Register Rn address
		return bin | RD | RM | RS | RN;
	} else {
		perror("Not multiply instruction");
		exit(EXIT_FAILURE);
	}

}
