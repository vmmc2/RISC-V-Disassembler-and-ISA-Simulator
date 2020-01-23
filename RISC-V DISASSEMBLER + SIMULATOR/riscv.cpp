#include "riscv.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <string.h>
#include <assert.h>
#include <bits/stdc++.h>

//Ponteiro que vai aponta para a memoria
//Lembrando que a memoria ta particionada em varias secoes. A parte de dados ta
//colada com a parte de instrucoes e tal.
//Basicamente a gente vai alocar um array dinamicamente e vai fazer *memory apontar
//la. Entao, memory vai funcionar como um array alocado dinamicamente.
ubyte *memory;

//Funcao que vai executar as instrucoes..
void execute(Processor *processor, int print){
    Instruction instruction;
    //ETAPA 1: BUSCAR UMA INSTRUCAO
    instruction.bits = load(memory, processor->PC, LENGTH_WORD, 1);
    //Terminou de buscar a instrucao... Printa o valor presente no registrador PC
    //Pra saber em qual posicao da memoria de instrucoes eu me encontro
    printf("%08x: ", processor->PC);
    //Peguei a instrucao, agora eu decodifico ela para poder executa-la
    decode_instruction(instruction);
    //Instrucao foi pega da memoria, foi decodificada e agora temos que executa-la
    execute_instruction(instruction, processor, memory);
    //Pra garantir que o registrador x0 seja hardwired para o valor 0, eu fico 
    //resetando ele.
    processor->R[0] = 0;
    //Por ultimo a gente tem que printar o estado dos registradores apos cada instrucao
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 4; j++){
            printf("r%2d = %08x ", i*4 + j, processor->R[i*4 + j]);
        }
        puts("");
    }
    printf("\n");
}

//Funcao para Carregar o Programa na Memoria...
//PRIMEIRO: A gente carrega o programa na memoria para so em seguida comecar a
//percorrer ele e executar as instrucoes.
void load_program(uint8_t *mem, size_t memsize, int startaddr, const char *filename) {
  //Arquivo que contem todas as instrucoes do programa escritas em hexadecimal
  //para que elas sejam executadas
  FILE *file = fopen(filename, "r");
  const int MAX_SIZE = 50;
  char line[MAX_SIZE];
  int instruction, offset = 0;
  while (fgets(line, MAX_SIZE, file) != NULL) { //Vou lendo as linhas/instrucoes do arquivo enquanto ele n acabar
    //strtol eh uma funcao que vai ler uma linha do arquivo, que ta como uma string em hex
    //vai transformar essa linha em um long int por padrao. E em seguida damos
    //um cast para int32_t porque o tamanho de todas as instrucoes do RV32I eh de
    //32 bits.
    instruction = (int32_t) strtol(line, NULL, 16);
    //Lembre-se que a memoria ta enderecada por byte. Cada indice = 1 byte(8 bits)
    //Como uma instrucao tem 32 bits (4 bytes), temos que quebrar ela em quatro 
    //partes e armazena-las na memoria byte a byte, lembrando que RISC-V eh Little
    //Endian: Bytes menos significativos nos enderecos menores.
    //Se eu pego uma instrucao  e dou um and dela com 0xff(11111111) eu to preservando
    //o seu estado de boas.
    //Jogo ela, vou incrementando no offset pra preencher os 4 bytes referentes a cada
    //instrucao e eh isso ai
    mem[startaddr + offset] = instruction & 0xFF;
    mem[startaddr + offset + 1] = (instruction >> 8) & 0xFF;
    mem[startaddr + offset + 2] = (instruction >> 16) & 0xFF;
    mem[startaddr + offset + 3] = (instruction >> 24) & 0xFF;
    uint8_t *inst_ptr = (uint8_t *) &instruction;
    printf("%08x: ", startaddr + offset);
    decode_instruction(*((Instruction *) inst_ptr));
    offset += 4; //Garante que eu consiga pular de 4 em 4 bytes apos cada instrucao
    //escrita na memoria...
  }
}

int main(){
    //Variavel representando o nosso processador...
    Processor processor;

    //Hora de carregar o programa na memoria
    assert(memory == NULL);
    memory = calloc(MEMORY_SPACE, sizeof(uint8_t)); //Criando o array que vai
    //representar nossa memoria.
    assert(memory != NULL);

    //setando o PC para 0x1000
    processor.PC = 0x1000;
    load_program(memory, MEMORY_SPACE, processor.PC, nome do arquivo de leitura);

    //Inicializando a CPU
    for(int i = 0; i < 32; i++){
        processor.R[i] = 0;
    }
    processor.R[2] = 0xEFFFF;
    processor.R[3] = 0x3000;
    //Nao vai ter na versao final esses dois ultimos argumentos nessa funcao abaixo
    while(1) execute(&processor, opt_interactive, opt_regdump);

    return 0;
}