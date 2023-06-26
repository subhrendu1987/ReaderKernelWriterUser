# Instructions to build and execute
This example represents a Reader-Writer example between kernel and user process. The given LKM which reads from a buffer. The buffer can be written by an user process. The example is created with the help of ChatGPT.
## LKM Reader
```
├─Chardev (Use character devices to interact between userspace process and kernel module)
├─Syscall (Use newly created syscall to interact between userspace process and kernel module) (Not Working So Far)
```

### Environment preparation
1. Install dependencies
```
sudo apt-get update
sudo apt-get install build-essential linux-headers-$(uname -r)
```
## Build module (Chardev)
1. Open a terminal.
2. `cd ReaderKernelWriterUser/Chardev`
3. Compile the kernel module:
```
make clean; make
```
5. Once the compilation process completes successfully, you will find a file named `reader_module.ko` in the current directory.
6. Load the kernel module using the following command:
```
sudo insmod reader_module.ko
```
7. Check the kernel log to verify that the module loaded successfully:
`dmesg`
You should see the message `Reader module initialized` in the output.
8. Check and create character device `/dev/reader_module`
   * Get major number of the device from `/proc'
   ```
    cat /proc/devices | grep reader_module
   ```
   Usually the top entry is the latest one.
   * Use `dmesg` to find the major number of the device (e.g. `New device created with Major Number: 241`)
   * `sudo mknod /dev/reader_module c ${MAJOR_NUMBER_OF_DEVICE} 0` replace ${MAJOR_NUMBER_OF_DEVICE} with the number
   * Enable write priviledge for user programs.
     ```
     sudo chmod 666 /dev/reader_module; ls -al /dev/reader_module
     ```
9. To write data to the buffer, open the character device file associated with the module. For example, you can use the echo command:
```
sudo echo "Hello, kernel module!" > /dev/reader_module
```
The kernel module will periodically check the buffer and print its contents in the kernel log.
10. To unload the kernel module, run the following command:
```
sudo rmmod reader_module
sudo rm /dev/reader_module
```

11. Check the kernel log again to ensure that the module unloaded successfully. You should see the message `Reader module exited` in the output.

## Build module (Syscall)
1. Open a terminal.
2. `cd ReaderKernelWriterUser/Syscall`
3. Compile the kernel module:
```
make clean; make
```
5. Once the compilation process completes successfully, you will find a file named `reader_module.ko` in the current directory.
6. Load the kernel module using the following command:
```
sudo insmod reader_module.ko
```
7. Check the kernel log to verify that the module loaded successfully:
`dmesg`
You should see the message "Buffer module loaded" in the output.