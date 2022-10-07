#include <netdb.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define ZERO 0x30
#define NINE 0x39
#define DOT 0x2e
#define DOUBLE_DOT 0x3a

// Here we are just checking that IPv4 and IPv6 contains allowed characters, we are not checking if it is a correct IPv4/IPv6
int isipaddr(char *input) {
  char *tmp = input;
  while (*tmp != '\0' && ((*tmp >= ZERO && *tmp <= NINE) || *tmp == DOT || *tmp == DOUBLE_DOT)) tmp++;
  return *tmp == '\0';
}

int is_number(char *input) {
  char *tmp = input;
  while (*tmp != '\0' && *tmp >= ZERO && *tmp <= NINE) tmp++;
  return *tmp == '\0';
}

void socktype_name(int socktype, char *dst) {
  switch (socktype) {
    case SOCK_STREAM:
      strcpy(dst, "SOCK_STREAM");
      break;
    case SOCK_DGRAM:
      strcpy(dst, "SOCK_DGRAM");
      break;
    case SOCK_RAW:
      strcpy(dst, "SOCK_RAW");
      break;
    default:
      strcpy(dst, "other");
      break;
  }
}

int main(int argc, char **argv) {
  if (argc != 3) {
    fprintf(stderr, "error parsing arguments: (<IP-address>|DNS) <port>");
    return 1;
  }

  if (!is_number(argv[2])) {
    fprintf(stderr, "error parsing arguments. second argument must be a number representing a port. (<IP-address>|DNS) <port>");
    return 1;
  }

  struct addrinfo hints;
  memset(&hints, 0, sizeof(hints));
  hints.ai_flags = AI_ALL;

  struct addrinfo *res;
  if (getaddrinfo(argv[1], argv[2], &hints, &res)) {
    perror("getaddrinfo");
    return 1;
  }

  do {
    char c[3][50];
    if (getnameinfo(res->ai_addr, res->ai_addrlen, c[0], 50, c[1], 50, NI_NUMERICHOST | NI_NUMERICSERV)) {
      perror("getnameinfo");
      res = res->ai_next;
      continue;
    }

    socktype_name(res->ai_socktype, c[2]);
    printf("%s %s %s:%s\n", res->ai_family == AF_INET6 ? "IPv6": "IPv4", c[2], c[0], c[1]);
  } while ((res = res->ai_next));

  return 0;
}
