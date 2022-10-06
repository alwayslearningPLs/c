#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define IP_ADDRESS 0
#define SERVICE 1

extern int errno;

int main() {
  struct addrinfo hints;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET6;
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_flags = AI_PASSIVE;

  struct addrinfo *res;
  if (getaddrinfo(0, "8080", &hints, &res) != 0) {
    perror("getaddrinfo");
    return 1;
  }

  int sockfd;
  if ((sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) < 0) {
    perror("sockfd");
    return 1;
  }

  int only_ipv6 = 1;
  if (setsockopt(sockfd, IPPROTO_IPV6, IPV6_V6ONLY, &only_ipv6, sizeof(only_ipv6)) != 0) {
    perror("setsockopt");
    return 1;
  }

  if (bind(sockfd, res->ai_addr, res->ai_addrlen) < 0) {
    perror("bind");
    return 1;
  }
  freeaddrinfo(res);

  struct sockaddr_storage sa;
  socklen_t lsa = sizeof(lsa);
  char buf[BUFSIZ];
  int bytes_recv;
  if ((bytes_recv = recvfrom(sockfd, buf, BUFSIZ, 0, (struct sockaddr*) &sa, &lsa)) < 1) {
    perror("recvfrom");
    return 1;
  }

  char c[2][50];
  if (getnameinfo((const struct sockaddr*) &sa, lsa, c[IP_ADDRESS], 50, c[SERVICE], 50, NI_NUMERICHOST | NI_NUMERICSERV) != 0) {
    perror("getnameinfo");
    return 1;
  }
  printf("[%s]:%s\n", c[0], c[1]);

  sendto(sockfd, buf, bytes_recv, 0, (const struct sockaddr*) &sa, lsa);

  close(sockfd);

  return 0;
}
