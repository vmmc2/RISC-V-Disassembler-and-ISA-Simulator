#ifndef UTILS_H
#define UTILS_H

#include "tipos.h"
#include <bits/stdc++.h>

#define RTYPE_FORMAT "%s\tx%d, x%d, x%d\n"
#define ITYPE_FORMAT "%s\tx%d, x%d, %d\n"
#define MEM_FORMAT "%s\tx%d, %d(x%d)\n"
#define AUIPC_FORMAT "auipc\tx%d, %d\n"
#define LUI_FORMAT "lui\tx%d, %d\n"
#define JAL_FORMAT "jal\tx%d, %d\n"
#define BRANCH_FORMAT "%s\tx%d, x%d, %d\n"
#define ECALL_FORMAT "ecall\n"

int bitSigner(unsigned, unsigned);
int get_branch_offset(Instruction);
int get_jump_offset(Instruction);
int get_store_offset(Instruction);
void handle_invalid_instruction(Instruction);
void handle_invalid_read(address);
void handle_invalid_write(address);


#endif
