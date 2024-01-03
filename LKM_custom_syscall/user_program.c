#include <stdio.h>
#include <unistd.h>

#define EXAMPLE_SYSCALL_NUMBER 437 // Replace with the actual system call number

int main() {
    long ret = syscall(EXAMPLE_SYSCALL_NUMBER);

    if (ret == 0) {
        printf("System call executed successfully.\n");
    } else {
        perror("System call failed");
    }

    return 0;
}
