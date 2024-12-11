#ifndef NETLINK_H
#define NETLINK_H

#include <linux/netlink.h>
#include <stdint.h>

#define NETLINK_TEST_PROTOCOL 31
int create_netlink_socket(int protocol);
void send_msg_to_kernel(int sock_fd, char *msg, int len);
void exit_userspace(int sock_fd);
int send_netlink_msg_to_kernel(int sock_fd, char *msg, uint32_t msg_size,
                               int nlmsg_type, uint16_t flags);

void *get_msg_from_kernel(void *arg);

#endif  // NETLINK_H
