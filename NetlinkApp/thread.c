#include <linux/netlink.h>  // For NLMSG_HDRLEN, NLMSG_SPACE, NLMSG_DATA
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>      // For calloc
#include <string.h>      // For memset
#include <sys/socket.h>  // For msghdr, recvmsg
#include <unistd.h>      // Include this for the read function

#include "netlink.h"
#include "thread.h"

void start_kernel_data_receiver_thread(thread_arg_t *arg) {
    pthread_attr_t attr;
    pthread_t recv_pkt_thread;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    pthread_create(&recv_pkt_thread, &attr, get_msg_from_kernel, (void *)arg);
}
