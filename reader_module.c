#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/kthread.h>
#include <linux/delay.h>

#define BUFFER_SIZE 1024
static int major_number = 0;
struct file *file;
struct inode *inode;

static char *buffer;
static struct task_struct *reader_thread;
/************************************************/
static int reader_func(void *data){
    while (!kthread_should_stop()){
        // Check if buffer has data
        if (buffer != NULL){
            // Read from buffer
            char local_buffer[BUFFER_SIZE];
            memset(local_buffer, 0, BUFFER_SIZE);
            strncpy(local_buffer, buffer, BUFFER_SIZE);

            // Print the read data
            printk(KERN_INFO "Data read from buffer: %s\n", local_buffer);

            // Reset the buffer
            kfree(buffer);
            buffer = NULL;
        }
        // Sleep for some time before checking the buffer again
        msleep(1000);
    }
    return 0;
}
/************************************************/
static ssize_t reader_module_write(struct file *file, const char __user *buffer_user, size_t count, loff_t *ppos){
    // Allocate memory for the buffer
    if (buffer == NULL){
        buffer = kmalloc(BUFFER_SIZE, GFP_KERNEL);
        if (buffer == NULL){
            printk(KERN_ALERT "Failed to allocate memory for buffer\n");
            return -ENOMEM;
        }
    }

    // Clear the buffer
    memset(buffer, 0, BUFFER_SIZE);

    // Copy the data from user space to kernel space
    if (copy_from_user(buffer, buffer_user, count) != 0){
        printk(KERN_ALERT "Failed to copy data from user space\n");
        return -EFAULT;
    }
    return count;
}
/************************************************/
static struct file_operations fops = {
    .write = reader_module_write,
};
/************************************************/
int set_permission(int major_number){
    inode = get_inode(major_number);
    if (!inode) {
        printk(KERN_ALERT "Failed to get inode for major number %u\n", major_number);
        return -ENODEV;
    }
    // Get the file structure for the inode
    file = file_inode(inode);
    if (!file) {
        printk(KERN_ALERT "Failed to get file for inode\n");
        iput(inode);
        return -ENODEV;
    }
    // Set the permissions
    file->f_path.dentry->d_inode->i_mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP;
    return 0;
}
/************************************************/
static int __init reader_module_init(void){
    // Create a character device for reader module
     major_number = register_chrdev(0, "reader_module", &fops);
    if (major_number  < 0){
        printk(KERN_ALERT "Failed to register character device\n");
        return -EFAULT;
    }else{
	printk("New device created with Major Number: %d\n",major_number);
	ret=set_permission(major_number);
	if(ret < 0){ return -1;}
    }

    // Start the reader thread
    reader_thread = kthread_run(reader_func, NULL, "reader_reader");
    if (IS_ERR(reader_thread)){
        printk(KERN_ALERT "Failed to create reader thread\n");
        unregister_chrdev(0, "reader_module");
        return PTR_ERR(reader_thread);
    }

    printk(KERN_INFO "Reader module initialized\n");
    return 0;
}
/************************************************/
static void __exit reader_module_exit(void){
    // Stop the reader thread
    if (reader_thread != NULL){
        kthread_stop(reader_thread);
        reader_thread = NULL;
    }
    // Unregister the character device
    unregister_chrdev(0, "reader_module");
    // Free the buffer memory
    if (buffer != NULL){
        kfree(buffer);
        buffer = NULL;
    }
    unlink("/dev/reader_module");
    printk(KERN_INFO "Reader module exited\n");
}
/************************************************/
module_init(reader_module_init);
module_exit(reader_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Subhrendu Chattopadhyay");
MODULE_DESCRIPTION("Kernel module to periodically check and read from a buffer");

