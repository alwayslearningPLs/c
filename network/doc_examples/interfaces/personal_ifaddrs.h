#ifndef __PERSONAL_IFADDRS_H
#define __PERSONAL_IFADDRS_H

#include <features.h>

__BEGIN_DECLS

#define SYS_CLASS_NET "/sys/class/net/"

// Reference: https://www.kernel.org/doc/Documentation/ABI/testing/sysfs-class-net

// What:		/sys/class/net/<iface>/name_assign_type
// Date:		July 2014
// KernelVersion:	3.17
// Contact:	netdev@vger.kernel.org
// Description:
//  Indicates the name assignment type. Possible values are:
//  1  enumerated by the kernel, possibly in an unpredictable way
//  2  predictably named by the kernel
//  3  named by userspace
//  4  renamed
#define NAME_ASSIGN_TYPE_FILE(x) SYS_CLASS_NET x "/name_assign_type"
#define NAME_ASSIGN_TYPE_FROM_VALUE(x)

enum {
  NAME_ASSIGN_TYPE_ENUMERATED_BY_KERNEL = 1,
#define NAME_ASSIGN_TYPE_ENUMERATED_BY_KERNEL NAME_ASSIGN_TYPE_ENUMERATED_BY_KERNEL
  NAME_ASSIGN_TYPE_PREDICTABLY_NAMED_BY_KERNEL = 2,
#define NAME_ASSIGN_TYPE_PREDICTABLY_NAMED_BY_KERNEL NAME_ASSIGN_TYPE_PREDICTABLY_NAMED_BY_KERNEL
  NAME_ASSIGN_TYPE_NAMED_BY_USERSPACE = 3,
#define NAME_ASSIGN_TYPE_NAMED_BY_USERSPACE NAME_ASSIGN_TYPE_NAMED_BY_USERSPACE
  NAME_ASSIGN_TYPE_RENAMED = 4
#define NAME_ASSIGN_TYPE_RENAMED NAME_ASSIGN_TYPE_RENAMED
};

// What:		/sys/class/net/<iface>/addr_assign_type
// Date:		July 2010
// KernelVersion:	3.2
// Contact:	netdev@vger.kernel.org
// Description:
// Indicates the address assignment type. Possible values are:
// 0  permanent address
// 1  randomly generated
// 2  stolen from another device
// 3  set using dev_set_mac_address
#define ADDR_ASSIGN_TYPE_FILE(x) SYS_CLASS_NET x "addr_assign_type"

enum {
  ADDR_ASSIGN_TYPE_PERMANENT_ADDRESS = 0,
#define ADDR_ASSIGN_TYPE_PERMANEN_ADDRESS ADDR_ASSIGN_TYPE_PERMANENT_ADDRESS
  ADDR_ASSIGN_TYPE_RANDOMLY_GENERATED = 1,
#define ADDR_ASSIGN_TYPE_RANDOMLY_GENERATED ADDR_ASSIGN_TYPE_RANDOMLY_GENERATED
  ADDR_ASSIGN_TYPE_STOLEN_FROM_ANOTHER_DEVICE = 2,
#define ADDR_ASSIGN_TYPE_STOLEN_FROM_ANOTHER_DEVICE ADDR_ASSIGN_TYPE_STOLEN_FROM_ANOTHER_DEVICE
  ADDR_ASSIGN_TYPE_SET_USING_DEV_SET_MAC_ADDRESS = 3,
#define ADDR_ASSIGN_TYPE_SET_USING_DEV_SET_MAC_ADDRESS ADDR_ASSIGN_TYPE_SET_USING_DEV_SET_MAC_ADDRESS
};

// What:		/sys/class/net/<iface>/addr_len
// Date:		April 2005
// KernelVersion:	2.6.12
// Contact:	netdev@vger.kernel.org
// Description:
//		Indicates the hardware address size in bytes.
//    Values vary based on the lower-level protocol used by the
//    interface (Ethernet, FDDI, ATM, IEEE 802.15.4...). See
//    include/uapi/linux/if_*.h for actual values.
#define ADDR_LEN_FILE(x) SYS_CLASS_NET x "addr_len"

__END_DECLS

#endif
