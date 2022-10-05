#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define CHAR_BITS 8
#define SHRT_BITS 16
#define INT_BITS  32
#define LONG_BITS 64

extern int errno;

long max(long a, long b) {
  return a > b ? a : b;
}

int is_number(char *input) {
  char *tmp = input;

  while (*tmp != '\0' && *tmp >= 0x30 && *tmp <= 0x39) tmp++;

  return *tmp == '\0';
}

void strrev(char *buf) {
  size_t blen = strlen(buf);

  for (size_t i = 0; i < blen/2; i++) {
    char tmp = buf[i];
    buf[i] = buf[blen-i-1];
    buf[blen-i-1] = tmp;
  }
}

void binary(long input, char *buf, size_t len) {
  int count = 0;
  while(input >= 1) {
    buf[count++] = input % 2 + 0x30;
    input /= 2;
  }
  buf[count++] = input + 0x30;

  for (int i = count; i < len; i++) {
    buf[i] = 0x30;
  }

  for (int i = 1; i < len-1; i++) {
    char tmp = buf[i];
    if (i % 8 == 0) {
      buf[i++] = ' ';
    }
    buf[i] = tmp;
  }
  buf[len] = '\0';

  strrev(buf);
}

void ipv4(const char* host, uint16_t port) {
  char binary_representation[4][INT_BITS+1];
  struct sockaddr_in sa;
  memset(&sa, 0, sizeof(sa));
  sa.sin_family = AF_INET; // or PF_INET
  sa.sin_port = htons(port);
  
  if (!inet_pton(AF_INET, host, &sa.sin_addr)) {
    perror("inet_pton");
    exit(1);
  }

  char ntop[INET_ADDRSTRLEN];
  if (inet_ntop(AF_INET, &sa.sin_addr, ntop, INET_ADDRSTRLEN) == NULL) {
    perror("inet_ntop");
    exit(1);
  }

  uint16_t tohs = ntohs(sa.sin_port);
  uint32_t s_addr_htos = ntohl(sa.sin_addr.s_addr);

  binary(sa.sin_port, binary_representation[0], SHRT_BITS);
  binary(tohs, binary_representation[1], SHRT_BITS);
  binary(sa.sin_addr.s_addr, binary_representation[2], INT_BITS);
  binary(s_addr_htos, binary_representation[3], INT_BITS);

  printf("port:\n\tnetwork representation: %d - %s\n\thost representation: %d - %s\n",
         sa.sin_port, binary_representation[0], tohs, binary_representation[1]);
  printf("host:\n\tnetwork representation: %d - %s\n\thost representation: %s - %s\n",
         sa.sin_addr.s_addr, binary_representation[2], ntop, binary_representation[3]);
}

int main(int argc, char **argv) {
  if (argc != 3) {
    printf("Arguments: <host> <port>\n");
    exit(1);
  }

  if (!is_number(argv[2])) {
    printf("second argument must be a port\n");
    exit(1);
  }

  ipv4(argv[1], atoi(argv[2]));
  return 0;
}
