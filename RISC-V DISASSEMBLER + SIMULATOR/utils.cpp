#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <bits/stdc++.h>

//A funcao a seguir serve para fazer extensao de sinal de um campo de bits
int bitSigner(unsigned int field, unsigned int size){
    int value = field;
    value = ((value << (32 - size)) >> (32 - size));
    return value;
}

/*AS FUNCOES DE OFFSET DEVEM RETORNAR O TAMANHO DO OFFSET EM BYTES*/
int get_branch_offset(Instruction instruction) {
  int offset = 0x00000000;
  offset = offset | (0x0000001e & instruction.sbtype.imm5);
  offset = offset | (0x000007e0 & (instruction.sbtype.imm7 << 5));
  offset = offset | (0x00000800 & (instruction.sbtype.imm5 << 11));
  offset = offset | (0x00001000 & (instruction.sbtype.imm7 << 6));
  return bitSigner(offset, 13);
}


int get_jump_offset(Instruction instruction) {
  int offset = 0x00000000;
	offset |= (instruction.ujtype.imm >> 8) & 0x000007fe; // imm[1:10]
	offset |= (instruction.ujtype.imm << 3) & 0x00000800; // imm[11]
	offset |= (instruction.ujtype.imm << 12) & 0x000ff000; // imm[12:19]
	offset |= (instruction.ujtype.imm << 2) & 0x00100000;// imm[20]
	return bitSigner(offset, 21);
}


int get_store_offset(Instruction instruction) {
  int offset = 0x00000000;
  offset = offset | instruction.stype.imm7;
  offset = offset << 5;
  offset = offset | instruction.stype.imm5;
  return bitSigner(offset, 12);
}


void handle_invalid_instruction(Instruction instruction) {
  printf("Invalid Instruction: 0x%08x\n", instruction.bits);
  return;
}


void handle_invalid_read(address addss) {
  printf("Bad Read. Address: 0x%08x\n", addss);
  exit(-1);
}


void handle_invalid_write(address addss) {
  printf("Bad Write. Address: 0x%08x\n", addss);
  exit(-1);
}
