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
2. Navigate to the directory containing the `reader_module.c` file.
3. Run the following command to compile the kernel module:
`make clean; make`
4. Once the compilation process completes successfully, you will find a file named `reader_module.ko` in the current directory.
5. Load the kernel module using the following command:
`sudo insmod reader_module.ko`
6. Check the kernel log to verify that the module loaded successfully:
`dmesg`
You should see the message "Buffer module loaded" in the output.
6.1 Check and create character device `/dev/reader_module`
   * Get major number of the device from `/proc'
   `cat /proc/devices | grep reader_module`
   * Use `dmesg` to find the major number of the device
   * sudo mknod /dev/reader_module c ${MAJOR_NUMBER_OF_DEVICE} 0
8. To write data to the buffer, open the character device file associated with the module (usually located at /dev/reader_module). For example, you can use the echo command:
`sudo echo "Hello, kernel module!" > /dev/reader_module`
The kernel module will periodically check the buffer and print its contents in the kernel log.
9. To unload the kernel module, run the following command:
`sudo rmmod reader_module`

Check the kernel log again to ensure that the module unloaded successfully. You should see the message "Buffer module unloaded" in the output.

