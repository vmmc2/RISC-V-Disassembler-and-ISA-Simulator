#include <bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>
#include "tipos.h"
#include "utils.h"

//DECLARACOES DAS FUNCOES//
//1) FUNCOES QUE VAO PRINTAR AS INSTRUCOES EM ASM DE FATO NO ARQUIVO DE SAIDA GERADO
void print_rtype(char *, Instruction);
void print_itype_except_load(char *, Instruction, int);
void print_load(char *, Instruction);
void print_store(char *, Instruction);
void print_branch(char *, Instruction);
//2) FUNCOES QUE VAO EXECUTAR AS INSTRUCOES DE FATO NO ROLE.
void write_rtype(Instruction); //ok
void write_itype_except_load(Instruction); //OK
void write_load(Instruction); //OK
void write_store(Instruction); //ok
void write_branch(Instruction); //ok
void write_auipc(Instruction); //ok
void write_lui(Instruction); //ok
void write_jalr(Instruction); //OK
void write_jal(Instruction); //ok
void write_ecall(Instruction); //ok


//INSTRUCOES PRESENTES MAS TEM QUE PENSAR COMO IMPLEMENTAR...:
/*
LBU, LHU, SLTIU, SLTU, BLTU, BGEU
*/


//DESCRICAO DE TODAS AS FUNCOES DE FORMA DETALHADA//

void decode_instruction(Instruction instruction) {
  /* YOUR CODE HERE: COMPLETE THE SWITCH STATEMENTS */
  //Lembra que Instruction eh uma variavel do tipo Union.
  //Ou seja, ela muda dinamicamente seu tipo de boa.
  switch(instruction.opcode) { // TEM QUE DAR SWITCH NO OPCODE DAS INSTRUCOES. PORQUE OQ CARACTERIZA
  //CADA TIPO DE INSTRUCAO EH O SEU OPCODE.
  //SWITCH NO OPCODE. 
    /* YOUR CODE HERE */
    case 0x03:
      write_load(instruction); //LB, LH, LW
      break;
    case 0x13:
      write_itype_except_load(instruction); //ADDI, SLLI, SLTI, XORI, SRLI, SRAI, ORI, ANDI
      break;
    case: 0x17//AUIPC
      write_auipc(instruction);
      break;
    case 0x67: //JALR
      write_jalr(instruction);
      break;
    case 0x33: //R-TYPE: ADD, SUB, SLL, SLT, XOR, SRL, SRA, OR, AND
      write_rtype(instruction);
      break;  
    case 0x63: //SB-TYPE: BEQ, BNE, BLT, BGE
      write_branch(instruction);
      break;
    case 0x6f: // UJ-TYPE: JAL
      write_jal(instruction);
      break;
    case 0x23: //S-TYPE: SB, SH, SW
      write_store(instruction);
      break;
    case 0x37: //U-TYPE: LUI
      write_lui(instruction);
      break;
    case 0x73: //ECALL
      write_ecall(instruction);
      break;
    default: // undefined opcode
      handle_invalid_instruction(instruction);
      break;
  }
  return;
}


void write_rtype(Instruction instruction){
  char *name;
  switch(instruction.rtype.funct3){
    case 0x0: //add - sub
      if(instruction.rtype.func7 == 0x00){
        name = "add";
      }else{
        name = "sub";
      }
      break;
    case 0x1: //sll
      name = "sll";
      break;
    case 0x2: //slt
      name = "slt";
      break;
    case 0x4: //xor
      name = "xor";
      break;
    case 0x5: //srl - sra
      if(instruction.rtype.funct7 == 0x00){
        name = "srl";
      }else{
        name = "sra";
      }
      break;
    case 0x6: //or
      name = "or";
      break;
    case 0x7: //and
      name = "and";
      break;
    case 0x3: //sltu --- TEM QUE VER COMO VAI IMPLEMENTAR ISSO AQUI
      //OBS OBS
      //OBS OBS
      //OBS OBS
      name = "sltu";
      break;
    default:  
      handle_invalid_instruction(instruction);
      break;
  }
  print_rtype(name, instruction);
  return;
}


void write_itype_except_load(Instruction instruction){
  int shiftoperator = -1;
  switch(instruction.itype.funct3) { // VAI TER QUE DAR SWITCH NO FUNCT3 POREM ELE N EH 
  //SUFICIENTE PARA CERTOS CASOS. COMO VEREMOS..
    case 0x0: //addi - ok
      print_itype_except_load("addi", instruction, bitSigner(instruction.itype.imm, 12));
      break;
    case 0x2: //slti - ok
      print_itype_except_load("slti", instruction, bitSigner(instruction.itype.imm, 12));
      break;
    case 0x3: //sltiu - ok
      print_itype_except_load("sltiu", instruction, bitSigner(instruction.itype.imm, 12));
      break;
    case 0x4: //xori - ok
      print_itype_except_load("xori", instruction, bitSigner(instruction.itype.imm, 12));
      break;
    case 0x6: //ori - ok
      print_itype_except_load("ori", instruction, bitSigner(instruction.itype.imm, 12));
      break;
    case 0x7: //andi - ok
      print_itype_except_load("andi", instruction, bitSigner(instruction.itype.imm, 12));
      break;
    case 0x1: //slli - ok
      print_itype_except_load("slli", instruction, bitSigner(instruction.itype.imm, 12));
      break;
    case 0x5: //srli - srai - ok
      shiftoperator = (instruction.itype.imm & 0x01f);
      if((instruction.itype.imm & 0xfe0) == 0x00){
        print_itype_except_load("srli", instruction, shiftoperator);
      }else{
        print_itype_except_load("srai", instruction, shiftoperator);
      }
      break;
    default:
      handle_invalid_instruction(instruction);
      break;
  }
  return;
}


void write_load(Instruction instruction) { //OK
  switch(instruction.itype.funct3) { // What do we switch on?
    case 0x0: //lb
      print_load("lb", instruction);
      break;
    case 0x1: //lh
      print_load("lh", instruction);
      break;
    case 0x2: //lw
      print_load("lw", instruction);
      break;
    case 0x4: //lbu -- TEM QUE VER COMO VOU FAZER COM ISSO AQUI
      print_load("lbu", instruction);
      break;
    case 0x5: //lhu -- TEM QUE VER COMO VOU FAZER COM ISSO AQUI
      print_load("lhu", instruction);
      break;
    default:
      handle_invalid_instruction(instruction);
      break;
  }
  return;
}


void write_store(Instruction instruction) { //OK
  switch(instruction.stype.funct3) { // O OPCODE DELAS SAO TODOS IGUAIS, AI A GENTE ANALISA O FUNCT3
    case 0x0: //sb
      print_store("sb", instruction);
      break;
    case 0x1: //sh
      print_store("sh", instruction);
      break;
    case 0x2: //sw
      print_store("sw", instruction);
      break;
    default:
      handle_invalid_instruction(instruction);
      break;
  }
  return;
}


void write_branch(Instruction instruction){ //ok
  switch(instruction.sbtype.funct3){
    case 0x0: //beq
      print_branch("beq", instruction);
      break;
    case 0x1: //bne
      print_branch("bne", instruction);
      break;
    case 0x4: //blt -- TEM QUE VER COMO VOU FAZER COM ISSO AQUI
      print_branch("blt", instruction);
      break;
    case 0x5: //bge -- TEM QUE VER COMO VOU FAZER COM ISSO AQUI
      print_branch("bge", instruction);
      break;
    case 0x6: //bltu -- TEM QUE VER COMO VOU FAZER COM ISSO AQUI
      print_branch("bltu", instruction);
      break;
    case 0x7: //bgeu -- TEM QUE VER COMO VOU FAZER COM ISSO AQUI
      print_branch("bgeu", instruction);
      break;
    default:
      handle_invalid_instruction(instruction);
      break;
  }
  return;
}

/* For the writes, probably a good idea to take a look at utils.h */

void write_auipc(Instruction instruction) {
  /* YOUR CODE HERE */
  /*
  */
}


void write_lui(Instruction instruction) {
  printf(LUI_FORMAT, instruction.utype.rd, (sWord)instruction.utype.imm);
}


void write_jalr(Instruction instruction) {
  /* YOUR CODE HERE */
  /*
  */
}


void write_jal(Instruction instruction) {
  printf(JAL_FORMAT, instruction.ujtype.rd, get_jump_offset(instruction));
}


void write_ecall(Instruction instruction) {
  printf(ECALL_FORMAT);
  return;
}


//DAQUI PRA BAIXO SAO AS FUNCOES DE PRINT
//TODAS JA FORAM IMPLEMENTADAS...


void print_rtype(char *name, Instruction instruction) {
  printf(RTYPE_FORMAT, name, instruction.rtype.rd, instruction.rtype.rs1, instruction.rtype.rs2);
}


void print_itype_except_load(char *name, Instruction instruction, int imm) {
  printf(ITYPE_FORMAT, name, instruction.itype.rd, instruction.itype.rs1, (word) imm);
}


void print_load(char *name, Instruction instruction) {
  // EXAMPLES
  // lw x1, 0(x4)
  // lw x5, 24(x8)
  printf(MEM_FORMAT, name, instruction.itype.rd,(word) bitSigner(instruction.itype.imm, 12), instruction.itype.rs1);
}


void print_store(char *name, Instruction instruction) {
  // EXAMPLES
  //sw x3, 12(x4)
  //sb x4, 34(x8)
  printf(MEM_FORMAT, name, instruction.stype.rs2, get_store_offset(instruction), instruction.stype.rs1);
}


void print_branch(char *name, Instruction instruction) {
  printf(BRANCH_FORMAT, name, instruction.sbtype.rs1, instruction.sbtype.rs2, get_branch_offset(instruction));
}
