#include <stdlib.h>
#include <stdint.h>
#include <string.h>

int main(int argc, char **argv) {
  return EXIT_SUCCESS;


  void checkCond(uint32_t i) {
	  int bit31 = bitCheck(i, 31);
	  int bit30 = bitCheck(i, 30);
	  int bit29 = bitCheck(i, 29);
	  int bit28 = bitCheck(i, 28);

	  char cond;
	  sprintf(cond, "%i%i%i%i", bit31, bit30, bit29, bit28);
	  int CPSR[32];

	  if (strcmp(cond, '0000') == 0) {
		  if (CPSR[1] == 1) {
			  checkInstruction(i);
		  }
  	  } else if (strcmp(cond, '0001') == 0) {
  		  if (CPSR[1] == 0) {
  			  checkInstruction(i);
  		  }
  	  } else if (strcmp(cond, '1010') == 0) {
  		  if (CPSR[0] == CPSR[3]) {
  			  checkInstruction(i);
  		  }
  	  } else if (strcmp(cond, '1011') == 0) {
  		  if (CPSR[0] != CPSR[3]) {
  			  checkInstruction(i);
  		  }
  	  } else if (strcmp(cond, '1100') == 0) {
  		  if (CPSR[1] == 0 & CPSR[0] == CPSR[3]) {
  			  checkInstruction(i);
  		  }
  	  } else if (strcmp(cond, '1101') == 0) {
  		  if (CPSR[1] == 1 | CPSR[0] != CPSR[3]) {
  			  checkInstruction(i);
  		  }
  	  } else if (strcmp(cond, '1110') == 0) {
  		      checkInstruction(i);
  	  }

  }


  void checkInstruction(uint32_t i) {
	  int bit27 = bitCheck(i, 27);
	  int bit26 = bitCheck(i, 26);

	  char str;
	  sprintf(str, "%i%i", bit27, bit26);

	  if (strcmp(str, '00') == 0) {
		  //Data Processing/Multiply
	  } else if (strcmp(str, '01') == 0) {
		  //Single Data Transfer
	  } else if (strcmp(str, '10') == 0) {
		  //Branch
	  }

  }

  int binaryToDecimal(uint32_t i) {
	  int sum = 0;
	  for (int k = i ; k < 32 ; k++) {
		if (bitCheck(i, k) == 1) {
		  sum += 2^k;
		}
	  }
	  return sum;

  }

  uint32_t rotateLeft(uint32_t i, int bits) {
    return ((i << bits) | (i >> (32 - bits)));
  }

  uint32_t rotateRight(uint32_t i, int bits) {
    return ((i >> bits) | (i << (32 - bits)));
  }

}
