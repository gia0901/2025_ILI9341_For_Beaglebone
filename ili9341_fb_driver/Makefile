# Host specifics
KERNEL_DIR_HOST := /lib/modules/$(shell uname -r)/build

# Beaglebone specifics
KERNEL_DIR_BBB := /home/gia/bbb/kernelbuildscripts/KERNEL
TOOLCHAIN_BBB  := /home/gia/bbb/kernelbuildscripts/dl/gcc-8.5.0-nolibc/arm-linux-gnueabi/bin/arm-linux-gnueabi-
	
.PHONY: host, target, all, clean

obj-m := ili9341_drv.o

all,host:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

target:
	make ARCH=arm CROSS_COMPILE=$(TOOLCHAIN_BBB) -C $(KERNEL_DIR_BBB) M=$(PWD) modules

clean:
	@if [ "$(ARCH)" = "arm" ]; then \
		echo "Cleaning for target (ARM)..."; \
		make -C $(KERNEL_DIR_BBB) M=$(PWD) clean; \
	else \
		echo "Cleaning for host (x86)..."; \
		make -C $(KERNEL_DIR_HOST) M=$(PWD) clean; \
	fi
