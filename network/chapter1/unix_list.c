#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int check(struct ifaddrs *addresses);

int main(void) {
  struct ifaddrs *addresses;

  if (getifaddrs(&addresses) == -1) {
    printf("getifaddrs error\n");
    return -1;
  }

  int count = 1;
  int max = check(addresses);
  struct ifaddrs *address = addresses;
  while(address) {
    int family = address->ifa_addr->sa_family;
    if (family == AF_INET || family == AF_INET6) {
      printf("%d: %*s:%*s ", count, count < 10 ? 2 : 0, address->ifa_name, (int) (max - strlen(address->ifa_name)), "");
      printf("\t%s", family == AF_INET ? "inet" : "inet6");

      char ap[100];
      const int family_size = family == AF_INET ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6);
      getnameinfo(address->ifa_addr, family_size, ap, family_size, 0, 0, NI_NUMERICHOST);

      printf("\t%s\n", ap);
      count++;
    }

    address = address->ifa_next;
  }

  freeifaddrs(addresses);

  return 0;
}

int check(struct ifaddrs *addresses)  {
  int max = 0;
  struct ifaddrs *tmp = addresses;

  while (tmp) {
    int t;
    int family = tmp->ifa_addr->sa_family;
    if (family == AF_INET || family == AF_INET6)
      if ((t = strlen(tmp->ifa_name)) > max) max = t;
    
    tmp = tmp->ifa_next;
  }
  
  return max;
}
