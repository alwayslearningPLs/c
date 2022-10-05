#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

extern int errno;

int is_numeric(char *input) {
  char *tmp = input;
  while (*tmp != '\0' && *tmp >= 0x30 && *tmp <= 0x39) tmp++;
  return *tmp == '\0';
}

int main(int argc, char **argv) {
  if (argc != 3) {
    fprintf(stderr, "error parsing arguments. We need <IP-address> <port> to connect to the chat server\n");
    return 1;
  }

  if (!is_numeric(argv[2])) {
    fprintf(stderr, "error parsing second argument. Port must be a number. <IP-address> <port>\n");
    return 1;
  }

  struct addrinfo hints;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;

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

  if (connect(sockfd, res->ai_addr, res->ai_addrlen) < 0) {
    perror("connect");
    return 1;
  }
  freeaddrinfo(res);

  for (;;) {
    int bytes_sent, bytes_received;
    char buf[BUFSIZ];

    fgets(buf, BUFSIZ, stdin);

    if ((bytes_sent = send(sockfd, buf, strlen(buf), 0)) < 1) {
      perror("send");
      continue;
    }

    memset(buf, 0, bytes_sent);
    if (recv(sockfd, buf, BUFSIZ, 0) < 1) {
      perror("recv");
      continue;
    }

    printf(buf);

    memset(buf, 0, bytes_sent);
  }

  close(sockfd);

  return 0;
}
