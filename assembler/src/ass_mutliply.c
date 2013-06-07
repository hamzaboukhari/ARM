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
#include "LinkedList.h"


uint32_t ass_multiply(char *inst[], table_t *table) {

	uint32_t RD = getConst(inst[1]) << 16;
	uint32_t RM = getConst(inst[2]);
	uint32_t RS = getConst(inst[3]) << 8;

	uint32_t bin = 0x0;
	bin = setBits(bin, 29, 31, 1);
	bin = setBits(bin, 22, 28, 0);
	bin = setBit(bin, 20, 0);
	bin = setBit(bin, 7, 1);
	bin = setBits(bin, 5, 6, 0);
	bin = setBit(bin, 4, 1);

	if (!strcmp(inst[0], "mul")) {
		printf("detected mul \n");
		bin = setBit(bin, 21, 0);
		return bin | RD | RM | RS;
	} else if (!strcmp(inst[0], "mla")) {
		printf("detected mla \n");
		bin = setBit(bin, 21, 1);
		int RN = getConst(inst[4]) << 12;
		return bin | RD | RM | RS | RN;
	} else {
		perror("Not multiply");
		exit(EXIT_FAILURE);
	}



 /* int res = getConst(inst[1]);
  printf("Mult test: %d \n",res); */
    return NULL;
}
