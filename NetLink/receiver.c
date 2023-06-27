#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/net.h>
#include <linux/socket.h>
#include <linux/in.h>
#include <linux/sched.h>
#include <linux/uaccess.h>
#include <net/sock.h>
#include <linux/version.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 6, 0)
    #define MSGHDR_HAS_MSG_IOV_ITER
#endif

#define SERVER_PORT 12345

static struct socket *conn_socket = NULL;

/*******************************************************
static int handle_message(const char *message){
    // Process the received message from the user program
    // You can perform any desired operations or logic here

    // Example: Print the received message in kernel log
    printk(KERN_INFO "Received message from user program: %s\n", message);

    // Example: Send a response back to the user program
    const char *response = "Hello from kernel module";
    ssize_t bytes_sent = conn_socket->ops->sendmsg(conn_socket, &response, 1, strlen(response), NULL, 0);
    if (bytes_sent < 0) {
        printk(KERN_ERR "Failed to send response to user program: %zd\n", bytes_sent);
        return bytes_sent;
    }

    return 0;
}
/****************************************************/
static int send_message_to_userspace(const char *message, size_t length){
    struct msghdr msg;
    struct kvec iov;
    mm_segment_t oldfs;
    int ret;

    iov.iov_base = (void *)message;
    iov.iov_len = length;
    memset(&msg, 0, sizeof(struct msghdr));
    msg.msg_name = NULL;
    msg.msg_namelen = 0;
    msg.msg_control = NULL;
    msg.msg_controllen = 0;

#ifdef MSGHDR_HAS_MSG_IOV_ITER
    struct iov_iter iter;
    ssize_t written;
    iov_iter_kvec(&iter, WRITE, &iov, 1, iov.iov_len);
    msg.msg_iter = iter;
#else
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;
#endif

    //oldfs = get_ds();
    //set_fs(KERNEL_DS);
    ret = kernel_sendmsg(conn_socket, &msg, iov.iov_len, NULL, 0);
    //set_fs(oldfs);

    if (ret < 0) {
        printk(KERN_ERR "Failed to send message to user program: %d\n", ret);
        return ret;
    }
    printk("Message to user program: %s\n", message);
    return 0;
}
/*******************************************************/
static int connect_to_userspace(struct net *net){
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
/*******************************************************/
static void disconnect_from_userspace(void){
    if (conn_socket) {
        sock_release(conn_socket);
        conn_socket = NULL;
    }
}
/*******************************************************/
static int __init mymodule_init(void){
    int ret;

    // Connect to userspace program
    ret = connect_to_userspace(&init_net);
    if (ret < 0) {
        printk(KERN_ERR "Failed to connect to userspace program\n");
        return ret;
    }
    // Send a test message to the user program
    const char *message = "Test message from kernel module";
    ret = send_message_to_userspace(message, strlen(message));
    if (ret < 0) {
        printk(KERN_ERR "Failed to send test message to userspace program\n");
    }

    printk(KERN_INFO "LKM initialized\n");
    return 0;
}
/*******************************************************/
static void __exit mymodule_exit(void){
    // Disconnect from userspace program
    disconnect_from_userspace();

    printk(KERN_INFO "LKM exited\n");
}
/*******************************************************/
module_init(mymodule_init);
module_exit(mymodule_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Example LKM for communicating with userspace program via socket");
