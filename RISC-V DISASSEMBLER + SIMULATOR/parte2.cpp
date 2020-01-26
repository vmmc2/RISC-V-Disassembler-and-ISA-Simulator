#include <stdio.h> // for stderr
#include <stdlib.h> // for exit()
#include "tipos.h"
#include "utils.h"
#include "riscv.h"
#include <bits/stdc++.h>


// forward declarations
void execute_rtype(Instruction, Processor *);
void execute_itype_except_load(Instruction, Processor *);
void execute_branch(Instruction, Processor *);
void execute_jalr(Instruction, Processor *);
void execute_jal(Instruction, Processor *);
void execute_load(Instruction, Processor *, ubyte *);
void execute_store(Instruction, Processor *, ubyte *);
void execute_ecall(Processor *, ubyte *);
void execute_auipc(Instruction, Processor *);
void execute_lui(Instruction, Processor *);


void execute_instruction(Instruction instruction,Processor *processor, ubyte *memory) {
  /* YOUR CODE HERE: COMPLETE THE SWITCH STATEMENTS */
  switch(0) { //TEM QUE DAR SWITCH NO OPCODE DAS INSTRUCOES...
    /* YOUR CODE HERE */
    default: // undefined opcode
      handle_invalid_instruction(instruction);
      exit(-1);
      break;
  }
}


void execute_rtype(Instruction instruction, Processor *processor) {
  switch(0) { // What do we switch on?
    /* YOUR CODE HERE */
    default:
      handle_invalid_instruction(instruction);
      exit(-1);
      break;
  }
}


void execute_itype_except_load(Instruction instruction, Processor *processor) {
  switch(0) { // What do we switch on?
    /* YOUR CODE HERE */
    default:
        handle_invalid_instruction(instruction);
        break;
  }
}


void execute_ecall(Processor *p, ubyte *memory) {
  switch(0) { // What do we switch on?
    /* YOUR CODE HERE */
    default: // undefined ecall
      printf("Illegal ecall number %d\n", -1); // What stores the ecall arg?
      exit(-1);
      break;
  }
}


void execute_branch(Instruction instruction, Processor *processor) {
  /* Remember that the immediate portion of branches
     is counting half-words, so make sure to account for that. */
  switch(0) { // What do we switch on?
    /* YOUR CODE HERE */
    default:
      handle_invalid_instruction(instruction);
      exit(-1);
      break;
  }
}


void execute_load(Instruction instruction, Processor *processor, ubyte *memory) {
  switch(0) { // What do we switch on?
    /* YOUR CODE HERE */
    default:
      handle_invalid_instruction(instruction);
      break;
  }
}


void execute_store(Instruction instruction, Processor *processor, ubyte *memory) {
  switch(0) { // What do we switch on?
    /* YOUR CODE HERE */
    default:
      handle_invalid_instruction(instruction);
      exit(-1);
      break;
  }
}


void execute_jalr(Instruction instruction, Processor *processor) {
  /* YOUR CODE HERE */
}


void execute_jal(Instruction instruction, Processor *processor) {
  /* YOUR CODE HERE */
}


void execute_auipc(Instruction instruction, Processor *processor) {
  /* YOUR CODE HERE */
}


void execute_lui(Instruction instruction, Processor *processor) {
  /* YOUR CODE HERE */
}


/* Checks that the address is aligned correctly */
int check(address addss, Alignment alignment) {
  //ESSA FUNCAO SIMPLESMENTE CHECA SE O ENDERECO DE MEMORIA PASSADO TA DENTRO DOS LIMITES DE MEMORIA QUE ESTAMOS TRABALHANDO.
  if (addss > 0 && addss < MEMORY_SPACE) {
    // byte align
    if (alignment == LENGTH_BYTE) return 1;
    // half align
    if (alignment == LENGTH_HALF_WORD) return ((addss % 2) == 0);
    // word align
    if (alignment == LENGTH_WORD) return ((addss % 4) == 0);
  }
  return 0;
}


void store(ubyte *memory, address addss, Alignment alignment, uword value, int check_align) {
  if ((check_align && !check(addss,alignment)) || (addss >= MEMORY_SPACE)) {
    handle_invalid_write(addss); //JA FOI IMPLEMENTADO
  }
  /* YOUR CODE HERE */
}

//FUNCAO QUE VAI SER USADA PARA RETORNAR UMA INSTRUCAO DA MEMORIA
uword load(ubyte *memory, address addss, Alignment alignment, int check_align) {
  if ((check_align && !check(addss,alignment)) || (addss >= MEMORY_SPACE)) {
    handle_invalid_read(addss); //JA FOI IMPLEMENTADO
  }
  uword instrucao = 0x00000000;
  switch(alignment){ //tem que testar para saber o tamanho do que eu to dando load
  //no caso tem que ver o Alignment alignment
    case LENGTH_BYTE:
      instrucao = (uword)memory[addss];
      break;
    case LENGTH_HALF_WORD:
      instrucao = (uword)memory[addss + 1];
      instrucao = (uword)(instrucao << 8);
      instrucao = (uword)(instrucao | memory[addss]);
      break;
    case LENGTH_WORD:
      instrucao = (uword)memory[addss + 3];
      instrucao = (uword)(instrucao << 8);
      instrucao = (uword)(instrucao | memory[addss + 2]);
      instrucao = (uword)(instrucao << 8);
      instrucao = (uword)(instrucao | memory[addss + 1]);
      instrucao = (uword)(instrucao << 8);
      instrucao = (uword)(instrucao | memory[addss]);
      break;
    default:
      break;
  }
  return instrucao;
}
