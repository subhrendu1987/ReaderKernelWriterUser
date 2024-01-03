#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

void example_function(void) {
    printk(KERN_INFO "Example function called from the kernel module.\n");
}

EXPORT_SYMBOL(example_function);

static int __init example_module_init(void) {
    printk(KERN_INFO "Example module loaded.\n");
    return 0;
}

static void __exit example_module_exit(void) {
    printk(KERN_INFO "Example module unloaded.\n");
}

module_init(example_module_init);
module_exit(example_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Example Kernel Module");
