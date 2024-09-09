obj-m += numa_mmap.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

install:
	insmod numa_mmap.o

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean