#include <bits/types/struct_timeval.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

extern int errno;

int is_number(char *input) {
  char *tmp = input;
  while (*tmp != '\0' && *tmp >= 0x30 && *tmp <= 0x39) tmp++;
  return *tmp == '\0';
}

int tcp_send(int sockfd, const char* buf, size_t buflen) {
  size_t bytes_sent = 0;
  
  do {
    int tmp;
    if ((tmp = send(sockfd, buf + bytes_sent, buflen - bytes_sent, 0)) < 1) {
      perror("send");
      break;
    }
    bytes_sent += tmp;
  } while(bytes_sent < buflen);

  return bytes_sent >= buflen;
}

int main(int argc, char ** argv) {
  if (argc < 3 || argc > 4) {
    fprintf(stderr, "error parsing arguments. <host> <port> [-udp]\n");
    return 1;
  }

  if (!is_number(argv[2])) {
    fprintf(stderr, "error parsing arguments. second argument must be a numeric port. <host> <port> [-udp]\n");
    return 1;
  }

  int socktype = argc == 4 && strcmp(argv[3], "-udp") == 0 ? SOCK_DGRAM : SOCK_STREAM;
  printf("we are creating a Socket for the protocol %s\n", socktype == SOCK_DGRAM ? "UDP" : "TCP");

  struct addrinfo hints;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET6;
  hints.ai_socktype = socktype;

  struct addrinfo *res;
  if (getaddrinfo(argv[1], argv[2], &hints, &res) != 0) {
    perror("getaddrinfo");
    return 1;
  }

  int sockfd;
  if ((sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) < 0) {
    perror("socket");
    return 1;
  }

  if (socktype == SOCK_STREAM && connect(sockfd, res->ai_addr, res->ai_addrlen) != 0) {
    perror("connect");
    return 1;
  }

  fd_set reads;
  FD_ZERO(&reads);
  FD_SET(sockfd, &reads);
  FD_SET(STDIN_FILENO, &reads);

  struct timeval t;
  t.tv_sec = 0;
  t.tv_usec = 100000;

  for (;;) {
    char buf[BUFSIZ];
    fd_set copy;
    copy = reads;

    if (select(sockfd+1, &copy, 0, 0, &t) < 0) {
      perror("select");
      break;
    }

    if (FD_ISSET(STDIN_FILENO, &copy)) {
      if (!fgets(buf, BUFSIZ, stdin)) continue;
      if (strcmp(buf, ":q\n") == 0) break;
      int bytes = strlen(buf);

      if (socktype == SOCK_STREAM) tcp_send(sockfd, buf, bytes);
      else sendto(sockfd, buf, bytes, 0, res->ai_addr, res->ai_addrlen);

      memset(buf, 0, bytes);
    }

    if (FD_ISSET(sockfd, &copy)) {
      int bytes_recv;
      if (socktype == SOCK_STREAM && (bytes_recv = recv(sockfd, buf, BUFSIZ, 0)) < 1) {
        perror("recv");
        break;
      } else if ((bytes_recv = recvfrom(sockfd, buf, BUFSIZ, 0, res->ai_addr, &res->ai_addrlen)) < 1) {
        perror("recv");
        break;
      }

      printf("%.*s", bytes_recv, buf);
      memset(buf, 0, bytes_recv);
    }
  }
  
  close(sockfd);

  return 0;
}
