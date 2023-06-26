obj-m += receiver.o

KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
	$(MAKE) user

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
user:
	gcc netlink_user.c -o netlink_user
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
	  @echo "  sudo ./send2kernel <Message> # e.g. sudo ./send2kernel \"Hello\"\n"
	  @echo "${YELLOW}"
	  @echo "UNLOAD the LKM using the following command:\n"
	  @echo "${GREEN}"
	  @echo "sudo rmmod reader_module"
	  @echo "${RESET}"