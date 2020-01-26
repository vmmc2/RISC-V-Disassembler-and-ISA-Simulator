#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <bits/stdc++.h>

//A funcao a seguir serve para fazer extensao de sinal de um campo de bits
int bitSigner(unsigned int field, unsigned int size){
    /*INSERE CODIGO AQUI*/
    return 0;
}

/*AS FUNCOES DE OFFSET DEVEM RETORNAR O TAMANHO DO OFFSET EM BYTES*/
int get_branch_offset(Instruction instruction) {
  /* YOUR CODE HERE */
  return 0;
}


int get_jump_offset(Instruction instruction) {
  /* YOUR CODE HERE */
  return 0;
}


int get_store_offset(Instruction instruction) {
  /* YOUR CODE HERE */
  return 0;
}


void handle_invalid_instruction(Instruction instruction) {
  printf("Invalid Instruction: 0x%08x\n", instruction.bits);
}


void handle_invalid_read(address addss) {
  printf("Bad Read. Address: 0x%08x\n", addss);
  exit(-1);
}


void handle_invalid_write(address addss) {
  printf("Bad Write. Address: 0x%08x\n", addss);
  exit(-1);
}
