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

    ret = register_chrdev(0, "buffer_module", &buffer_fops);
    if (ret < 0) {
        kfree(buffer);
        printk(KERN_ALERT "Failed to register buffer module\n");
        return ret;
    }

    reader_thread = kthread_run(read_buffer, NULL, "reader_thread");
    if (IS_ERR(reader_thread)) {
        unregister_chrdev(0, "buffer_module");
        kfree(buffer);
        printk(KERN_ALERT "Failed to create reader thread\n");
        return PTR_ERR(reader_thread);
    }

    printk(KERN_INFO "Buffer module loaded\n");
    return 0;
}

static void __exit buffer_module_exit(void) {
    if (reader_thread) {
        kthread_stop(reader_thread);
    }

    unregister_chrdev(0, "buffer_module");
    kfree(buffer);

    printk(KERN_INFO "Buffer module unloaded\n");
}

module_init(buffer_module_init);
module_exit(buffer_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Kernel module for periodically checking and reading from a buffer");

