#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define STA 0x10
#define LDA 0x20
#define ADD 0x30
#define NOT 0x60
#define JMP 0x80
#define JN 0x90
#define JZ 0xA0
#define HLT 0xF0

void print_memory(uint8_t *bytes, int size, uint8_t ac, uint8_t pc, bool z,
                  bool n) {
  size_t offset = 0;

  printf("AC: %03d  | PC: %03d\n", ac, pc);
  printf("Z : %s | N : %s\n", z ? "true" : "false", n ? "true" : "false");

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

  offset += 16;

  printf("\n\n");
}

int main(int argc, char const *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
    return 1;
  }

  uint8_t ac = 0, pc = 0;
  bool z = 0, n = 0;

  const char *file_name = argv[1];
  FILE *file = fopen(file_name, "rb");
  if (!file) {
    perror("Error opening file");
    return 1;
  }

  uint8_t bytes[516];
  uint8_t file_id[4];
  fread(file_id, 1, 4, file);

  fread(bytes + 4, 1, 512, file);
  fclose(file);

  print_memory(bytes, 516, ac, pc, z, n);

  while (bytes[pc] != HLT) {

    if (ac == 0) {
      z = true;
    } else {
      z = false;
    }

    n = (ac & 0x80) != 0;

    switch (bytes[pc]) {
    case 0x00:
      break;
    case STA:
      bytes[bytes[pc + 2] * 2 + 4] = ac;
      break;
    case LDA:
      ac = bytes[bytes[pc + 2] * 2 + 4];
      break;
    case ADD:
      ac += bytes[bytes[pc + 2] * 2 + 4];
      break;
    case 0x40:
      ac = bytes[bytes[pc + 2] * 2 + 4] | ac;
      break;
    case 0x50:
      ac = bytes[bytes[pc + 2] * 2 + 4] & ac;
      break;
    case NOT:
      ac = ~ac;
      pc += 2;
      continue;
      break;
    case JMP:
      pc = bytes[pc + 2] * 2 + 4;
      continue;
      break;
    case JN:
      if (n) {
        pc = bytes[pc + 2] * 2 + 4;
        continue;
      }
      break;
    case JZ:
      if (z) {
        pc = bytes[pc + 2] * 2 + 4;
        continue;
      }
      break;
    }

    pc += 4;
    print_memory(bytes, 516, ac, pc, z, n);
  }

  print_memory(bytes, 516, ac, pc, z, n);

  return 0;
}