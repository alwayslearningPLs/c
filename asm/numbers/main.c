#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BINARY_OPTION "-b"
#define HEX_OPTION "-h"

#define BINARY_SPACE 9
#define HEX_SPACE 0

int is_number(char * input) {
  char *tmp = input;
  while (*tmp != '\0' && *tmp >= 0x30 && *tmp <= 0x39) tmp++;
  return *tmp == '\0';
}

void strrev(char *src) {
  size_t lsrc = strlen(src);
  for (size_t i = 0; i < lsrc/2; i++) {
    char tmp = src[i];
    src[i] = src[lsrc-i-1];
    src[lsrc-i-1] = tmp;
  }
}

char hex_rpr(int input) {
  if (input >= 16 || input < 0) return 0x30;
  return input >= 10 ? input - 10 + 0x61 : input + 0x30;
}

void hex(long input, char * dst) {
  if (input <= 0) return;
  size_t count = 0;
  while (input > 15) {
    dst[count++] = hex_rpr(input % 16);
    input /= 16;
  }
  dst[count++] = hex_rpr(input % 16);

  int max = 2 + ((count - 1) / 2) * 2;
  printf("%ld %d\n", count, max);
  for (; count < max; count++) {
    dst[count] = 0x30;
  }
  dst[count++] = '\0';

  strrev(dst);
}

void hdec(char * src, char * dst) {
}

void binary(long input, char * dst) {
  if (input <= 0) return;

  size_t count = 0;
  while (input > 1) {
    dst[count++] = input % 2 == 0 ? 0x30 : 0x31;
    input /= 2;
  }
  dst[count++] = input % 2 == 0 ? 0x30 : 0x31;

  int max = 8 + ((count - 1) / 8) * 8; // 0 -> 8 bits
  for (; count < max; count++) {
    dst[count] = 0x30;
  }

  strrev(dst);
}

void pretty(const char *dst, const size_t space) {
  size_t count = 0;
  while (*dst != '\0') {
    fputc(*dst, stdout);
    count++; dst++;
    if (count % space == 0) fputc(' ', stdout);
  }
  fputc('\n', stdout);
}

int main(int argc, char **argv) {
  if (argc != 3) {
    fprintf(stderr, "You must pass arguments: <number> [-h|-b]\n");
    return 1;
  }

  if (!is_number(argv[1])) {
    fprintf(stderr, "You must pass arguments: <number> [-h|-b]\n");
    return 1;
  }

  if (strcmp(argv[2], BINARY_OPTION) != 0 && strcmp(argv[2], HEX_OPTION) != 0) {
    fprintf(stderr, "You must pass arguments: <number> [-h|-b]\n");
    return 1;
  }

  char buf[50];
  if (strcmp(argv[2], BINARY_OPTION) == 0) {
    binary(atoi(argv[1]), buf);
    pretty(buf, BINARY_SPACE);
  } else if (strcmp(argv[2], HEX_OPTION) == 0) {
    hex(atoi(argv[1]), buf);
    printf("0x%s\n", buf);
  }

  return 0;
}
