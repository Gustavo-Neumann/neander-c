#include "neander.h"
#include <stdio.h>
#include <stdlib.h>

//inicialização
void init_neander(Neander *cpu, const char *filename) {
    cpu->ac = 0;
    cpu->pc = 0;
    cpu->z = false;
    cpu->n = false;
    
    FILE *file = fopen(filename, "rb");
    if (!file) {
        printf("Erro ao abrir arquivo.\n");
        exit(1);
    }

    fread(cpu->memory, 1, MEM_SIZE, file);
    fclose(file);
}

void execute_neander(Neander *cpu) {
    while (cpu->memory[cpu->pc] != HLT) {
        uint8_t operand = cpu->memory[cpu->pc + 2];
        uint8_t addr = operand * 2 + 4;

        switch (cpu->memory[cpu->pc]) {
            case STA:
                cpu->memory[addr] = cpu->ac;
                break;
            case LDA:
                cpu->ac = cpu->memory[addr];
                break;
            case ADD:
                cpu->ac += cpu->memory[addr];
                break;
            case NOT:
                cpu->ac = ~cpu->ac;
                break;
            case JMP:
                cpu->pc = addr;
                continue;
            case JN:
                if (cpu->n) {
                    cpu->pc = addr;
                    continue;
                }
                break;
            case JZ:
                if (cpu->z) {
                    cpu->pc = addr;
                    continue;
                }
                break;
            default:
                break;
        }

        cpu->z = (cpu->ac == 0);
        cpu->n = (cpu->ac & 0x80) != 0;
        cpu->pc += 4;
    }
}

void print_memory(const Neander *cpu) {
    printf("\n\nAC: %03d  | PC: %03d\n", cpu->ac, cpu->pc);
    printf("Z : %s | N : %s\n", cpu->z ? "true" : "false", cpu->n ? "true" : "false");

    for (size_t offset = 0; offset < MEM_SIZE; offset += 16) {
        printf("%08zx: ", offset);
        for (size_t i = 0; i < 16; i++) {
            if (offset + i < MEM_SIZE)
                printf("%02x ", cpu->memory[offset + i]);
            else
                printf("   ");
        }
        printf("\n");
    }
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Uso: %s <arquivo>\n", argv[0]);
        return 1;
    }

    Neander cpu;
    init_neander(&cpu, argv[1]);
    print_memory(&cpu);
    execute_neander(&cpu);
    print_memory(&cpu);

    return 0;
}
