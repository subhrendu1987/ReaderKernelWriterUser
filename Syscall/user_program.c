#include <stdio.h>
#include <dlfcn.h>

int main() {
    void (*lkm_function)(void);

    // Load the LKM dynamically
    void *handle = dlopen("./reader_module.ko", RTLD_NOW);
    if (!handle) {
        fprintf(stderr, "Failed to load LKM: %s\n", dlerror());
        return 1;
    }

    // Resolve the LKM function address
    lkm_function = dlsym(handle, "my_lkm_function");
    if (!lkm_function) {
        fprintf(stderr, "Failed to resolve LKM function: %s\n", dlerror());
        dlclose(handle);
        return 1;
    }

    // Call the LKM function
    lkm_function();

    // Unload the LKM
    dlclose(handle);

    return 0;
}
