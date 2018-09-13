#ifndef AES_C
#define AES_C 1

#include "aes.h"

Options handleArgs(int arg, char** argv) {
	Options opt;
	return opt;
}

void subBytes(State* state) {}
void shiftRows(State* state) {}
void mixColumns(State* state) {}
void addRoundKey(State* state) {}

//void rotWord(State* state) {}
//void subWord(State* state) {}

void invSubBytes(State* state) {}
void invShiftRows(State* state) {}
void invMixColumns(State* state) {}

#endif
