/* hello-3.c - Demonstrates module documentation. */

#include <linux/module.h>   /* Needed by all modules */
#include <linux/kernel.h>   /* Needed for KERN_INFO */
#include <linux/init.h>     /* Needed for the macros */

#define DRIVER_AUTHOR   "Egor Bronnikov <egor@ebronnikov.xyz>"
#define DRIVER_DESC     "A sample driver"


static int __init init_hello_3(void) {
    printk(KERN_INFO "Hello, world 3\n");
    return 0;
}


static void __exit cleanup_hello_3(void) {
    printk(KERN_INFO "Goodbye, world 3\n");
}


module_init(init_hello_3);
module_exit(cleanup_hello_3);


/* Can use string, like this: */
MODULE_LICENSE("GPL");

/* Or with defines, like this: */
MODULE_AUTHOR(DRIVER_AUTHOR);       /* Who wrote this module? */
MODULE_DESCRIPTION(DRIVER_DESC);    /* What does this module do */

/* This module uses /dev/testdevice. The MODULE_SUPPORTED_DEVICE macros might
 * be used in the future to help automatic configuration of modules, but is
 * currently unused other than documentation purposes.
 * */

/* MODULE_SUPPORTED_DEVICE("testdevice"); */
