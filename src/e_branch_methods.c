/*
 * branch_methods.c
 *
 *  Created on: 29 May 2013
 *      Author: Pavan
 */


#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "utils.h"

void branch(uint32_t i, state *s, cycle *c) {
	uint32_t offset = getBits(i, 0, 23);
	offset = shift(offset, 0, 2);
	offset = setBits(offset, 26, 31, bitCheck(offset, 25));
	s -> PC += offset;
	c -> prev_instr = 0x1;
	c -> current_instr = 0x01;
}



