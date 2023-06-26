#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>

#define SYS_MY_SYSCALL_NR __NR_my_syscall
#define BUFFER_SIZE 256

int main() {
    char input[BUFFER_SIZE];

    printf("Enter a message to send to the LKM: ");
    fgets(input, BUFFER_SIZE, stdin);

    // Invoke the system call to communicate with the LKM
    syscall(SYS_MY_SYSCALL_NR, input, sizeof(input));

    return 0;
}
