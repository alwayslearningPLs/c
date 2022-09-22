#include <sys/socket.h> // socket, connect
#include <sys/types.h>  // AF_INET, SOCK_STREAM
#include <netdb.h>      // getprotobyname
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>     // close

#include <stdio.h>
#include <stdlib.h>     // malloc
#include <errno.h>
#include <string.h>     // it is needed to use strerror_r

#define BUFSIZE 4096
#define IP_ADDRESS "127.0.0.1"
#define PORT 8080
#define REQUEST "GET / HTTP/1.1\r\n" \
  "Host: www.example.com\r\n" \
  "User-Agent: C client\r\n" \
  "Accept: text/html\r\n\r\n"

extern int errno;

void fatal(const char* msg) {
  char* buf = (char*) malloc(sizeof(char) * 50);
  strerror_r(errno, buf, 50);

  fprintf(stderr, "%s %s", msg, buf);
  exit(errno);
}

int main(void) {
  int code,
      sockfd,
      bytes_sent, bytes_received;

  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    fatal("error socket");
  }

  struct sockaddr_in addr_in;

  // htons means host to network short
  addr_in.sin_port = htons(PORT);
  addr_in.sin_family = AF_INET;
  if (inet_pton(addr_in.sin_family, IP_ADDRESS, &addr_in.sin_addr) <= 0) {
    fatal("error inet_pton");
  }

  // int fd, const sockaddr __addr, socklen_t __len
  if ((code = connect(sockfd, (struct sockaddr *) &addr_in, sizeof(addr_in))) < 0 ) {
    fatal("error connect");
  }

  if ((bytes_sent = send(sockfd, REQUEST, strlen(REQUEST), 0)) <= 0) {
    fatal("error send");
  }
  printf("bytes sent to the server: %d\n", bytes_sent);

  char buf[BUFSIZE];
  if ((bytes_received = recv(sockfd, buf, BUFSIZE, 0)) <= 0) {
    fatal("error recv");
  }
  printf("bytes received from the server:\n%.*s", bytes_received, buf);
 
  close(sockfd);

  return 0;
}
