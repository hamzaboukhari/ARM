/*
 * data_transfer.h
 *
 *  Created on: 29 May 2013
 *      Author: Pavan
 */

#ifndef DATA_TRANSFER_H_
#define DATA_TRANSFER_H_

uint32_t get_dest(uint32_t inst);
uint32_t get_source(uint32_t inst);

void data_transfer(uint32_t inst, state *s);


#endif /* DATA_TRANSFER_H_ */
