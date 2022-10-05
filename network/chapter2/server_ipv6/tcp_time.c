#include <asm-generic/socket.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>

#define RESPONSE "HTTP/1.1 200 OK\r\n" \
  "Connection: close\r\n" \
  "Content-Type: text/plain\r\n\r\n" \
  "Local time now: "

extern int errno;

int is_number(char *s) {
  char *tmp = s;
  while (*tmp != '\0' && *tmp >= 0x30 && *tmp <= 0x39) tmp++;
  return *tmp == '\0';
}

// curl http://[::1]:<port>
int main(int argc, char **argv) {
  int sockfd;
  struct addrinfo hints;

  if (argc != 2) {
    fprintf(stderr, "possible arguments are only <port>\n");
    return 1;
  }

  if (!is_number(argv[1])) {
    fprintf(stderr, "the port is not a number\n");
    return 1;
  }

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET6;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  struct addrinfo *res;
  if (getaddrinfo(0, argv[1], &hints, &res) != 0) {
    perror("getaddrinfo");
    exit(1);
  }

  if ((sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) <= 0) {
    perror("socket");
    return 1;
  }

  int optval = 1;
  if (setsockopt(sockfd, IPPROTO_IPV6, IPV6_V6ONLY, &optval, sizeof(optval)) != 0) {
    perror("setsockopt");
    return 1;
  }

  if (bind(sockfd, (const struct sockaddr *) res->ai_addr, sizeof(*res)) != 0) {
    perror("bind");
    return 1;
  }

  freeaddrinfo(res);

  if (listen(sockfd, 5) != 0) {
    perror("listen");
    return 1;
  }

  char *buf[BUFSIZ];
  for (;;) {
    int bytes_received,
        bytes_sent,
        client_sock;
    struct addrinfo client_addr;
    socklen_t lclient_addr = sizeof(client_addr);

    if ((client_sock = accept(sockfd, (struct sockaddr *) &client_addr, &lclient_addr)) <= 0) {
      perror("accept");
      continue;
    }

    memset(&buf, 0, BUFSIZ);
    if ((bytes_received = recv(client_sock, buf, BUFSIZ, 0)) <= 0) {
      perror("recv");
      continue;
    }
    
    if ((bytes_sent = send(client_sock, RESPONSE, strlen(RESPONSE), 0)) <= 0) {
      close(client_sock);
      continue;
    }
   
    time_t t;
    time(&t);
    char* date = ctime(&t);
    if ((bytes_sent = send(client_sock, date, strlen(date), 0)) <= 0) {
      close(client_sock);
      continue;
    }

    close(client_sock);
  }

  freeaddrinfo(res);
  close(sockfd);
  
  return 0;
}
