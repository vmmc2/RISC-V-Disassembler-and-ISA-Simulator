//Diretivas de pre-compilacao usadas para incluir os tipos de dados que serao
//usados no nosso simulador
#ifndef TIPOS_H
#define TIPOS_H

#include <stdint.h> //contem os inteiros sem sinal.

//Definicao de dados
typedef uint8_t ubyte; //inteiro de 8 bits sem sinal
typedef uint16_t uhword; //inteiro de 16 bits sem sinal (halfword)
typedef uint32_t uword; //inteiro de 32 bits sem sinal (word)
typedef uint64_t udword; //inteiro de 64 bits sem sinal (doubleword)

typedef int8_t byte; //inteiro de 8 bits (byte)
typedef int16_t hword; //inteiro de 16 bits  (halfword)
typedef int32_t word; //inteiro de 32 bits  (word)
typedef int64_t dword; //inteiro de 64 bits (doubleword)

//Lembrando que um endereco de memoria do modulo RV32I tem 32 bits
typedef uint32_t address;

//Tamanho do registrador no RV32I tbm eh de 32 bits
typedef uint32_t Register;

//Nosso processador consiste do que? Um vetor de numeros de 32 bits que representam
//nossos registradores e um inteiro de 32 bits que representa o PC
struct Processor{
    //DUVIDA: Pq nao lidar com o numero armazenado nos registradores como um 
    //numero com sinal?????
    Register R[32]; //Registradores: x0 - x31
    Register PC; //Registrador responsavel por nos indicar qual instrucao devemos
    //executar
};

//Enum para lidar com o alinhamento de memoria e o tamanhos dos dados
//O TAMANHO TA TODO EM BYTES
typedef enum{
    LENGTH_BYTE = 1,
    LENGTH_HALF_WORD = 2,
    LENGTH_WORD = 4,
} Alignment;

//Lembrando que a memoria da CPU ta enderecada por byte.
// 1 indice = 1 byte
//Esse define da pra gente mais ou menos 1MByte de memoria. (1e6 bytes aprox)
#define MEMORY_SPACE (1024*1024)

//Trabalhando com Union... Oq seria uma Union?
/*Basicamente eh um tipo especial de dado que nos permite armazenar diferentes
  tipos de dados em uma mesma localizacao da memoria
  Da pra definir uma union com muitos membros, entretanto apenas um membro por 
  vez pode conter um valor de fato.
  Unions provide an efficient way of using the same memory location 
  for multiple-purpose.
  Perceba o exemplo abaixo...

  union Data{
      int i;
      float f;
      char str[20];
  } data;
  Feita essa declaracao desse novo tipo de dado, temos que uma variavel do tipo
  Data pode armazenar um inteiro, um float ou uma string de caracteres. Isso quer
  dizer que uma mesma variavel, uma mesma posicao de memoria pode ser usada para
  armazenar multiplos tipos de dados diferentes.
  O espaco de memoria ocupado pela union vai ser tao grande quanto o maior membro
  presente dentro dessa union.
  IMPORTANTE: A Union so pode armazenar um desses membros por vez. Quando um ta 
  sendo armazenado, os outros ficam com lixo de memoria.
*/
typedef union{
    //acesse o opcode fazendo: Instruction.opcode
    struct{
        unsigned int opcode : 7;
        unsigned int : 25;
    };

    //acessando instrucao do tipo R: instruction.rtype(opcode|rd|funct3|rs1|rs2|funct7)
    struct{
        unsigned int opcode : 7;
        unsigned int rd : 5;
        unsigned int funct3 : 3;
        unsigned int rs1 : 5;
        unsigned int rs2 : 5;
        unsigned int funct7 : 7;
    } rtype;

    //acessando instrucao do tipo I: instruction.itype.(opcode|rs|rt|imm)
    struct{
        unsigned int opcode: 7;
        unsigned int rd : 5;
        unsigned int funct3 : 3;
        unsigned int rs1: 5;
        unsigned int imm : 12;
    } itype;

    //acessando instrucao do tipo U: instruction.utype.(opcode|rd|imm)
    struct{
        unsigned int opcode : 7;
        unsigned int rd : 5;
        unsigned int imm : 20;
    } utype;

    //acessando instrucao do tipo UJ: instruction.ujtype.(opcode|rd|imm)
    struct{
        unsigned int opcode : 7;
	    unsigned int rd : 5;
	    unsigned int imm : 20;
    } ujtype;

    //acessando instrucao do tipo S: instruction.stype.(opcode|imm5|funct3|rs1|rs2|imm7)
    struct{
        unsigned int opcode : 7;
        unsigned int imm5: 5;
        unsigned int funct3: 3;
        unsigned int rs1 : 5;
        unsigned int rs2 : 5;
        unsigned int imm7 : 7;
    } stype;

    //acessando instrucao do tipo SB: instruction.sbtype.(opcode|imm5|funct3|rs1|rs2|imm7)
    struct{
        unsigned int opcode : 7;
        unsigned int imm5: 5;
        unsigned int funct3: 3;
        unsigned int rs1 : 5;
        unsigned int rs2 : 5;
        unsigned int imm7 : 7;
    } sbtype;

    /*
    Os tipos a seguir nao tavam especificado no material que eu me baseei
    logo eu nao faco ideia pra que eles servem [por enquanto...]
    */
    int16_t chunks16[2];
    uint32_t bits;
} Instruction;

#endif
