/*
 * data_process.h
 *
 *  Created on: 29 May 2013
 *      Author: Pavan
 */

#ifndef DATA_PROCESS_H_
#define DATA_PROCESS_H_

void and(state *st, uint32_t inst,int I, int S);
void eor(state *st, uint32_t inst,int I, int S);
void sub(state *st, uint32_t inst,int I, int S);
void rsb(state *st, uint32_t inst,int I, int S);
void add(state *st, uint32_t inst,int I, int S);
void tst(state *st, uint32_t inst,int I, int S);
void teq(state *st, uint32_t inst,int I, int S);
void cmp(state *st, uint32_t inst,int I, int S);
void orr(state *st, uint32_t inst,int I, int S);
void mov(state *st, uint32_t inst,int I, int S);

uint32_t rotateRight(uint32_t i, int bits);

#endif /* DATA_PROCESS_H_ */
