#include <linux/kernel.h>
#include <linux/syscalls.h>

asmlinkage long sys_example_syscall(void) {
    example_function(); // Call the function from the kernel module
    return 0;
}

// Define the system call number (replace with a proper, unused number)
#define EXAMPLE_SYSCALL_NUMBER 333

// Register the system call number
EXPORT_SYMBOL(sys_example_syscall);
SYSCALL_DEFINE0(EXAMPLE_SYSCALL_NUMBER, sys_example_syscall);
