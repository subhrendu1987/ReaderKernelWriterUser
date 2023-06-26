#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/netlink.h>
#include <linux/skbuff.h>
#include <linux/string.h>

#define NETLINK_USER 31

struct sock *nl_sk = NULL;

static void receive_message(struct sk_buff *skb)
{
    struct nlmsghdr *nlh;
    int pid;
    struct sk_buff *skb_out;
    int msg_size;
    char *msg;
    int res;

    nlh = (struct nlmsghdr *)skb->data;
    pid = nlh->nlmsg_pid;
    msg = (char *)NLMSG_DATA(nlh);
    msg_size = strlen(msg);

    skb_out = nlmsg_new(NLMSG_DEFAULT_SIZE, 0);
    if (!skb_out) {
        printk(KERN_ERR "Failed to allocate new skb\n");
        return;
    }

    nlh = nlmsg_put(skb_out, 0, 0, NLMSG_DONE, msg_size, 0);
    NETLINK_CB(skb_out).dst_group = 0;
    strncpy(nlmsg_data(nlh), msg, msg_size);

    res = nlmsg_unicast(nl_sk, skb_out, pid);
    if (res < 0)
        printk(KERN_INFO "Error while sending message to user\n");
}

static int __init init_netlink(void)
{
    struct netlink_kernel_cfg cfg = {
        .input = receive_message,
    };

    nl_sk = netlink_kernel_create(&init_net, NETLINK_USER, &cfg);
    if (!nl_sk) {
        printk(KERN_ALERT "Failed to create netlink socket\n");
        return -ENOMEM;
    }

    printk(KERN_INFO "Netlink socket created\n");
    return 0;
}

static void __exit exit_netlink(void)
{
    if (nl_sk)
        netlink_kernel_release(nl_sk);

    printk(KERN_INFO "Netlink socket closed\n");
}

module_init(init_netlink);
module_exit(exit_netlink);

MODULE_LICENSE("GPL");

MODULE_AUTHOR("Subhrendu Chattopadhyay");
MODULE_DESCRIPTION("Kernel module to interact with a userspace program");
