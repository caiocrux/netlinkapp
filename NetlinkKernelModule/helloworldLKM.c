#include <linux/module.h> /* Needed by all modules */
#include <linux/netlink.h>
#include <linux/string.h>
#include <net/sock.h>

#include "common.h"

static struct sock *netlink_socket = NULL;

static void nlmsg_dump(struct nlmsghdr *nlh) {
    if (!nlh) {
        printk(KERN_ERR "Netlink Dump: Invalid nlmsghdr pointer (NULL)\n");
        return;
    }
    printk(KERN_DEBUG
           "Netlink Dump: Length: %u, Type: %u, Flags: %u, Seq: %u, PID: %u\n",
           nlh->nlmsg_len, nlh->nlmsg_type, nlh->nlmsg_flags, nlh->nlmsg_seq,
           nlh->nlmsg_pid);
}

static void netlink_recv_msg_fn(struct sk_buff *skb_in) {
    struct nlmsghdr *nlh_recv = {0}, *nlh_reply = {0};
    int user_space_port_id = 0;
    int user_space_len = 0;
    char *user_space_data;
    char kernel_reply[256] = {0};
    int res = 0;

    struct sk_buff *skb_out;

    // Log the input skb structure
    printk(KERN_DEBUG
           "Netlink message received: skb_in address: %p, length: %u\n",
           skb_in, skb_in->len);

    // Cast skb_in to nlmsghdr
    nlh_recv = (struct nlmsghdr *)(skb_in->data);

    // Debugging the received Netlink message header
    if (!nlh_recv) {
        printk(KERN_ERR "Received null nlmsghdr\n");
        return;
    }
    nlmsg_dump(nlh_recv);

    // Extract and log the sender's port ID
    user_space_port_id = nlh_recv->nlmsg_pid;
    printk(KERN_DEBUG "User space port ID: %d\n", user_space_port_id);

    // Extract and log the data length and data content
    user_space_data = (char *)nlmsg_data(nlh_recv);
    if (!user_space_data) {
        printk(KERN_ERR "User space data is NULL\n");
    } else {
        printk(KERN_DEBUG "User space data length: %u, content: %s\n",
               skb_in->len, user_space_data);
    }

    // Log the total skb_in length
    user_space_len = skb_in->len;
    printk(KERN_DEBUG "Total skb_in length: %d\n", user_space_len);

    if (nlh_recv->nlmsg_flags & NLM_F_ACK) {
        printk(KERN_INFO "Sending back the message to user space");
        snprintf(kernel_reply, sizeof(kernel_reply),
                 "Msg from %d process has been processed by kernel",
                 nlh_recv->nlmsg_pid);
        skb_out = nlmsg_new(sizeof(kernel_reply), 0);
        nlh_reply = nlmsg_put(skb_out, 0, nlh_recv->nlmsg_seq, NLMSG_DONE,
                              sizeof(kernel_reply), 0);

        strlcpy(nlmsg_data(nlh_reply), kernel_reply, sizeof(kernel_reply));
        res = nlmsg_unicast(netlink_socket, skb_out, user_space_port_id);
        if (res < 0) {
            printk(KERN_INFO "Error to send back the message to user space");
            kfree_skb(skb_out);
        } else {
            printk(KERN_INFO "ignore the message from userspace");
        }
    }
}
static struct netlink_kernel_cfg cfg = {
    .input = netlink_recv_msg_fn,
};

int hello_world_init_module(void) {
    printk(KERN_INFO "Hello world Module Loaded Successfully\n");
    netlink_socket = netlink_kernel_create(&init_net, NETLINK_TEST, &cfg);
    if (!netlink_socket) {
        printk(KERN_INFO "Netlink socket %u failed \n", NETLINK_TEST);
        return -ENOMEM;
    }
    printk(KERN_INFO "Netlink socket created Successfully\n");
    return 0;
}
void hello_world_cleanup_module(void) {
    printk(KERN_INFO "Goodbye hello world LKM\n");
    netlink_kernel_release(netlink_socket);
}

module_init(hello_world_init_module);
module_exit(hello_world_cleanup_module);

/*Module Information*/
#define AUTHOR_NAME "Caio Cruz"
#define MODULE_DESC "hello world"

MODULE_AUTHOR(AUTHOR_NAME);
MODULE_DESCRIPTION(MODULE_DESC);
MODULE_LICENSE("GPL");
