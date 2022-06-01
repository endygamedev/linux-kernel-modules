obj-m := chardev.o

all:
	make -C /lib/modules/$$(uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$$(uname -r)/build M=$(PWD) clean

create:
	sudo mknod /dev/chardev c 242 0

delete:
	sudo rm /dev/chardev
