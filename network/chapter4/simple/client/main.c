#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

extern int errno;

int main() {
  struct addrinfo hints;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET6;
  hints.ai_socktype = SOCK_DGRAM;

  struct addrinfo *res;
  if (getaddrinfo("::1", "8080", &hints, &res) != 0) {
    perror("getaddrinfo");
    return 1;
  }

  int sockfd;
  if ((sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) < 0) {
    perror("socket");
    return 1;
  }

  int only_ipv6 = 1;
  if (setsockopt(sockfd, IPPROTO_IPV6, IPV6_V6ONLY, &only_ipv6, sizeof(only_ipv6)) != 0) {
    perror("setsockopt");
    return 1;
  }

  char buf[BUFSIZ];
  if (!fgets(buf, BUFSIZ, stdin)) return 1;
  int buflen = strlen(buf);

  sendto(sockfd, buf, buflen, 0, res->ai_addr, res->ai_addrlen);

  memset(buf, 0, buflen);
  int bytes_recv = recvfrom(sockfd, buf, BUFSIZ, 0, res->ai_addr, &res->ai_addrlen);

  printf("%.*s", bytes_recv, buf);

  close(sockfd);
  freeaddrinfo(res);

  return 0;
}
