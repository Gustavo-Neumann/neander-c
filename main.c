#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define STA 0x10
#define LDA 0x20
#define ADD 0x30
#define NOT 0x60
#define JMP 0x80
#define JN 0x90
#define JZ 0xA0
#define HLT 0xF0

void print_memory(uint8_t *bytes, int size, uint8_t ac, uint8_t pc, bool z, bool n) {
    size_t offset = 0;

    printf("\n\n");
    printf("AC: %03d  | PC: %03d\n", ac, pc);
    printf("Z : %s | N : %s\n", z ? "true": "false", n ? "true" : "false");

    while (offset < size) {
        printf("%08zx: ", offset);

        for (size_t i = 0; i < 16; i++) {
            if (offset + i < size)
                printf("%02x ", bytes[offset + i]);
            else
                printf("   ");
        }

        printf("\n");
        offset += 16;
    }
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Uso: %s <arquivo>\n", argv[0]);
        return 1;
    }

    uint8_t ac = 0, pc = 0;
    bool z = false, n = false;

    FILE *file = fopen(argv[1], "rb");
    if (!file) {
        printf("Erro ao abrir arquivo.\n");
        return 1;
    }

    uint8_t bytes[516];
    fread(bytes, 1, sizeof(bytes), file);
    fclose(file);

    print_memory(bytes, sizeof(bytes), ac, pc, z, n);

    while (bytes[pc] != HLT) {
        uint8_t operand = bytes[pc + 2];

        switch (bytes[pc]) {
            case STA:
                bytes[operand * 2 + 4] = ac;
                break;
            case LDA:
                ac = bytes[operand * 2 + 4];
                break;
            case ADD:
                ac += bytes[operand * 2 + 4];''
                break;
            case NOT:
                ac = ~ac;
                break;
            case JMP:
                pc = operand * 2 + 4;
                continue;
            case JN:
                if (n) {
                    pc = operand * 2 + 4;
                    continue;
                }
                break;
            case JZ:
                if (z) {
                    pc = operand * 2 + 4;
                    continue;
                }
                break;
            default:
                break;
        }

        z = (ac == 0);
        n = (ac & 0x80) != 0; 

        pc += 4;
    }

    print_memory(bytes, sizeof(bytes), ac, pc, z, n);

    return 0;
}