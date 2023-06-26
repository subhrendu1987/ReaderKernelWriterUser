# Define color codes
RESET = \033[0m
YELLOW = \033[1;33m
GREEN = \033[1;32m
obj-m += reader_module.o

KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
