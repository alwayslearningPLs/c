#include <ifaddrs.h>
#include "personal_ifaddrs.h"
#include <netdb.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define PRINT_MACRO(x) #x

// Interfaces are located inside /sys/class/net/ folder which points to /sys/devices/{name}
// If it is virtual, it will point like that: /sys/class/net/docker0 -> /sys/devices/virtual/net/docker0/
void interfaces() {
  // "/sys/class/net/<iface>/name_assign_type";
  printf("%s\n", PRINT_MACRO(NAME_ASSIGN_TYPE_ENUMERATED_BY_KERNEL));
}

void display_interfaces() {
  struct ifaddrs *ifa;

  if (getifaddrs(&ifa) < 0) {
    perror("getifaddrs");
    return;
  }


  do {
    char c[2][50];
    getnameinfo(ifa->ifa_addr, sizeof(*ifa->ifa_addr), c[0], 50, NULL, 0, NI_NUMERICHOST|NI_NUMERICSERV);
    getnameinfo(ifa->ifa_netmask, sizeof(*ifa->ifa_netmask), c[1], 50, NULL, 0, NI_NUMERICHOST|NI_NUMERICSERV);

    printf("name: %s\thost: %s\tnetmask: %s\tflags: %u\n", ifa->ifa_name, c[0], c[1], ifa->ifa_flags);
  } while ((ifa = ifa->ifa_next) != NULL);
}

int main() {
  interfaces();
  return 0;
}
