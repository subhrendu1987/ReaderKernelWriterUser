# Makefile for the example kernel module, system call, and user-space program

KERNEL_DIR := /lib/modules/$(shell uname -r)/build

obj-m += example_module.o

all: example_syscall user_program

example_syscall: example_syscall.c
	gcc -o example_syscall example_syscall.c

user_program: user_program.c
	gcc -o user_program user_program.c

modules:
	make -C $(KERNEL_DIR) M=$(PWD) modules

clean:
	make -C $(KERNEL_DIR) M=$(PWD) clean
	rm -f example_syscall user_program

