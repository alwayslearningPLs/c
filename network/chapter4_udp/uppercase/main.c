#include <bits/types/struct_timeval.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

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
    perror("socket");
    return 1;
  }

  if (bind(sockfd, res->ai_addr, res->ai_addrlen) < 0) {
    perror("bind");
    return 1;
  }
  freeaddrinfo(res);

  struct timeval t;
  t.tv_sec = 0;
  t.tv_usec = 100000; // 100milliseconds

  fd_set sockets;
  FD_ZERO(&sockets);
  FD_SET(sockfd, &sockets);

  int max_socket = sockfd;
  for (;;) {
    char buf[BUFSIZ];
    fd_set reads;
    reads = sockets;

    if (select(max_socket + 1, &reads, 0, 0, &t) < 0) {
      perror("select");
      break;
    }

    if (FD_ISSET(sockfd, &reads)) {
      printf("hello");
      struct sockaddr_storage sa;
      socklen_t lsa = sizeof(sa);
      int bytes_recv;
      if ((bytes_recv = recvfrom(sockfd, buf, BUFSIZ, 0, (struct sockaddr*) &sa, &lsa)) < 1) {
        perror("recvfrom");
        continue;
      }

      char c[2][50];
      if (getnameinfo((const struct sockaddr*) &sa, lsa, c[IP_ADDRESS], 50, c[SERVICE], 50, NI_NUMERICHOST | NI_NUMERICSERV) != 0) {
        perror("getnameinfo");
        continue;
      }
      printf("connection from %s:%s\n", c[IP_ADDRESS], c[SERVICE]);

      for (int i = 0; i < bytes_recv; i++) buf[i] = toupper(buf[i]);
      
      sendto(sockfd, buf, bytes_recv, 0, (const struct sockaddr*) &sa, lsa);

      memset(buf, 0, bytes_recv);
    }
  }

  close(sockfd);

  return 0;
}
