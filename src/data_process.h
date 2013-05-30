/*
 * data_process.h
 *
 *  Created on: 29 May 2013
 *      Author: Hakeem
 */

#ifndef DATA_PROCESS_H_
#define DATA_PROCESS_H_

uint32_t rotateRight(uint32_t i, int bits);
uint32_t zero_extend(uint8_t immval);

uint32_t shift(uint32_t op2, int type, int n);
int shiftCarryOut(uint32_t op2, int type, int n);

uint32_t and(state *s, uint32_t op1, uint32_t op2, int Rd);
uint32_t eor(state *s, uint32_t op1, uint32_t op2, int Rd);
uint32_t sub(state *s, uint32_t op1, uint32_t op2, int Rd);
uint32_t rsb(state *s, uint32_t op1, uint32_t op2, int Rd);
uint32_t add(state *s, uint32_t op1, uint32_t op2, int Rd);
uint32_t tst(state *s, uint32_t op1, uint32_t op2);
uint32_t teq(state *s, uint32_t op1, uint32_t op2);
uint32_t cmp(state *s, uint32_t op1, uint32_t op2);
uint32_t orr(state *s, uint32_t op1, uint32_t op2, int Rd);
uint32_t mov(state *s, uint32_t op2, int Rd);

uint8_t getOpCode(uint32_t inst);
void data_process(uint32_t inst, state *s);

#endif /* DATA_PROCESS_H_ */
