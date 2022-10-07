#!/bin/bash
#
# Learning about ip command in linux so we can understand how to manage network features in our OS

# display only the MAC address of some device
ip link show dev docker0 | grep link | awk '{print $2}'

# Display only ipv4 address from docker0 interface/device
ip -4 address show dev docker0
# Display only ipv6 address from docker0 interface/device
ip -6 address show dev docker0
# Display only ipv4 in format json
ip -4 -json address show dev docker0 | jq .

# Display the MAC address of any interface
while read ifa; do ip link show dev $ifa | grep link | awk -v i=$ifa '{print i, "\t", $2}'; done <<< "$(ls /sys/class/net)"


