/*
 * a_symTable.c
 *
 *  Created on: 5 Jun 2013
 *      Author: pp2112
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "a_linked_list.h";



void buildSymTable(table_t *symTable){

 char keys[][100] = {"and", "eor", "sub", "rsb", "add", "orr", "mov", "tst", "teq", "cmp",
		 	 	 	 "mul", "mla", "ldr", "str", "beq", "bne", "bge", "blt", "bgt", "ble",
                     "b", "lsl", "andeq", "Label"};

 int  values[24]    = {and, eor, sub, rsb, add, orr, mov, tst, teq, cmp,
	 	 	           mul, mla, ldr, str, beq, bne, bge, blt, bgt, ble,
                       b, lsl, andeq, Label};

 int  types[24]     = {Data_Processing,Data_Processing,Data_Processing,Data_Processing
		               ,Data_Processing,Data_Processing,Data_Processing,Data_Processing
		               ,Data_Processing,Data_Processing,Multiply,Multiply,Data_Transfer
		               ,Data_Transfer,Branch,Branch,Branch,Branch,Branch,Branch,Branch
		               ,Data_Processing,Data_Processing, Label};
 insertElements(symTable,keys,values,types);

}

