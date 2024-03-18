#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("H. Turgut Uyar");

static int pseudo_major = 0;
static int pseudo_minor = 0;

struct cdev pseudo_cdev;
struct semaphore pseudo_sem;

static int pseudo_capacity = 50;
static char *pseudo_data = NULL;

int pseudo_open(struct inode *inode, struct file *filp) {
    printk(KERN_DEBUG "pseudo: opening device\n");
    return 0;
}

int pseudo_release(struct inode *inode, struct file *filp) {
    printk(KERN_DEBUG "pseudo: closing device\n");
    return 0;
}

ssize_t pseudo_read(struct file *filp, char __user *buf,
                    size_t count, loff_t *f_pos) {
    int i = 0;
    int err = 0;

    if (down_interruptible(&pseudo_sem) != 0) {
        return -ERESTARTSYS;
    }
    if (*f_pos >= pseudo_capacity) {
        up(&pseudo_sem);
        return 0;
    }
    if (*f_pos + count > pseudo_capacity) {
        count = pseudo_capacity - *f_pos;
    }
    for (i = 0; i < count; i++) {
        err = copy_to_user(buf + i, pseudo_data + *f_pos, 1);
        if (err != 0) {
            up(&pseudo_sem);
            return -EFAULT;
        }
	(*f_pos)++;
    }
    up(&pseudo_sem);
    return count;
}

struct file_operations pseudo_fops = {
    .owner   = THIS_MODULE,
    .open    = pseudo_open,
    .release = pseudo_release,
    .read    = pseudo_read,
};

void pseudo_fill(void) {
    int i = 0;

    for (i = 0; i < pseudo_capacity; i++) {
        pseudo_data[i] = i;
    }
}

static int pseudo_init(void) {
    dev_t devno = 0;
    int result = -1;
    int err = 0;

    pseudo_data = kmalloc(pseudo_capacity, GFP_KERNEL);
    if (pseudo_data == NULL) {
	return -ENOMEM;
    }
    pseudo_fill();

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

    sema_init(&pseudo_sem, 1);

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
    kfree(pseudo_data);
}

module_init(pseudo_init);
module_exit(pseudo_exit);
