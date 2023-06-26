# Define color codes
RESET = \033[0m
YELLOW = \033[1;33m
GREEN = \033[1;32m

obj-m += reader_module.o

KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
	$(MAKE) user

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
	rm send2kernel
user:
	gcc user_program.c -o send2kernel
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