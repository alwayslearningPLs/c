#include <netdb.h>
#include <ifaddrs.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef AI_DEFAULT
# define AI_DEFAULT (AI_V4MAPPED | AI_ADDRCONFIG)
#endif

#ifndef DEPRECATED_AI_SDN
# define DEPRECATED_AI_SDN 0x300
#endif

// Only used if GNU is set. __USE_GNU
#ifndef AI_IDN
# define AI_IDN 0x0040
#endif

// Only used if GNU is set. __USE_GNU
#ifndef AI_CANONIDN
# define AI_CANONIDN 0x0080
#endif

// node     = ['*', '\0']
// service  = ['*', '\0']
// When node or/and service is set to wildcard, then this values will be set to NULL.
// If both values are set to NULL, EAI_NONAME is returned.
void node_and_service_wildcard() {
  struct addrinfo hints;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET6;
  hints.ai_flags = AI_PASSIVE;

  struct addrinfo *res;
  int result;
  if ((result = getaddrinfo("*", "*", &hints, &res)) < 0) {
    printf("result is %d with the name %s\n", result, result == EAI_NONAME ? "EAI_NONAME" : "unknown");
    return;
  }
}

// When we don't specify the hints parameter, a default_hints is set to this parameter
void default_hints_source_code() {
  // allowed from C99
  const struct addrinfo default_hints = {
    .ai_flags = AI_DEFAULT,
    .ai_family = PF_UNSPEC,
    .ai_socktype = 0,
    .ai_protocol = 0,
    .ai_addrlen = 0,
    .ai_addr = 0,
    .ai_canonname = 0,
    .ai_next = 0,
  };
}

// When passing bad flags, return EAI_BADFLAGS
//  1. if passed a lot of flags that I don't fully understand
//  2. if passed flag AI_CANONNAME and no name or *
void bad_flags() {
  int ai_flags = (AI_PASSIVE|AI_CANONNAME|AI_NUMERICHOST|AI_ADDRCONFIG|AI_V4MAPPED|AI_IDN|AI_CANONIDN|DEPRECATED_AI_SDN|AI_NUMERICSERV|AI_ALL);

  struct addrinfo hints;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_flags = ~ai_flags;

  printf("ai_flags: %x\tbitwise ai_flags: %x\n", hints.ai_flags, (int) ~hints.ai_flags);

  struct addrinfo *res;
  int result;
  if ((result = getaddrinfo("not used", "not used", &hints, &res)) < 0) {
    printf("result is %d with the name %s\n", result, result == EAI_BADFLAGS ? "EAI_BADFLAGS" : "unkonwn");
  }

  hints.ai_flags = AI_CANONNAME;
  if ((result = getaddrinfo("*", "not used", &hints, &res)) < 0) {
    printf("result is %d with the name %s\n", result, result == EAI_BADFLAGS ? "EAI_BADFLAGS" : "unknown");
  }
}

// When passing a family value that is not AF_UNSPEC, AF_INET, AF_INET6
void bad_family() {
  struct addrinfo hints;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNIX;

  struct addrinfo *res;
  int result;
  if ((result = getaddrinfo("*", "not used", &hints, &res)) < 0) {
    printf("result is %d with the name %s\n", result, result == EAI_FAMILY ? "EAI_FAMILY" : "unknown");
  }
}

void understanding_binary_operations() {
  const unsigned char a = 0x01;             // 00000001
  const unsigned char bitwise_not_a = 0xfe; // 11111110

  if ((unsigned char) ~a == bitwise_not_a) {
    printf("~%x == %x\n", a, bitwise_not_a);
  }
}

int main() {
  node_and_service_wildcard(); 
  understanding_binary_operations();
  bad_flags();
  bad_family();
  return 0;
}
