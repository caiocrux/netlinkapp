#include <errno.h>
#include <linux/netlink.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "netlink.h"
#include "thread.h"
#include "common.h"

int main(int argc, char **argv) {
    int choice;
    int sock_fd;

    sock_fd = create_netlink_socket(NETLINK_TEST);

    if (sock_fd == -1) {
        printf("Error to create socket! error: %d", errno);
        exit(EXIT_FAILURE);
    }

    struct sockaddr_nl src_addr = {0};
    src_addr.nl_family = AF_NETLINK;
    src_addr.nl_pid = getpid();

    if (bind(sock_fd, (struct sockaddr *)&src_addr, sizeof(src_addr)) == -1) {
        printf("Error to bind\r\n");
        exit(1);
    }

    thread_arg_t thread_arg;
    thread_arg.sock_fd = sock_fd;

    start_kernel_data_receiver_thread(&thread_arg);

    while (1) {
        printf("Netlink Test app ! \r\n");
        printf("1. Send message to kernel \r\n");
        printf("2. Exit test app  \r\n");
        scanf("%d", &choice);
        // Clear the input buffer
        while (getchar() != '\n')
            ;

        switch (choice) {
            case 1: {
                char user_msg[MSG_PAYLOAD] = {0};
                if (fgets(user_msg, MSG_PAYLOAD, stdin) == NULL) {
                    printf("Error in reading stdin!! \r\n");
                    exit(EXIT_FAILURE);
                }
                send_msg_to_kernel(sock_fd, user_msg, strlen(user_msg));
            } break;
            case 2:
                exit_userspace(sock_fd);
                exit(1);
                break;
            default:
                printf("Option not available ! \r\n");
                break;
        }
    }
}
