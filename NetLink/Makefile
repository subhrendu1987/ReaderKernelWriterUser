obj-m += receiver.o

KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
	$(MAKE) user

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
	rm server
user:
	gcc server.c -o server
help:
	  @echo "${YELLOW}"
	  @echo "-----------------------------------------------\n"
	  @echo "               Makefile Help\n"
	  @echo "-----------------------------------------------\n\n"
	  @echo "Usage:\n"
	  @echo "make help; make; make clean\n\n"
	  @echo "After compilation LOAD the LKM using the following command:\n"
	  @echo "${GREEN}"
	  @echo "  sudo insmod reader_module.ko\n"
	  @echo "${YELLOW}"
	  @echo "Execute userspace process\n"
	  @echo "${GREEN}"
	  @echo "  sudo ./user <Message> # e.g. sudo ./user \"Hello\"\n"
	  @echo "${YELLOW}"
	  @echo "UNLOAD the LKM using the following command:\n"
	  @echo "${GREEN}"
	  @echo "sudo rmmod reader_module"
	  @echo "${RESET}"