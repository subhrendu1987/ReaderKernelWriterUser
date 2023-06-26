#include <stdio.h>
#include <stdlib.h>

#define SYSFS_INTERACT_PATH "/sys/kernel/interact/interact"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <message>\n", argv[0]);
        return 1;
    }

    FILE* file = fopen(SYSFS_INTERACT_PATH, "w");
    if (!file) {
        perror("Failed to open sysfs attribute");
        return -1;
    }

    //char buff[] = "Hello from userspace!";
    //if (fwrite(buff, sizeof(char), sizeof(buff), file) != sizeof(buff)) {
    if (fwrite(argv[1], sizeof(char), sizeof(argv[1]), file) != sizeof(argv[1])) {
        perror("Failed to write to sysfs attribute");
        fclose(file);
        return -1;
    }

    fclose(file);
    return 0;
}
