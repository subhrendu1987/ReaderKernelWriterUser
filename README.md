# Instructions to build and execute
This example represents a Reader-Writer example between kernel and user process. The given LKM which reads from a buffer. The buffer can be written by an user process.
## LKM Reader
### Environment preparation
1. Install dependencies
```
sudo apt-get update
sudo apt-get install build-essential linux-headers-$(uname -r)
```
### Build module
1. Open a terminal.
2. Navigate to the directory containing the `buffer_module.c` file.
3. Run the following command to compile the kernel module:
`make clean; make`
4. Once the compilation process completes successfully, you will find a file named `buffer_module.ko` in the current directory.
5. Load the kernel module using the following command:
`sudo insmod buffer_module.ko`
6. Check the kernel log to verify that the module loaded successfully:
`dmesg`
You should see the message "Buffer module loaded" in the output.

7. To write data to the buffer, open the character device file associated with the module (usually located at /dev/buffer_module). For example, you can use the echo command:
`echo "Hello, kernel module!" > /dev/buffer_module`
The kernel module will periodically check the buffer and print its contents in the kernel log.
8. To unload the kernel module, run the following command:
`sudo rmmod buffer_module`

Check the kernel log again to ensure that the module unloaded successfully. You should see the message "Buffer module unloaded" in the output.

