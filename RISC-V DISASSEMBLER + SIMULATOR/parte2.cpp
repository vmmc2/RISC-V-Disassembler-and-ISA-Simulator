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
  switch(instruction.opcode) { //TEM QUE DAR SWITCH NO OPCODE DAS INSTRUCOES...
    case 0x03:
      execute_load(instruction, processor, memory); //LB, LH, LW
      break;
    case 0x13:
      execute_itype_except_load(instruction, processor); //ADDI, SLLI, SLTI, XORI, SRLI, SRAI, ORI, ANDI
      break;
    case: 0x17//AUIPC
      execute_auipc(instruction, processor, memory);
      break;
    case 0x67: //JALR
      execute_jalr(instruction, processor);
      break;
    case 0x33: //R-TYPE: ADD, SUB, SLL, SLT, XOR, SRL, SRA, OR, AND
      execute_rtype(instruction, processor);
      break;  
    case 0x63: //SB-TYPE: BEQ, BNE, BLT, BGE
      execute_branch(instruction, processor);
      break;
    case 0x6f: // UJ-TYPE: JAL
      execute_jal(instruction, processor);
      break;
    case 0x23: //S-TYPE: SB, SH, SW
      execute_store(instruction, processor, memory);
      break;
    case 0x37: //U-TYPE: LUI
      execute_lui(instruction, processor);
      break;
    case 0x73: //ECALL
      execute_ecall(instruction, memory);
      break;
    default: // undefined opcode
      handle_invalid_instruction(instruction);
      exit(-1);
      break;
  }
}


void execute_rtype(Instruction instruction, Processor *processor) {
  switch(instruction.rtype.funct3) {
    case 0x0: //add - sub - OK
      if(instruction.rtype.func7 == 0x00){ //add
        //No caso do add eu faco a adicao considerando rs1 e rs2 como numeros
        //com sinal, entretanto o resultado que eu jogo no rd eh sem sinal..
        processor->R[instruction.rtype.rd] = (uword)((word)processor->R[instruction.rtype.rs1] + (word)processor->R[instruction.rtype.rs2]);
      }else{ //sub
        processor->R[instruction.rtype.rd] = (uword)((word)processor->R[instruction.rtype.rs1] - (word)processor->R[instruction.rtype.rs2]);
      }
      break;
    case 0x1: //sll - OK
      processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rs1] << processor->R[instruction.rtype.rs2];
      break;
    case 0x2: //slt - OK
      if((word)processor->R[instruction.rtype.rs1] < (word)processor->R[instruction.rtype.rs2]){
        processor->R[instruction.rtype.rd] = 1;
      }else{
        processor->R[instruction.rtype.rd] = 0;
      }
      break;
    case 0x4: //xor - OK
      processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rs1] ^ processor->R[instruction.rtype.rs2];
      break;
    case 0x5: //srl - sra - OK
      if(instruction.rtype.funct7 == 0x00){ //srl - bits vazios preenchidos com zero
        processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rs1] >> processor->R[instruction.rtype.rs2];
      }else{ //sra - bits vazios preenchidos com o bits mais significativo
        processor->R[instruction.rtype.rd] = ((word)processor->R[instruction.rtype.rs1]) >> processor->R[instruction.rtype.rs2];
      }
      break;
    case 0x6: //or - OK
      processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rs1] | processor->R[instruction.rtype.rs2];
      break;
    case 0x7: //and - OK
      processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rs1] & processor->R[instruction.rtype.rs2];
      break;
    case 0x3: //sltu - OK
      if(processor->R[instruction.rtype.rs1] < processor->R[instruction.rtype.rs2]){
        processor->R[instruction.rtype.rd] = 1;
      }else{
        processor->R[instruction.rtype.rd] = 0;
      }
      break;
    default:
      handle_invalid_instruction(instruction);
      exit(-1);
      break;
  }
  processor->PC += 4; //incrementando PC para a proxima instrucao.
}


void execute_itype_except_load(Instruction instruction, Processor *processor) {
  switch(instruction.itype.funct3) { //damos switch no campo funct3
    case 0x0: //addi - OK
      //DETALHE DE IMPLEMENTACAO: QUANDO A GENTE FOR PEGAR UM VALOR DE UM REG SOURCE (RS1, RS2) A GENTE TEM QUE
      //DAR UM CAST NELE PARA CONSIDERAR ELE COM SINAL, BEM COMO DAR O SIGN EXTEND NO IMEDIATO DE 12 BITS
      processor->R[instruction.itype.rd] = (uword)((word)processor->R[instruction.itype.rs1] + bitSigner(instruction.itype.imm, 12));
      break;
    case 0x2: //slti - OK
      if((word)processor->R[instruction.itype.rs1] < (word)bitSigner(instruction.itype.imm, 12)){
        processor->R[instruction.itype.rd] = 1;
      }else{
        processor->R[instruction.itype.rd] = 0;
      }
      break;
    case 0x3: //sltiu - OK
      if(processor->R[instruction.itype.rs1] < (uword)bitSigner(instruction.itype.imm, 12)){
        processor->R[instruction.itype.rd] = 1;
      }else{
        processor->R[instruction.itype.rd] = 0;
      }
      break;
    case 0x4: //xori - OK
      processor->R[instruction.itype.rd] = processor->R[instruction.itype.rs1] ^ bitSigner(instruction.itype.imm, 12);
      break;
    case 0x6: //ori - OK
      processor->R[instruction.itype.rd] = processor->R[instruction.itype.rs1] | bitSigner(instruction.itype.imm, 12);
      break;
    case 0x7: //andi - OK
      processor->R[instruction.itype.rd] = processor->R[instruction.itype.rs1] & bitSigner(instruction.itype.imm, 12);
      break;
    case 0x1: //slli - OK
      processor->R[instruction.itype.rd] = processor->R[instruction.itype.rs1] << bitSigner(instruction.itype.imm, 12);
      break;
    case 0x5: //srli - srai - 
      int shiftoperator = (instruction.itype.imm & 0x01f);
      if((instruction.itype.imm & 0xfe0) == 0x00){ //srli
        processor->R[instruction.itype.rd] = processor->R[instruction.itype.rs1] >> shiftoperator;
      }else{ //srai
        processor->R[instruction.itype.rd] = (uword)((word)processor->R[instruction.itype.rs1] >> shiftoperator);
      }
      break;
    default:
        handle_invalid_instruction(instruction);
        break;
  }
  processor->PC += 4; //incrementando PC para a proxima instrucao.
}


void execute_ecall(Processor *p, ubyte *memory) {
  //INSTRUCAO DE TRANSFERENCIA DE CONTROLE DE AMBIENTE (PARA O SISTEMA OPERACIONAL)
  //a0 = 1 is print value of a1 as an integer. a0 = x10
  //a0 = 10 is exit or end of code indicator.  a1 = x11
  switch(p->R[10]) {
    case 1:
      printf("x11 = %d\n", p->R[11]);
      p->pc += 4;
      break;
    case 10:
      printf("Exiting/End Of Code!\n");
      exit(0);
      break;
    default:
      printf("Illegal ecall number %d\n", -1);
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
  switch(instruction.itype.funct3) { //Manda o switch no campo funct3
    case 0x0: //lb
      
      break;
    case 0x1: //lh
      
      break;
    case 0x2: //lw

      break;
    case 0x4: //lbu -- TEM QUE VER COMO VOU FAZER COM ISSO AQUI

      break;
    case 0x5: //lhu -- TEM QUE VER COMO VOU FAZER COM ISSO AQUI
      
      break;
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
  /* FUNCAO RESPONSAVEL POR CARREGAR UM DADO DO REGISTRADOR NA MEMORIA */
  //Lembrar que a memoria funciona de acordo com o padrao Little-Endian
  switch(alignment){
    case LENGTH_BYTE: //tem que carregar 1 byte na memoria.
      memory[addss] = (ubyte)(value & 0x000000ff);
      break;
    case LENGTH_HALF_WORD: //tem que carregar 2 bytes na memoria
      memory[addss] = (ubyte)(value & 0x000000ff)
      memory[addss + 1] = (ubyte)((value & 0x0000ff00) >> 8);
      break;
    case LENGTH_WORD: //tem que carregar 4 bytes na memoria
      memory[addss] = (ubyte)(value & 0x000000ff)
      memory[addss + 1] = (ubyte)((value & 0x0000ff00) >> 8)
      memory[addss + 2] = (ubyte)((value & 0x00ff0000) >> 16)
      memory[addss + 3] = (ubyte)((value & 0xff000000) >> 24)
      break;
    default:
      break;
  }
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
