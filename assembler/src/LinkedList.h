/*
 * LinkedList.h
 *
 *  Created on: 5 Jun 2013
 *      Author: pp2112
 */

#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct List_Node{
 char *key;
 int type;
 uint32_t value;
 struct List_Node *prev;
 struct List_Node *next;
}node_t;


typedef struct List{
 node_t *head;
 node_t *foot;
}table_t;

typedef node_t *iterator;

enum DPCommands {
//integer represents OpCode bits
and = 0,
eor = 1,
sub = 2,
rsb = 3,
ADD = 4,
orr = 12,
mov = 13,
tst = 8,
teq = 9,
cmp = 10,

//integer are simply unique values
lsl = 14,
andeq = 15
};

enum MCommands {
//integers represents A (Accumulate) bit
mul,
mla
};

enum SDTCommands {
//integer represents L bit
ldr = 0,
str = 1
};

enum BCommands {
//integer represents Cond bits
beq = 0,
bne = 1,
bge = 10,
blt = 11,
bgt = 12,
ble = 13,
b = 14
};

enum type {
	Data_Processing = 0,
	Multiply = 1,
	Data_Transfer = 2,
	Branch = 3,
	Label = 5
};

#endif /* LINKEDLIST_H_ */
