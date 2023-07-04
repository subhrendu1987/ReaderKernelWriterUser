#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/timer.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/hashtable.h>
#include <linux/slab.h>

#define MAX_KEY_SIZE 32
#define MAX_VALUE_SIZE 256
#define DEVICE_PATH "/dev/bpfPolicy"
#define MAX_BUFFER_SIZE 1024

struct kv_pair {
    char key[MAX_KEY_SIZE];
    char value[MAX_VALUE_SIZE];
    struct hlist_node node;
};

DEFINE_HASHTABLE(kvstore, 8);  // Initialize the hash table

int insert_into_kv_store(char* k, char* v){
    struct kv_pair *pair;
    char key[MAX_KEY_SIZE] = "my_key";
    char value[MAX_VALUE_SIZE] = "my_value";
    pair = kmalloc(sizeof(*pair), GFP_KERNEL);
    if (!pair)
        return -ENOMEM;

    strlcpy(pair->key, key, MAX_KEY_SIZE);
    strlcpy(pair->value, value, MAX_VALUE_SIZE);

    //hash_add(kvstore, &pair->node, hash_str(key));  // Insert the key-value pair into the hash table
    hash_add(kvstore, &pair->node, hash_string(key, MAX_KEY_SIZE));


    printk(KERN_INFO "Key-value pair inserted into the kvstore\n");
    return 0;
}



static struct timer_list policy_timer;
static char policy_buffer[MAX_BUFFER_SIZE];

static void policy_timer_callback(struct timer_list *t)
{
    struct file *file;
    mm_segment_t old_fs;
    ssize_t num_bytes;

    file = filp_open(DEVICE_PATH, O_RDONLY, 0);
    if (IS_ERR(file)) {
        pr_alert("Failed to open %s\n", DEVICE_PATH);
        return;
    }

    // Read from the device file
    old_fs = get_fs();
    set_fs(KERNEL_DS);
    num_bytes = kernel_read(file, policy_buffer, MAX_BUFFER_SIZE, &file->f_pos);
    set_fs(old_fs);

    filp_close(file, NULL);

    if (num_bytes < 0) {
        pr_alert("Failed to read from %s\n", DEVICE_PATH);
    } else {
        // fetch the policy from device and store in a data structure
        // ...

        // Re-arm the timer for the next read
        mod_timer(&policy_timer, jiffies + msecs_to_jiffies(5000)); // Read every 5 seconds
    }
}

static int __init bpf_policy_reader_init(void)
{
    init_timer(&policy_timer);
    policy_timer.expires = jiffies + msecs_to_jiffies(5000); // Initial read after 5 seconds
    policy_timer.function = policy_timer_callback;
    add_timer(&policy_timer);

    pr_info("BPF policy reader module initialized\n");
    return 0;
}

static void __exit bpf_policy_reader_exit(void)
{
    del_timer(&policy_timer);
    pr_info("BPF policy reader module exited\n");
}

module_init(bpf_policy_reader_init);
module_exit(bpf_policy_reader_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Kernel module to periodically read from /dev/bpfPolicy");
