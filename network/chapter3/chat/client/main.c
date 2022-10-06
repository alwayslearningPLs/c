#include <bits/types/struct_timeval.h>
#include <sys/select.h>
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

int do_send(int sockfd, const char* buf, size_t buflen) {
  size_t bytes_sent = 0;

  do {
    int tmp;
    if ((tmp = send(sockfd, &buf[bytes_sent], buflen-bytes_sent, 0)) < 1) {
      perror("send");
      break;
    }
    bytes_sent += tmp;
  } while(bytes_sent < buflen);

  return bytes_sent >= buflen;
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
  hints.ai_family = AF_INET6;
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

  int only_ipv6 = 1;
  if (setsockopt(sockfd, IPPROTO_IPV6, IPV6_V6ONLY, &only_ipv6, sizeof(int)) != 0) {
    perror("setsockopt");
    return 1;
  }

  if (connect(sockfd, res->ai_addr, res->ai_addrlen) < 0) {
    perror("connect");
    return 1;
  }
  freeaddrinfo(res);

  fd_set reads;
  FD_SET(sockfd, &reads);
  FD_SET(STDIN_FILENO, &reads); // We can use select for any fd in linux. Awesome
  
  struct timeval timeout;
  timeout.tv_sec = 1;
  timeout.tv_usec = 50000;

  for (;;) {
    char buf[BUFSIZ];
    fd_set copy;

    copy = reads;

    if (select(sockfd+1, &copy, 0, 0, &timeout) < 0) {
      perror("select");
      break;
    }

    if (FD_ISSET(STDIN_FILENO, &copy)) {
      int bytes_sent;

      if (!fgets(buf, BUFSIZ, stdin)) break;
      if (strcmp(buf, ":quit\n") == 0) break;
     
      if (!do_send(sockfd, buf, strlen(buf))) {
        continue;
      }

      memset(buf, 0, bytes_sent);
    }

    if (FD_ISSET(sockfd, &copy)) {
      int bytes_recv;

      if ((bytes_recv = recv(sockfd, buf, BUFSIZ, 0)) < 1) {
        perror("recv");
        break;
      }

      printf("%.*s", bytes_recv, buf); // remember that the string is not null terminated
      memset(buf, 0, bytes_recv);
    }
  }

  close(sockfd);

  return 0;
}
