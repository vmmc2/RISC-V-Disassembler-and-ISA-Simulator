#ifndef RISCV_H
#define RISCV_H

#include <types.h>

//Funcao referente a Parte 1: Decodificar uma instrucao
void decode_instruction(Instruction i);

//Funcoes referentes a Parte 2: 
void execute_instruction(Instruction instruction, Processor* processor, ubyte *memory);

void store(ubyte *memory, address endereco, Alignment alignment, uword value, int);

uword load(ubyte *memory, address endereco, Alignment alignment, int);

#endif