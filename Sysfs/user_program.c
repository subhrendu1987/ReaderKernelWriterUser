#include <stdio.h>
#include <stdlib.h>

#define SYSFS_INTERACT_PATH "/sys/kernel/interact/interact"

int main() {
    FILE* file = fopen(SYSFS_INTERACT_PATH, "w");
    if (!file) {
        perror("Failed to open sysfs attribute");
        return -1;
    }

    char buff[] = "Hello from userspace!";
    if (fwrite(buff, sizeof(char), sizeof(buff), file) != sizeof(buff)) {
        perror("Failed to write to sysfs attribute");
        fclose(file);
        return -1;
    }

    fclose(file);
    return 0;
}
