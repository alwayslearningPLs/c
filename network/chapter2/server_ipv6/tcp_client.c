#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define PORT "8080"
#define BUFSIZE 4096

#define GET_METHOD "GET"

extern int errno;

// TODO: this program is not finishes yet
void fatal(const char* msg) {
  char* buf = (char *) malloc(50 * sizeof(char));
  strerror_r(errno, buf, sizeof(buf));
  fprintf(stderr, "%s %s\n", msg, buf);
  exit(1);
}

int parse_first_line(char* request, int bytes, char* dest) {
  char *tmp = (char*) malloc(sizeof(char) * bytes);
  strcpy(tmp, request);
  
  char* ptr = strtok(tmp, " ");
  if (ptr == NULL || strcmp(tmp, GET_METHOD)) {
    return -1;
  }
  int len = strlen(tmp) + 1;

  ptr = strtok(NULL, " ");
  if (ptr == NULL || strcmp(tmp + len, "/datetime")) {
    return -1;
  }
  len = len + strlen(tmp + len) + 1;

  ptr = strtok(NULL, " ");
  if (ptr == NULL || strcmp(tmp + len, "HTTP/1.1")) {
    return -1;
  } 

  return 0;
}

int main(void) {
  char* request = "GET / HTTP/1.1\r\nAccept: */*\r\nHost: myhouse\r\n\r\n";
  char* dest = (char*) malloc(sizeof(char) * 50);
  unsigned int len = strcspn(request, "\r\n");

  int result = parse_first_line(request, len, dest);

  printf("here the result: %d\n", result);

  return 0;
}

int main_1(void) {
  int sockfd;

  struct addrinfo hints;
  hints.ai_family = AF_INET6;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;
  
  struct addrinfo *addr;
  if (getaddrinfo(0, PORT, &hints, &addr) < 0) {
    fatal("error getaddrinfo");
  }

  if ((sockfd = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol)) < 0) {
    fatal("error socket");
  }

  if (bind(sockfd, (struct sockaddr*) addr, addr->ai_addrlen) < 0) {
    fatal("error bind");
  }

  if (listen(sockfd, 10) < 0) {
    fatal("error listen");
  }

  for (;;) {
    int client_sock,
        bytes_sent,
        bytes_received;
    struct sockaddr_storage client;
    socklen_t client_len;

    if ((client_sock = accept(sockfd, (struct sockaddr*) &client, &client_len)) < 0 ) {
      continue;
    }

    char client_addr[100];
    getnameinfo((struct sockaddr*) &client, client_len, client_addr, 100, 0, 0, NI_NUMERICHOST);

    char request[BUFSIZE];
    if ((bytes_received = recv(client_sock, request, sizeof(request), 0)) <= 0) {
      close(client_sock);
      continue;
    }

  }

  close(sockfd);

  return 0;
}

