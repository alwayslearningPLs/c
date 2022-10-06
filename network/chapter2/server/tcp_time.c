#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>

#define PROTOCOL "TCP"
#define PORT "8080"
#define BUFSIZE 1024
#define RESPONSE "HTTP/1.1 200 OK\r\n" \
  "Connection: close\r\n" \
  "Content-Type: text/plain\r\n\r\n" \
  "Local time now: "

extern int errno;

void fatal(const char* msg) {
  char* buf = (char*) malloc(sizeof(char) * 50);
  strerror_r(errno, buf, 50);

  fprintf(stderr, "%s %s", msg, buf);
  exit(errno);
}

int main(void) {
  int sockfd;
  
  struct protoent *p;
  if ((p = getprotobyname(PROTOCOL)) < 0) {
    fatal("error getprotobyname");
  }

  struct addrinfo hints;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE; // Socket address is intended for `bind'

  struct addrinfo *addr;
  if (getaddrinfo(0, PORT, &hints, &addr) < 0) {
    fatal("error getaddrinfo");
  }

  if ((sockfd = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol)) < 0) {
    fatal("error socket");
  }

  if (bind(sockfd, addr->ai_addr, addr->ai_addrlen)) {
    fatal("error bind");
  }

  freeaddrinfo(addr);

  if (listen(sockfd, 10)) {
    fatal("error listen");
  }

  for (;;) {
    int client_sock,
        bytes_received,
        bytes_sent;
    struct sockaddr_storage client;
    socklen_t client_len = sizeof(client);

    if ((client_sock = accept(sockfd, (struct sockaddr*) &client, &client_len)) < 0) {
      continue;
    }

    char request[BUFSIZE];
    if (getnameinfo((struct sockaddr*) &client, client_len, request, BUFSIZE, 0, 0, NI_NUMERICHOST) < 0) {
      close(client_sock);
      continue;
    }
    printf("GET HTTP / from %s\n", request);
   
    char buf[BUFSIZE];
    if ((bytes_received = recv(client_sock, buf, BUFSIZE, 0)) <= 0) {
      close(client_sock);
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
  
  close(sockfd);
  freeaddrinfo(addr);

  return 0;
}
