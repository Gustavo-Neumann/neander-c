#ifndef NEANDER_H
#define NEANDER_H

#include <stdbool.h>
#include <stdint.h>

#define MEM_SIZE 516

//instruções
#define STA 0x10
#define LDA 0x20
#define ADD 0x30
#define NOT 0x60
#define JMP 0x80
#define JN  0x90
#define JZ  0xA0
#define HLT 0xF0

typedef struct {
    uint8_t ac;
    uint8_t pc;
    bool z;
    bool n;
    uint8_t memory[MEM_SIZE];
} Neander;

void init_neander(Neander *cpu, const char *filename); //inicialização
void execute_neander(Neander *cpu); //execução
void print_memory(const Neander *cpu); //função auxiliar

#endif // NEANDER_H
