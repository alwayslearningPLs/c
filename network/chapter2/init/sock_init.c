#include <asm-generic/socket.h>
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

void sockopt_acceptconn() {
  int sockfd;
  struct sockaddr_in sa;
  memset(&sa, 0, sizeof(sa));
  sa.sin_family = AF_INET;
  sa.sin_port = htons(8080);
  sa.sin_addr.s_addr = INADDR_ANY;

  if ((sockfd = socket(sa.sin_family, SOCK_STREAM, 0)) <= 0) {
    perror("socket");
    return;
  }

  if (bind(sockfd, (const struct sockaddr*) &sa, sizeof(sa)) < 0) {
    perror("bind");
    return;
  }

  int optval;
  socklen_t optlen = sizeof(optval);
  if (getsockopt(sockfd, SOL_SOCKET, SO_ACCEPTCONN, &optval, &optlen) != 0) {
    perror("getsockopt");
    return;
  }
  printf("SO_ACCEPTCONN on the SOL_SOCKET is %s\n", optval == 0 ? "not activated" : "activated");

  if (listen(sockfd, 5) < 0) {
    perror("listen");
    return;
  }

  if (getsockopt(sockfd, SOL_SOCKET, SO_ACCEPTCONN, &optval, &optlen) != 0) {
    perror("getsockopt");
    return;
  }
  printf("SO_ACCEPTCONN on the SOL_SOCKET is %s\n", optval == 0 ? "not activated" : "activated");


  close(sockfd);
}

int main() {
  sockopt_acceptconn();
  return 0;
}
