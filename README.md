# RISC-V-Disassembler-and-ISA-Simulator
![[RISC-V](https://http://riscv.org/)](outros/image8.png)

# Introduction
This project is a disassembler and ISA simulator for the RISC-V architecture, but just the RV32I extension. It receives a file called "entrada.txt" that contains the instructions in hexadecimal format. The program then will execute these instructions, generating the corresponding code in RISC-V assembly and will display it in the terminal. It will also display the status of
every single register (x0 - x31, including PC).

# Compiling
First of all, make sure you are in the "RISC-V DISASSEMBLER + SIMULATOR" folder.
Then, you put the hexadecimal instructions that you want to execute/simulate in the "entrada.txt" file.
After that, you just have to write in the terminal the following commands:

* g++ riscv.cpp -o executable_name
* ./executable_name

# Status
The following instructions from RV32I extension are supported:
#### R-Type
- [x] add
- [x] sub
- [x] sll
- [x] xor
- [x] srl
- [x] slt
- [x] sra
- [x] or
- [x] and
- [x] sltu

#### I-Type
- [x] lb
- [x] lh
- [x] lw
- [x] lbu
- [x] lhu
- [ ] lwu
- [x] addi
- [x] slli
- [x] srli
- [x] slti
- [x] sltiu
- [x] srai
- [x] xori
- [x] ori
- [x] andi
- [ ] jalr
- [x] ecall

#### S-Type
- [x] sb
- [x] sh
- [x] sw

#### SB-Type
- [x] bne
- [x] blt
- [x] beq
- [x] bge
- [x] bltu
- [x] bgeu

#### U-Type
- [x] lui
- [x] auipc

#### UJ-Type
- [x] jal

