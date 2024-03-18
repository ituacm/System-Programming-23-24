#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("H. Turgut Uyar");

static int pseudo_major = 0;
static int pseudo_minor = 0;

struct cdev pseudo_cdev;

int pseudo_open(struct inode *inode, struct file *filp) {
    printk(KERN_DEBUG "pseudo: opening device\n");
    return 0;
}

int pseudo_release(struct inode *inode, struct file *filp) {
    printk(KERN_DEBUG "pseudo: closing device\n");
    return 0;
}

struct file_operations pseudo_fops = {
    .owner   = THIS_MODULE,
    .open    = pseudo_open,
    .release = pseudo_release,
};

static int pseudo_init(void) {
    dev_t devno = 0;
    int result = -1;
    int err = 0;

    if (pseudo_major == 0) {
        result = alloc_chrdev_region(&devno, pseudo_minor, 1, "pseudo");
        pseudo_major = MAJOR(devno);
    } else {
        devno = MKDEV(pseudo_major, pseudo_minor);
        result = register_chrdev_region(devno, 1, "pseudo");
    }
    if (result < 0) {
        printk(KERN_WARNING "pseudo: can't get major %d\n", pseudo_major);
        return result;
    }

    cdev_init(&pseudo_cdev, &pseudo_fops);
    pseudo_cdev.owner = THIS_MODULE;
    pseudo_cdev.ops = &pseudo_fops;
    err = cdev_add(&pseudo_cdev, devno, 1);
    if (err != 0) {
        printk(KERN_NOTICE "Error %d adding pseudo device", err);
    }
    return 0;
}

static void pseudo_exit(void) {
    dev_t devno = 0;

    cdev_del(&pseudo_cdev);
    devno = MKDEV(pseudo_major, pseudo_minor);
    unregister_chrdev_region(devno, 1);
}

module_init(pseudo_init);
module_exit(pseudo_exit);
