/*
 *  chardev.c: Creates a read-only char device that says how many times you've read from the dev file
 * */


#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <asm/uaccess.h>        /* for put_user */


#define SUCCESS 0
#define DEVICE_NAME "chardev"   /* Dev name as it appears in /proc/devices */
#define BUF_LEN 80              /* Max length of the message from the device */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Egor Bronnikov");


/*
 *
 * Prototypes
 *
 * */

int init_module(void);
void cleanup_module(void);
static int device_open(struct inode *inode, struct file *file);
static int device_release(struct inode *inode, struct file *file);
static ssize_t device_read(struct file *filp, char __user *buffer, size_t length, loff_t *offset);
static ssize_t device_write(struct file *filp, const char *buffer, size_t length, loff_t *offset);



/* Global variables are declared as static, so are global within the file. */
static int Major;               /* Major number assigned to our device driver */
static int Device_Open = 0;     /* Is device open ? */
                                /* Used to prevent multiple access to device */
static char msg[BUF_LEN];
static char *msg_Ptr;

static struct file_operations fops = {
    /* All this stuff we declare in header "chardev.h" */
    /* file_operations: https://tldp.org/LDP/lkmpg/2.4/html/c577.htm */
    .read = device_read,
    .write = device_write,
    .open = device_open,
    .release = device_release
};


/* This function is called when the module is loaded */
int init_module(void) {
    Major = register_chrdev(0, DEVICE_NAME, &fops);

    if (Major < 0) {
        printk(KERN_ALERT "Registering char device failed with %d\n", Major);
        return Major;
    }

    printk(KERN_INFO "I was assigned major number %d. To talk to\n", Major);
    printk(KERN_INFO "the driver, create a dev file with\n");
    printk(KERN_INFO "'mknod /dev/%s c %d 0'.\n", DEVICE_NAME, Major);
    printk(KERN_INFO "Try various minor numbers. Try to cat and echo to\n");
    printk(KERN_INFO "the device file.\n");
    printk(KERN_INFO "Remove the device file and module when done.\n");

    return SUCCESS;
}


/* This function is called when the module is unloaded */
void cleanup_module(void) {
    /* Unregister the device */
    /* https://www.kernel.org/doc/htmldocs/kernel-api/API---unregister-chrdev.html */
    unregister_chrdev(Major, DEVICE_NAME);
}


/*
 *  
 *  Methods
 * 
 * */


/* Called when a process tries to open the device file, like "cat /dev/mycharfile" */
static int device_open(struct inode *inode, struct file *file) {
    static int counter = 0;

    if (Device_Open)
        /* Device or resource busy (POSIX.1-2001) */
        /* https://man7.org/linux/man-pages/man3/errno.3.html */
        return -EBUSY;

    Device_Open++;
    sprintf(msg, "I already told you %d times Hello world!\n", counter++);
    msg_Ptr = msg;
    try_module_get(THIS_MODULE);

    return SUCCESS;
}


/* Called when a process closes the device file. */
static int device_release(struct inode *inode, struct file *file) {
    Device_Open--;      /* We're now ready for our next caller */

    /* 
     * Decrement the usage count, or else once you opened the file, you'll
     * never get get rid of the module.
     * */
    module_put(THIS_MODULE);

    return 0;
}


/* Called when a process, which already opened the dev file, attempts to read from it. */
static ssize_t device_read(struct file *filp,   /* see include/linux/fs.h */
                           char __user *buffer,        /* buffer to fill with data */
                           size_t length,       /* length of the buffer */
                           loff_t *offset) {
    
    /* Number fo bytes actually written to the buffer */
    int bytes_read = 0;

    /* If we're at the end of the message, return 0 signifying end of file */
    if (*msg_Ptr == 0)
        return 0;

    /* Actually put the data into the buffer */
    while (length && *msg_Ptr) {
        /* The buffer is in the user data segment, not the kernel,
         * segment so "*" assignment won't work. We have to use
         * put_user which copies data from the kernel data segment to
         * the user data segment.
         * */
        put_user(*(msg_Ptr++), buffer++);
        length--;
        bytes_read++;
    }

    /* Most read functions return the number of bytes put into the buffer */
    return bytes_read;
}


/* Called when a process writes to dev file: echo "hi" > /dev/hello */
static ssize_t device_write(struct file *filp, const char *buff, size_t len, loff_t *off) {
    printk(KERN_ALERT "Sorry, this operation isn't supported.\n");
    /* Invalid argument (POSIX.1-2001) */
    /* https://man7.org/linux/man-pages/man3/errno.3.html */
    return -EINVAL;
}
