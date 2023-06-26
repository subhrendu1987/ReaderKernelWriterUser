#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sysfs.h>
#include <linux/kobject.h>


struct kobject *interact_kobj;
/**********************************************************/
// Function to be called from userspace
int interact(char* buff) {
    printk(KERN_INFO "User message: %s\n", buff);
    return 0;
}
/**********************************************************/
// sysfs entry write handler
static ssize_t interact_sysfs_write(struct kobject *kobj, struct kobj_attribute *attr,
                                    const char *buf, size_t count) {
    interact(buf);
    return count;
}
/**********************************************************/
// sysfs attribute structure
static struct kobj_attribute interact_attribute =
    __ATTR(interact, 0220, NULL, interact_sysfs_write);
/**********************************************************/
// Initialize the LKM
static int __init interact_lkm_init(void) {
    // Create a sysfs directory
    interact_kobj = kobject_create_and_add("interact", kernel_kobj);
    if (!interact_kobj) {
        printk(KERN_ERR "Failed to create sysfs directory\n");
        return -ENOMEM;
    }

    // Create the sysfs attribute
    int result = sysfs_create_file(interact_kobj, &interact_attribute.attr);
    if (result) {
        printk(KERN_ERR "Failed to create sysfs attribute\n");
        kobject_put(interact_kobj);
        return result;
    }

    printk(KERN_INFO "Interact LKM initialized\n");
    return 0;
}
/**********************************************************/
// Cleanup and unload the LKM
static void __exit interact_lkm_exit(void) {
    // Remove the sysfs attribute
    struct kobject *interact_kobj = NULL;
    struct kernfs_node *kn = kernel_kobj->sd;

    if (kn) {
        printk(KERN_INFO "Unlink file in /sys\n");
        interact_kobj = kn->parent->priv;
        sysfs_remove_file(interact_kobj, &interact_attribute.attr);
        kobject_put(interact_kobj);
        kernfs_put(kn);
    }else{

    }

    printk(KERN_INFO "Interact LKM exited\n");
}
/**********************************************************/


// Specify the LKM initialization and exit functions
module_init(interact_lkm_init);
module_exit(interact_lkm_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Subhrendu Chattopadhyay");
MODULE_DESCRIPTION("Kernel module to interact with a userspace program");
