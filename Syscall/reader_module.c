#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/uaccess.h>


#define BUFFER_SIZE 256

static char msg[BUFFER_SIZE];
/************************************************/
asmlinkage long sys_my_syscall(char __user *buffer, size_t length){
    int bytesWritten = 0;

    for (bytesWritten = 0; bytesWritten < length && bytesWritten < BUFFER_SIZE; bytesWritten++)
        get_user(msg[bytesWritten], buffer + bytesWritten);

    printk(KERN_INFO "Received message from user program: %s", msg);

    return bytesWritten;
}
/************************************************/
static int __init reader_module_init(void){
    printk(KERN_INFO "Interact LKM initialized\n");
    return 0;
}
/************************************************/
static void __exit reader_module_exit(void){
    printk(KERN_INFO "Interact LKM removed\n");
}
/************************************************/
module_init(interact_init);
module_exit(interact_exit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Subhrendu Chattopadhyay");
MODULE_DESCRIPTION("Kernel module to periodically check and read from a buffer");

