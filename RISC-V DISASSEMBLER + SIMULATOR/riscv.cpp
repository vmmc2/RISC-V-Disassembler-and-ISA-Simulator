#include "riscv.h"
#include "tipos.h"
#include "parte2.cpp"
#include "parte1.cpp"
#include "utils.h"
#include "utils.cpp"
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
    instruction.bits = load(memory, processor->PC, LENGTH_WORD, 1); //TUDO CERTO ATE AQUI!

    //printf("instrucao pega: %08x\n", instruction.bits); //PRINTA A INSTRUCAO PEGA

    //Terminou de buscar a instrucao... Printa o valor presente no registrador PC
    //Pra saber em qual posicao da memoria de instrucoes eu me encontro
    printf("%08x: ", processor->PC); //PRINTA O ENDERECO ATUAL DE PC


    //Peguei a instrucao, agora eu decodifico ela para poder executa-la
    decode_instruction(instruction);
    //Instrucao foi pega da memoria, foi decodificada e agora temos que executa-la
    execute_instruction(instruction, processor, memory);
    //Pra garantir que o registrador x0 seja hardwired para o valor 0, eu fico 
    //resetando ele.
    processor->R[0] = 0;
    return;
}

void print_state(Processor *processor){
  //Por ultimo a gente tem que printar o estado dos registradores apos cada instrucao
  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 4; j++){
      printf("r%2d = %08x ", i*4 + j, processor->R[i*4 + j]);
    }
    puts("");
  }
  printf("\n");
  return;
}

//Funcao para Carregar o Programa na Memoria...
//PRIMEIRO: A gente carrega o programa na memoria para so em seguida comecar a
//percorrer ele e executar as instrucoes.
int load_program(uint8_t *mem, size_t memsize, int startaddr, const char *filename) {
  //Arquivo que contem todas as instrucoes do programa escritas em hexadecimal
  //para que elas sejam executadas
  //Abrindo o arquivo que contem as instrucoes a serem executadas (no formato de leitura r)
  FILE *file = fopen(filename, "r");
  FILE *filesaida = fopen("saida.txt", "w");
  const int MAX_SIZE = 50;
  char line[MAX_SIZE];
  int instruction, offset = 0;
  int iterations = 0; //variavel responsavel por indicar quantas instrucoes eu vou executar.

  //PRIMEIRO EU VOU TESTAR PRA VER SE TA DE BOA.. LER DE UM ARQUIVO E JOGAR O CONTEUDO
  //PARA OUTRO... (PASSOU NOS TESTES! TUDO OK ATE AQUI)
  /*while(fgets(line, MAX_SIZE, file) != NULL){
      ++iterations;
      fprintf(filesaida, "%s", line);
  }
  */
  
  while (fgets(line, MAX_SIZE, file) != NULL) { //Vou lendo as linhas/instrucoes do arquivo enquanto ele n acabar
    //strtol eh uma funcao que vai ler uma linha do arquivo, que ta como uma string em hex
    //vai transformar essa linha em um long int por padrao. E em seguida damos
    //um cast para int32_t porque o tamanho de todas as instrucoes do RV32I eh de
    //32 bits.
    ++iterations;
    instruction = (int32_t) strtol(line, NULL, 16);
    Instruction inst;
    inst.bits = instruction;
    decode_instruction(inst);
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

    
    //uint8_t *inst_ptr = (uint8_t *) &instruction;
    //printf("%08x: ", startaddr + offset);                        //TEM QUE VER OQ EU VOU FAZER COM ISSO AQUI...
    //decode_instruction(*((Instruction *) inst_ptr));
    
    offset += 4; //Garante que eu consiga pular de 4 em 4 bytes apos cada instrucao
    //escrita na memoria...
  }
  printf("\n\n\n");
  fclose(file);
  fclose(filesaida);
  return iterations;
}

void print_memory(int qtd_iteracoes, int adss){
  for(int i = 0; i < qtd_iteracoes * 4; i++){
    printf("memory[%d] = %d\n", adss, memory[adss]);
    adss++;
  }
  return;
}

int main(){
    //LEMBRAR DE FECHAR O ARQUIVO DPS QUE TERMINAR DE LER TODAS AS INSTRUCOES..
    //COMO INDICAR QUE TERMINAMOS A SIMULACAO?? QUANDO CHEGARMOS EM UMA LINHA VAZIA
    //PRA FAZER ISSO DA PRA USAR FGETS...
    //Variavel representando o nosso processador...
    Processor processor;
    const char *arquivo_entrada = "entrada.txt";
    int qtd_iteracoes = 0;
    //Hora de carregar o programa na memoria
    assert(memory == NULL);
    memory = (ubyte*)calloc(MEMORY_SPACE, sizeof(uint8_t)); //Criando o array que vai representar nossa memoria.
    assert(memory != NULL);

    //setando o PC para 0x1000
    processor.PC = 0x1000;
    qtd_iteracoes = load_program(memory, MEMORY_SPACE, processor.PC, arquivo_entrada);


    /*TESTE PARA VER SE A MEMORIA TA PEGANDO DIREITINHO AS INSTRUCOES CONVERTIDAS PARA NUMEROS INTEIROS SEM SINAL DE 32 BITS*/
    //print_memory(qtd_iteracoes, processor.PC);
    

    //Inicializando a CPU
    for(int i = 0; i < 32; i++){
        processor.R[i] = 0;
    }
    processor.R[2] = 0xEFFFF; //stack pointer - sp (cresce em direcao aos enderecos menores)
    processor.R[3] = 0x3000;  //global pointer - gp (nao sei direito oq isso faz, preciso investigar melhor...)

    printf("%d\n", qtd_iteracoes);
    //Nao vai ter na versao final esses dois ultimos argumentos nessa funcao abaixo
    for(int i = 0; i < qtd_iteracoes; i++){ 
      execute(&processor, 1);
      print_state(&processor);
    }
    return 0;
}
