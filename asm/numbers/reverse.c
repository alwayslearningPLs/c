#include <sys/types.h>
#include <netinet/in.h> // Here it includes <endian.h>, <byteswap.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BINARY_SPACE 9

#ifndef __BYTE_ORDER
#define __BYTE_ORDER __LITTLE_ENDIAN
#endif

// __BIG_ENDIAN   : 1 (dec) => 0b00000000 00000000 00000000 00000001
// __LITTLE_ENDIAN: 1 (dec) => 0b00000001 00000000 00000000 00000000
#define is_big_endian() (*((char *) &(int){1}) == 0)
#define is_little_endian() (*((char *) &(int){1}) == 1)

// host to network short
// the most significant byte is at the highest address because my machine is a __LITTLE_ENDIAN
#define personal_htons(x) \
  ((u_int16_t) ((((x) >> 8) & 0xff) | (((x) & 0xff) << 8)))

// 10100000 00110000 10101010 00000001 -- __LITTLE_ENDIAN
// 00000001 10101010 00110000 10100000 -- __BIG_ENDIAN
#define personal_htonl(x) \
  ((u_int32_t) ((((x) & 0x000000ffu) << 24) | \
                (((x) & 0x0000ff00u) << 8 ) | \
                (((x) & 0x00ff0000u) >> 8 ) | \
                (((x) & 0xff000000u) >> 24)   \
  ))

// 8 bytes
#define personal_swap64(x) \
  (((x) & 0x00000000000000ffull) << 56) | \
  (((x) & 0x000000000000ff00ull) << 40) | \
  (((x) & 0x0000000000ff0000ull) << 24) | \
  (((x) & 0x00000000ff000000ull) << 8 ) | \
  (((x) & 0x000000ff00000000ull) >> 8 ) | \
  (((x) & 0x0000ff0000000000ull) >> 24) | \
  (((x) & 0x00ff000000000000ull) >> 40) | \
  (((x) & 0xff00000000000000ull) >> 56)

void strrev(char *src) {
  size_t lsrc = strlen(src);
  for (size_t i = 0; i < lsrc/2; i++) {
    char tmp = src[i];
    src[i] = src[lsrc-i-1];
    src[lsrc-i-1] = tmp;
  } }
void binary(u_int32_t input, char * dst) {
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
  fflush(stdout);
}

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "error parsing arguments\n");
    return 1;
  }

  if (is_little_endian()) printf("My computer is little endian\n");
  else printf("My computer is big endian\n");

  u_int32_t v = atoi(argv[1]);

  char from[60], to[60];
  binary(v, from);

  u_int32_t vv = personal_htonl(v);
  binary(vv, to);

  printf("%u %u\n", vv, htonl(v));

  pretty(from, BINARY_SPACE);
  printf(" -> ");
  pretty(to, BINARY_SPACE);
  printf("\n");

  return 0;
}
