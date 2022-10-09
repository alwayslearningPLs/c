/**
 * How to use?
 *
 * ./main 10 -d -b
 *
 * 0b00001010
 *
 * input: binary | decimal | hexadecimal
 * output: binary | decimal | hexadecimal
 *
 * input: binary -> bdec() | bdec() && hex()
 * input: decimal -> binary() | hex() input: hexadecimal -> bdec() && binary() | hdec()
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#define BINARY_OPTION "-b"
#define DEC_OPTION "-d"
#define HEX_OPTION "-h"

#define BINARY_SPACE 9
#define HEX_SPACE 0

#define __bswap_constant_16(x)					\
  ((__uint16_t) ((((x) >> 8) & 0xff) | (((x) & 0xff) << 8)))

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

char rpr_hex(char input) {
  return input >= 0x61 ? (input - 0x61) + 10 : input - 0x30;
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
  for (; count < max; count++) {
    dst[count] = 0x30;
  }
  dst[count++] = '\0';

  strrev(dst);
}

long hdec(char * src) {
  int r = 0;
  while (*src != '\0') {
    r = (r + rpr_hex(*src)) * 16;
    src++;
  }
  return r/16;
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

long bdec(char * src) {
  int r = 0;
  while (*src != '\0') {
    r = (r + *src - 0x30) * 2;
    src++;
  }
  return r/2;
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

int is_option_ko(const char *input) {
  return strcmp(input, BINARY_OPTION) != 0 && strcmp(input, HEX_OPTION) != 0 && strcmp(input, DEC_OPTION) != 0;
}

int main(int argc, char **argv) {
  if (argc != 4) {
    fprintf(stderr, "You must pass arguments: <number> [-h|-b|-d] [-h|-b|-d|-r]\n");
    return 1;
  }

  if (is_option_ko(argv[2]) || is_option_ko(argv[3])) {
    fprintf(stderr, "You must pass arguments: <number> [-h|-b|-d] [-h|-b|-d]\n");
    return 1;
  }

  if (strcmp(argv[2], BINARY_OPTION) == 0) {
    long dec = bdec(argv[1]);
    if (strcmp(argv[3], HEX_OPTION) == 0) {
      char buf[50];
      hex(dec, buf);
      printf("0x%s\n", buf);
    } else printf("%ld\n", dec);
  } else if (strcmp(argv[2], DEC_OPTION) == 0) {
    char buf[50];
    long dec = atoi(argv[1]);
    if (strcmp(argv[3], HEX_OPTION) == 0) hex(dec, buf); else binary(dec, buf);
    printf("%s", strcmp(argv[3], HEX_OPTION) == 0 ? "0x" : "0b");
    pretty(buf, BINARY_SPACE);
  } else {
    long dec = hdec(argv[1]);
    if (strcmp(argv[3], BINARY_OPTION) == 0) {
      char buf[50];
      binary(dec, buf);pretty(buf, BINARY_SPACE);
      pretty(buf, BINARY_SPACE);
    } else printf("%ld\n", dec);
  }

  return 0;
}
