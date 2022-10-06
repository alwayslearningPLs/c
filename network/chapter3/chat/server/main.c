#include <asm-generic/socket.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

extern int errno;

int is_number(char *str) {
  char *tmp = str;
  while (*tmp != '\0' && *tmp >= 0x30 && *tmp <= 0x39) tmp++;
  return *tmp == '\0';
}

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "couldn't parse arguments correctly. You need to pass the <port> as first argument.\n");
    return 1;
  }

  if (!is_number(argv[1])) {
    fprintf(stderr, "port argument contains characters that are not numbers");
    return 1;
  }

  int sockfd, max_socket;
  struct addrinfo hints;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET6;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;
 
  struct addrinfo *res;
  if (getaddrinfo(0, argv[1], &hints, &res) != 0) {
    perror("getaddrinfo");
    return 1;
  }

  if ((sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) < 0) {
    perror("socket");
    return 1;
  }

  int ipv6_only = 1;
  if (setsockopt(sockfd, IPPROTO_IPV6, IPV6_V6ONLY, &ipv6_only, sizeof(ipv6_only)) != 0) {
    perror("setsockopt");
    return 1;
  }

  int reuse_addr = 1;
  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse_addr, sizeof(reuse_addr)) != 0) {
    perror("setsockopt");
    return 1;
  }

  // Assigning a name to a socket
  if (bind(sockfd, res->ai_addr, res->ai_addrlen)) {
    perror("bind");
    return 1;
  }
  freeaddrinfo(res);

  if (listen(sockfd, 10) != 0) {
    perror("listen");
    return 1;
  }

  max_socket = sockfd;
  fd_set master;
  FD_ZERO(&master);
  FD_SET(max_socket, &master);

  for (;;) {
    fd_set reads;
    reads = master; // we need to copy the actual sockets, because select will modify the sockets to the ones that are available/ready.
    if (select(max_socket+1, &reads, 0, 0, 0) < 0) { // Here we are blocking until some of the sockets inside the reads set is ready to read. We can pass as the last paramter the structure timeval to specify how much time we can wait for. struct timeval { long tv_sec; long tv_usec; };
      perror("select");
      continue;
    }

    for (int fd = 1; fd <= max_socket; fd++) {
      if (FD_ISSET(fd, &reads)) {
        if (fd == sockfd) { // this is the server socket, the one that is listening
          struct sockaddr_storage addr_client;
          socklen_t laddr_client = sizeof(addr_client);
          int sock_client;
          if ((sock_client = accept(fd, (struct sockaddr*) &addr_client, &laddr_client)) < 0) {
            perror("accept");
            continue;
          }

          FD_SET(sock_client, &master);
          if (sock_client > max_socket) {
            max_socket = sock_client;
          }
          
          char client[2][50];
          if (getnameinfo((const struct sockaddr*) &addr_client, laddr_client, client[0], 50, client[1], 50, NI_NUMERICHOST) != 0) {
            perror("getnameinfo");
            continue;
          }

          printf("connection with: [%s]:%s\n", client[0], client[1]);
        } else {
          char read[BUFSIZ];
          int bytes_recv;
          if ((bytes_recv = recv(fd, read, BUFSIZ, 0)) < 1) {
            perror("recv");
            FD_CLR(fd, &master);
            continue;
          }

          if (strcmp(read, "bye") == 0) {
            FD_CLR(fd, &master);
            printf("removing the fd %d from the fd set\n", fd);
            continue;
          }

          for (int fdr = 1; fdr <= max_socket; fdr++) {
            // if it is a socket fd from the master and it is not the master nor the sender, we send the content.
            if (FD_ISSET(fdr, &master) && fdr != sockfd && fdr != fd) {
              send(fdr, read, bytes_recv, 0);
            }
          }
        }
      }
    }
  }

  // FD_ISSET(sockfd, &sockets)
  // FD_CLR(sockfd, &sockets)

  close(sockfd);

  return 0;
}
