# The Linux Kernel Module Programming

Tring to build own Linux kernel module here.

[The Linux Kernel Module Programming Guide →](https://tldp.org/LDP/lkmpg/2.6/html/index.html)

Check the `/var/log/kern.log`:
```bash
dmesg | tail
```

Insert kernel module:
```bash
sudo insmod <name>.ko
```

Remove kernel module:
```bash
sudo rmmod <name>.ko
```

List of running kernel modules:
```bash
lsmod
```

Module info:
```
modinfo <name>
```
