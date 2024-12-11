#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>  // For getpid() and close()

#include "netlink.h"
#include "thread.h"

int create_netlink_socket(int protocol) {
    return socket(PF_NETLINK, SOCK_RAW, protocol);
}

void send_msg_to_kernel(int sock_fd, char *msg, int len) {
    send_netlink_msg_to_kernel(sock_fd, msg, len, 1, NLM_F_ACK);
}
int send_netlink_msg_to_kernel(int sock_fd, char *msg, uint32_t msg_size,
                               int nlmsg_type, uint16_t flags) {
    struct sockaddr_nl dest_addr = {0};
    struct iovec iov = {0};
    static struct msghdr outermsghdr = {0};

    int rc = 0;

    dest_addr.nl_family = AF_NETLINK;
    dest_addr.nl_pid = 0;

    struct nlmsghdr *nlh =
        (struct nlmsghdr *)calloc(1, NLMSG_HDRLEN + NLMSG_SPACE(msg_size));

    nlh->nlmsg_len = NLMSG_HDRLEN + NLMSG_SPACE(msg_size);
    nlh->nlmsg_pid = getpid();
    nlh->nlmsg_flags = flags;
    nlh->nlmsg_type = nlmsg_type;
    nlh->nlmsg_seq = 0;

    strncpy(NLMSG_DATA(nlh), msg, msg_size);

    iov.iov_base = (void *)nlh;
    iov.iov_len = nlh->nlmsg_len;

    outermsghdr.msg_name = (void *)&dest_addr;
    outermsghdr.msg_namelen = sizeof(dest_addr);
    outermsghdr.msg_iov = &iov;
    outermsghdr.msg_iovlen = 1;

    rc = sendmsg(sock_fd, &outermsghdr, 0);

    if (rc < 0) {
        printf("Sending Falied! Reason : %d ", errno);
    }
    return rc;
}

void *get_msg_from_kernel(void *arg) {
    int rc = 0;
    // This structure describes a buffer for I/O operations.
    struct iovec iov;
    // This structure defines the header of a Netlink message. It is used by
    // both the kernel and user-space to format messages exchanged via Netlink
    // sockets.
    struct nlmsghdr *nlh_recv = NULL;
    // This structure is used by the recvmsg() and sendmsg() system calls to
    // describe messages
    static struct msghdr outermsghdr;
    int sock_fd = 0;

    thread_arg_t *thread_arg = (thread_arg_t *)arg;
    sock_fd = thread_arg->sock_fd;

    /*Take a new buffer to recv data from kernel*/
    nlh_recv =
        (struct nlmsghdr *)calloc(1, NLMSG_HDRLEN + NLMSG_SPACE(MAX_PAYLOAD));

    do {
        memset(nlh_recv, 0, NLMSG_HDRLEN + NLMSG_SPACE(MAX_PAYLOAD));

        iov.iov_base = (void *)nlh_recv;
        iov.iov_len = NLMSG_HDRLEN + NLMSG_SPACE(MAX_PAYLOAD);

        memset(&outermsghdr, 0, sizeof(struct msghdr));

        outermsghdr.msg_iov = &iov;
        outermsghdr.msg_name = NULL;
        outermsghdr.msg_iovlen = 1;
        outermsghdr.msg_namelen = 0;

        /* Read message from kernel. Its a blocking system call
         * Application execuation is suspended at this point
         * and would not resume until it receives linux kernel
         * msg. We can configure recvmsg() to not to block,
         * but lets use it in blocking mode for now */

        rc = recvmsg(sock_fd, &outermsghdr, 0);

        /* We have successfully received msg from linux kernel*/
        /* print the msg from kernel. kernel msg shall be stored
         * in outermsghdr.msg_iov->iov_base
         * in same format : that is Netlink hdr followed by payload data*/
        nlh_recv = outermsghdr.msg_iov->iov_base;
        char *payload = NLMSG_DATA(nlh_recv);

        printf("Received Netlink msg from kernel, bytes recvd = %d\n", rc);
        printf("msg recvd from kernel = %s\n", payload);
    } while (1);
}

void exit_userspace(int sock_fd) { close(sock_fd); }
