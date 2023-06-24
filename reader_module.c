#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/mutex.h>

#define BUFFER_SIZE 1024
#define MODULE_FILE_NAME "reader_module"

static char* buffer;
DEFINE_MUTEX(buffer_mutex);

static struct task_struct *reader_thread;

static int read_buffer(void *data) {
    while (!kthread_should_stop()) {
        mutex_lock(&buffer_mutex);

        // Read from the buffer
        printk(KERN_INFO "Reader: Buffer contents: %s\n", buffer);

        // Reset the buffer after reading
        memset(buffer, 0, BUFFER_SIZE);

        mutex_unlock(&buffer_mutex);

        // Sleep for some time before checking again
        msleep(1000);
    }

    return 0;
}

ssize_t write_buffer(struct file *file, const char __user *user_buffer, size_t count, loff_t *ppos) {
    ssize_t bytes_written = 0;

    if (mutex_lock_interruptible(&buffer_mutex)) {
        return -ERESTARTSYS;
    }

    // Copy data from the user space to the buffer
    if (copy_from_user(buffer, user_buffer, count) != 0) {
        mutex_unlock(&buffer_mutex);
        return -EFAULT;
    }

    mutex_unlock(&buffer_mutex);

    bytes_written = count;
    return bytes_written;
}

static const struct file_operations buffer_fops = {
    .write = write_buffer,
};

static int __init buffer_module_init(void) {
    int ret = 0;

    buffer = kmalloc(BUFFER_SIZE, GFP_KERNEL);
    if (!buffer) {
        printk(KERN_ALERT "Failed to allocate buffer memory\n");
        return -ENOMEM;
    }

    memset(buffer, 0, BUFFER_SIZE);

    ret = register_chrdev(0, MODULE_FILE_NAME, &buffer_fops);
    if (ret < 0) {
        kfree(buffer);
        printk(KERN_ALERT "Failed to register reader module\n");
        return ret;
    }else{
        printk(KERN_ALERT "Reader module Created /dev/%s",MODULE_FILE_NAME);
    }

    reader_thread = kthread_run(read_buffer, NULL, "reader_thread");
    if (IS_ERR(reader_thread)) {
        unregister_chrdev(0, MODULE_FILE_NAME);
        kfree(buffer);
        printk(KERN_ALERT "Failed to create reader thread\n");
        return PTR_ERR(reader_thread);
    }
    if (access(MODULE_FILE_NAME, F_OK) == 0) {
        printk(KERN_INFO "/dev/%s exists\n",MODULE_FILE_NAME);
    } else {
        printk(KERN_INFO "/dev/%s does not exist\n",MODULE_FILE_NAME);
    }

    printk(KERN_INFO "reader module loaded\n");
    return 0;
}

static void __exit buffer_module_exit(void) {
    if (reader_thread) {
        kthread_stop(reader_thread);
    }

    unregister_chrdev(0, MODULE_FILE_NAME);
    kfree(buffer);

    printk(KERN_INFO "Buffer module unloaded\n");
}

module_init(buffer_module_init);
module_exit(buffer_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Subhrendu Chattopadhyay");
MODULE_DESCRIPTION("Kernel module for periodically checking and reading from a buffer");

