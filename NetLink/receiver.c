#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/net.h>
#include <linux/socket.h>
#include <linux/in.h>
#include <linux/sched.h>
#include <net/sock.h>

#define SERVER_PORT 12345

static struct socket *conn_socket = NULL;

static int connect_to_userspace(struct net *net)
{
    struct sockaddr_in addr;
    int ret;

    // Create socket
    ret = sock_create_kern(net, AF_INET, SOCK_STREAM, IPPROTO_TCP, &conn_socket);
    if (ret < 0) {
        printk(KERN_ERR "Failed to create socket: %d\n", ret);
        return ret;
    }

    // Set up socket address
    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(SERVER_PORT);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    // Connect the socket
    ret = conn_socket->ops->connect(conn_socket, (struct sockaddr *)&addr,
                                    sizeof(struct sockaddr_in), O_RDWR);
    if (ret < 0) {
        printk(KERN_ERR "Failed to connect to userspace program: %d\n", ret);
        sock_release(conn_socket);
        return ret;
    }

    return 0;
}

static void disconnect_from_userspace(void)
{
    if (conn_socket) {
        sock_release(conn_socket);
        conn_socket = NULL;
    }
}

static int __init mymodule_init(void)
{
    int ret;

    // Connect to userspace program
    ret = connect_to_userspace(&init_net);
    if (ret < 0) {
        printk(KERN_ERR "Failed to connect to userspace program\n");
        return ret;
    }

    printk(KERN_INFO "LKM initialized\n");
    return 0;
}

static void __exit mymodule_exit(void)
{
    // Disconnect from userspace program
    disconnect_from_userspace();

    printk(KERN_INFO "LKM exited\n");
}

module_init(mymodule_init);
module_exit(mymodule_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Example LKM for communicating with userspace program via socket");
