obj-m += ili9341_drv.o



#--------------- Test build on Host Machine (x86) -------------------#
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

#--------------------------------------------------------------------#



#--------------- Build on Target device (BBB - ARM) -----------------#
